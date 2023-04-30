#include "MusicAPI.hpp"

#include "NetEase.h"

ref<MusicAPI> MusicAPI::CreateMusicAPI(API api)
{
    switch (api)
    {
        case API::NetEase:
        return create_ref<NetEaseAPI>();
        break;
    }

    throw std::runtime_error("unknown api was been setted");
    return nullptr;
}
