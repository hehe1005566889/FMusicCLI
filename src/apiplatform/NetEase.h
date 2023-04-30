#pragma once

#include "globals.h"
#include "MusicAPI.hpp"
#include "network/NetWorkManager.h"

class NetEaseAPI : public MusicAPI
{
public:
    NetEaseAPI();

public:
    virtual void DownloadMusic(const char* id) override;
    virtual void DownloadPlayList(const char* id) override;
    virtual void SearchMusic(const char* keyword, std::vector<SearchResult>& result) override;
    virtual void Login(const char* un, const char* pw) override;

private:
    ref<NetWorkManager> _manager;
};