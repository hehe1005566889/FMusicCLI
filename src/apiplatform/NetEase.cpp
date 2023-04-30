#include "NetEase.h"

#include "network/JsonTool.hpp"


#define SERACH_URL "http://cloud-music.pl-fe.cn/search?keywords={}"
#define LOGIN_URL "http://cloud-music.pl-fe.cn/login/cellphone?username={}&password={}"

NetEaseAPI::NetEaseAPI()
{
    _manager = create_ref<NetWorkManager>();
}

void NetEaseAPI::DownloadMusic(const char *id)
{
}

void NetEaseAPI::DownloadPlayList(const char *id)
{
}

void NetEaseAPI::SearchMusic(const char *keyword, std::vector<SearchResult>& result)
{
    ManagerInfo info;

    std::string txt(keyword);
    _manager->EncodeUrl(txt);
    info.uri = CreateUri(SERACH_URL, {txt.c_str()});

    _manager->SendRequestSync(info);

    auto item = json::parse(info.output);
    for (auto song : item["result"]["songs"])
    {
        auto id = song["id"].asInt();
        auto name = song["name"].asCString();
        
        std::string artists;
        for (auto item : song["artists"])
            artists.append(item["name"].asCString()).append("/");
        artists.substr(artists.find_last_of("/")-1);

        auto album = song["album"]["name"].asCString();

        SearchResult aresult;
        aresult.Album = album; aresult.Artist = artists.c_str();
        aresult.Id = std::to_string(id).c_str(); aresult.Name = name;

        result.push_back(aresult);
    }

    DELOBJ(info);
}

void NetEaseAPI::Login(const char *un, const char *pw)
{
}
