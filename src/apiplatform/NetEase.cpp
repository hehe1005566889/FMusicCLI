#include "NetEase.h"

#include "network/JsonTool.hpp"

#include "utils/MusicTool.hpp"
#include <filesystem>
#include <vector>

#define PLAYLIST_URL "http://cloud-music.pl-fe.cn/playlist/detail?id={}"
#define GETSONGDETIL_URL "http://cloud-music.pl-fe.cn/song/detail?ids={}"
#define GETMUSIC_URL "http://music.163.com/api/song/enhance/player/url?id=123456&ids=[{}]&br=3200000"
#define LRYIC_URL "http://cloud-music.pl-fe.cn/lyric?id={}"
#define SERACH_URL "http://cloud-music.pl-fe.cn/search?keywords={}"
#define LOGIN_URL "http://cloud-music.pl-fe.cn/login/cellphone?username={}&password={}"

#define Async false

NetEaseAPI::NetEaseAPI()
{
    _manager = create_ref<NetWorkManager>();
}

void NetEaseAPI::DownloadMusic(const char *id, const char *path)
{
    try
    {
        ManagerInfo info;
        info.uri = CreateUri(GETMUSIC_URL, {id});

        _manager->SendRequestSync(info);
        auto root = json::parse(info.output);

        auto url = root["data"][0]["url"];
        auto type = root["data"][0]["type"];

        ManagerInfo info1;
        info1.uri = CreateUri(GETSONGDETIL_URL, {id});

        _manager->SendRequestSync(info1);
        auto root2 = json::parse(info1.output);
        auto songname = root2["songs"][0]["name"].asCString();
        auto album = root2["songs"][0]["al"]["name"].asCString();

        // Download Album Picture To temp.data File For Template
        ManagerInfo info3;
        info3.uri = root2["songs"][0]["al"]["picUrl"].asCString();
        info3.uri = info3.uri.replace(0, 5, "http");
        _manager->SendRequestSyncForFile(info3, "temp.data");
        DELOBJ(info3);
        // =====================================================

        std::string artists;
        for (auto item : root2["songs"][0]["ar"])
            artists.append(item["name"].asCString()).append(";");
        DELOBJ(info1);

        ManagerInfo info2;
        info2.uri = (url.asCString());

        std::string name;
        name.append(path).append("//").append(songname).append("=").append(artists).append(".").append(type.asCString()); // todo replace m4a filetype

        _manager->SendRequestSyncForFile(info2, name.c_str());

        if(type.asCString() == "m4a")
        return;

        ManagerInfo info4;
        info4.uri = CreateUri(LRYIC_URL, {id});
        _manager->SendRequestSync(info4);
        auto root3 = json::parse(info4.output);

        auto lryic = root3["lrc"]["lyric"].asString();

        mp3tool::MusicFileInfo minfo;
        minfo.path = name;
        mp3tool::Load(minfo);

        minfo.artist = artists;
        minfo.title = songname;
        minfo.album = album;
        minfo.lryic = lryic;
        mp3tool::CreateImage(minfo, "temp.data");

        mp3tool::Save(minfo);
        mp3tool::Free(minfo);
        DELOBJ(info);
        DELOBJ(info2);
        DELOBJ(info4);
    }
    catch (const Json::Exception &e)
    {
        return;
    }
    std::filesystem::remove("temp.data");
}

void NetEaseAPI::FetchPlayListInfo(const char *id, PlayListInfo &info)
{
    ManagerInfo info1;
    info1.uri = CreateUri(PLAYLIST_URL, {id});
    _manager->SendRequestSync(info1);
    auto root = json::parse(info1.output);

    auto playlist = root["playlist"];

    info.Name = playlist["name"].asCString();
    info.Info = playlist["description"].asCString();

    std::vector<int> ids;
    for (auto item : playlist["trackIds"])
        ids.push_back(item["id"].asInt());

    info.Ids = ids;
    DELOBJ(info1);
}

void NetEaseAPI::DownloadPlayList(PlayListInfo &info, void (*callback)(const PlayListInfo &info))
{
    if (std::filesystem::exists(info.Name))
        std::filesystem::remove_all(info.Name);
    std::filesystem::create_directory(info.Name);

    info.max = info.Ids.size();
    info.index = 1;

    for (auto id : info.Ids)
    {
        auto sid = std::to_string(id).c_str();
        DownloadMusic(sid, info.Name.c_str());
        callback(info);

        info.index++;
    }
}

void NetEaseAPI::SearchMusic(const char *keyword, std::vector<SearchResult> &result)
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
        artists.substr(artists.find_last_of("/") - 1);

        auto album = song["album"]["name"].asCString();

        SearchResult aresult;
        aresult.Album = album;
        aresult.Artist = artists.c_str();
        aresult.Id = std::to_string(id).c_str();
        aresult.Name = name;

        result.push_back(aresult);
    }

    DELOBJ(info);
}

void NetEaseAPI::Login(const char *un, const char *pw)
{
}

std::string NetEaseAPI::CreatePath(const std::string &path, const std::string &filename)
{
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path subPath = std::filesystem::path(path) / std::filesystem::path(filename);
    std::filesystem::path fullPath = currentPath / subPath;
    return fullPath.string(); // 将 std::filesystem::path 转换为 std::string
}
