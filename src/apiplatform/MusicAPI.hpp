#pragma once

#include "globals.h"
#include <string>

enum API
{ NetEase };

struct SearchResult
{
    std::string Name;
    std::string Id;
    std::string Artist;
    std::string Album;
};

class MusicAPI
{
public:
    virtual ~MusicAPI() = default;

public:
    virtual void DownloadMusic(const char* id) = 0;
    virtual void DownloadPlayList(const char* id) = 0;
    virtual void SearchMusic(const char* keyword, std::vector<SearchResult>& result) = 0;
    virtual void Login(const char* un, const char* pw) = 0;

public:
    static ref<MusicAPI> CreateMusicAPI(API api);
};