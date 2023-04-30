#pragma once

#include "globals.h"

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

class IniParser
{
public:
    IniParser() {}
    IniParser(const std::string &config)
    {
        Parse(config);
    }

public:
    bool Load(const std::string &file_path)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            return false;
        }

        std::string line, section_name;
        while (std::getline(file, line))
        {
            // 去除空格
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

            // 忽略注释和空行
            if (line.empty() || line[0] == ';' || line[0] == '#')
            {
                continue;
            }

            // 解析节名
            if (line[0] == '[' && line[line.length() - 1] == ']')
            {
                section_name = line.substr(1, line.length() - 2);
            }
            else
            {
                // 解析键值对
                std::istringstream iss(line);
                std::string key, value;
                std::getline(iss, key, '=');
                std::getline(iss, value);

                // 添加键值对到映射表中
                sections_[section_name][key] = value;
            }
        }

        file.close();
        return true;
    }

    std::string Get(const std::string &section, const std::string &key) const
    {
        auto it = sections_.find(section);
        if (it != sections_.end())
        {
            auto it2 = it->second.find(key);
            if (it2 != it->second.end())
            {
                return it2->second;
            }
        }
        return "";
    }

    void Set(const std::string &section, const std::string &key, const std::string &value)
    {
        sections_[section][key] = value;
    }

    bool Save(const std::string &file_path) const
    {
        std::ofstream file(file_path);
        if (!file.is_open())
        {
            return false;
        }

        for (const auto &section : sections_)
        {
            file << "[" << section.first << "]" << std::endl;
            for (const auto &key_value : section.second)
            {
                file << key_value.first << "=" << key_value.second << std::endl;
            }
            file << std::endl;
        }

        file.close();
        return true;
    }

private:
    void Parse(const std::string &str)
    {
        std::string line, section_name;
        std::stringstream ss(str);

        std::string line;
        while (std::getline(ss, line, '\n'))
        {
            // 去除空格
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

            // 忽略注释和空行
            if (line.empty() || line[0] == ';' || line[0] == '#')
            {
                continue;
            }

            // 解析节名
            if (line[0] == '[' && line[line.length() - 1] == ']')
            {
                section_name = line.substr(1, line.length() - 2);
            }
            else
            {
                // 解析键值对
                std::istringstream iss(line);
                std::string key, value;
                std::getline(iss, key, '=');
                std::getline(iss, value);

                // 添加键值对到映射表中
                sections_[section_name][key] = value;
            }
        }
    }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sections_;
};
