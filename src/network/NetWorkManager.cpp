#include "NetWorkManager.h"

#include <fstream>
#include <filesystem>
#include <ostream>

static unsigned char ToHex(unsigned char x)
{
    return x > 9 ? x + 55 : x + 48;
}

static unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z')
        y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z')
        y = x - 'a' + 10;
    else if (x >= '0' && x <= '9')
        y = x - '0';
    else
        assert(0);
    return y;
}

static std::string UrlEncode(const std::string &str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

static std::string UrlDecode(const std::string &str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+')
            strTemp += ' ';
        else if (str[i] == '%')
        {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high * 16 + low;
        }
        else
            strTemp += str[i];
    }
    return strTemp;
}

NetWorkManager::NetWorkManager()
{
}

void NetWorkManager::SendRequestSyncForFile(const ManagerInfo &info, const char *path)
{
    try
    {
        http::Request request{info.uri, info.protocol};

        const auto response = request.send(info.method, info.arguments, {{"Content-Type", "application/x-www-form-urlencoded;charset=utf-8"}, {"User-Agent", "runscope/0.1"}, {"Set-Cookie", "SameSite=None"}, {"Accept", "*/*"}}, std::chrono::seconds(2));

        // std::cout << response.status.reason << '\n';

        if (response.status.code == http::Status::Ok)
        {
            printf("=> DEBUG %i [%s]\n", response.body.size(), path);

            std::ofstream outfile(path, std::ios::out | std::ios::binary);

            if(!outfile.is_open())
            printf("=> [NetMan] Open File Faild");

            outfile.write(reinterpret_cast<const char *>(response.body.data()),
                          static_cast<std::streamsize>(response.body.size()));
            //outfile.flush();
            outfile.close();
        }
    }
    catch (const http::RequestError &e)
    {
        std::cerr << "Request error: " << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    catch (const http::ResponseError &e)
    {
        std::cerr << "Response error: " << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        // return EXIT_FAILURE;
    }
}

void NetWorkManager::DecodeUrl(std::string &str)
{
    str = UrlDecode(str);
}

void NetWorkManager::EncodeUrl(std::string &str)
{
    str = UrlEncode(str);
}

void NetWorkManager::SendRequestSync(ManagerInfo &info)
{
    try
    {
        http::Request request{info.uri, info.protocol};

        const auto response = request.send(info.method, info.arguments, {{"Content-Type", "application/x-www-form-urlencoded;charset=utf-8"}, {"User-Agent", "runscope/0.1"}, {"Set-Cookie", "SameSite=None"}, {"Accept", "*/*"}}, std::chrono::seconds(2));

        // std::cout << response.status.reason << '\n';

        if (response.status.code == http::Status::Ok)
            info.output = std::string{response.body.begin(), response.body.end()}; // << '\n';
    }
    catch (const http::RequestError &e)
    {
        std::cerr << "Request error: " << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    catch (const http::ResponseError &e)
    {
        std::cerr << "Response error: " << e.what() << '\n';
        // return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        // return EXIT_FAILURE;
    }
}

std::future<void> NetWorkManager::SendRequestForFileAsync(const ManagerInfo& info, const char* path)
{
    return std::async(std::launch::async, [this, info, path]() {
        SendRequestSyncForFile(info, path);
    });
}

std::future<std::string> NetWorkManager::SendRequestAsync(ManagerInfo& info)
{
    return std::async(std::launch::async, [this, &info]() {
        SendRequestSync(info);
        return info.output;
    });
}

