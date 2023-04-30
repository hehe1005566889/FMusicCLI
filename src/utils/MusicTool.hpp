#pragma once

#include "globals.h"
#include <string>

struct MusicInfo {
    std::string musicFile;

    std::string name;
    std::string artist;
    std::string album;
    std::string imageUri;
};

class MusicTool
{
public:
   static void ApplyToMp3File(const MusicInfo& info);
};