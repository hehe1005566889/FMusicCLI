#pragma once

#include "globals.h"
#include "IniParser.h"

#include <filesystem>

class CLIConfig
{
public:
    CLIConfig(const std::string &path)
        : _path(path)
    {
        if (!std::filesystem::exists(path))
            SaveAll();
        ReadAll();
    }

    void ReadAll()
    {
        _parser.reset(new IniParser());
        _parser->Load(_path);

        DownloadFileNamedFormat = _parser->Get("Download", "Format");
    }

    void SaveAll()
    {
        _parser.reset(new IniParser());

        _parser->Set("Download", "Format", DownloadFileNamedFormat);
        _parser->Save(_path);
    }

public:
    std::string DownloadFileNamedFormat = "[NAME]-[ARTIST]-[ALBUM]";

private:
    std::string _path;
    ref<IniParser> _parser;
};