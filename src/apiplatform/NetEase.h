#pragma once

#include "globals.h"
#include "MusicAPI.hpp"
#include "network/NetWorkManager.h"

class NetEaseAPI : public MusicAPI
{
public:
    NetEaseAPI();

public:
    virtual void DownloadMusic(const char *id, const char *path) override;

    virtual void FetchPlayListInfo(const char *id, PlayListInfo &info) override;
    virtual void DownloadPlayList(PlayListInfo &info, void(*callback)(const PlayListInfo& info)) override;

    virtual void SearchMusic(const char *keyword, std::vector<SearchResult> &result) override;
    virtual void Login(const char *un, const char *pw) override;

private:
    std::string CreatePath(const std::string &path, const std::string &filename);

private:
    ref<NetWorkManager> _manager;
};