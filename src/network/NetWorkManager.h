#pragma once

#include "globals.h"

#include <future>
#include <thread>

#include <HTTPRequest.hpp>

#include <sstream>
#include <initializer_list>

static std::string CreateUri(const char* base, std::initializer_list<const char*> replaces)
{
    std::stringstream ss;
    const char* ptr = base;
    for (auto str : replaces) {
        const char* pos = strstr(ptr, "{}");
        if (!pos) {
            break;
        }
        ss << std::string(ptr, pos);
        ss << str;
        ptr = pos + 2;
    }
    ss << ptr;
    return ss.str();
}

struct ManagerInfo
{
    std::string uri;
    std::string method = "GET";
    std::string arguments;
    std::string output;
    http::InternetProtocol protocol = http::InternetProtocol::v4;

    std::string result;
};

class NetWorkManager
{
public:
    NetWorkManager();

    void SendRequestSync(ManagerInfo& info);
    void SendRequestSyncForFile(const ManagerInfo& info, const char* path);

    std::future<void> SendRequestForFileAsync(const ManagerInfo& info, const char* path);
    std::future<std::string> SendRequestAsync(ManagerInfo& info);

    void EncodeUrl(std::string& str);
    void DecodeUrl(std::string& str);
};