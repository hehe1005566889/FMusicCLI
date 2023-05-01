#pragma once

#include "globals.h"
#include <string>
#include <vector>

enum API
{ NetEase };

struct SearchResult
{
    std::string Name;
    std::string Id;
    std::string Artist;
    std::string Album;
};

struct PlayListInfo
{
    std::string Name;
    std::string Info;

    std::vector<int> Ids;

    int index;
    int max;
};

class MusicAPI
{
public:
    virtual ~MusicAPI() = default;

public:
    virtual void DownloadMusic(const char* id, const char* path = "") = 0;
    virtual void FetchPlayListInfo(const char* id, PlayListInfo& info) = 0;
    virtual void DownloadPlayList(PlayListInfo& info, void(*callback)(const PlayListInfo& info)) = 0;
    virtual void SearchMusic(const char* keyword, std::vector<SearchResult>& result) = 0;
    virtual void Login(const char* un, const char* pw) = 0;

public:
    static ref<MusicAPI> CreateMusicAPI(API api);
};