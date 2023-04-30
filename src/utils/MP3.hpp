#pragma once

#include "globals.h"
#include <fstream>
#include <cstring>

class MP3File
{
public:
    MP3File(const char *filename)
    {
        file_.open(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (!file_.is_open())
        {
            throw std::runtime_error("Failed to open file.");
        }
        read_id3_tag();
    }

    ~MP3File()
    {
        if (file_.is_open())
        {
            file_.close();
        }
    }

    void set_title(const char *title)
    {
        set_id3_text_field("TIT2", title);
    }

    void set_artist(const char *artist)
    {
        set_id3_text_field("TPE1", artist);
    }

    void set_album(const char *album)
    {
        set_id3_text_field("TALB", album);
    }

    void save()
    {
        if (!file_.is_open())
        {
            throw std::runtime_error("File is not open.");
        }
        if (tag_start_ < 0)
        {
            throw std::runtime_error("ID3 tag not found.");
        }
        int tag_size = tag_end_ - tag_start_;
        file_.seekp(tag_start_ + 6, std::ios::beg);
        file_.put((tag_size >> 21) & 0x7F);
        file_.put((tag_size >> 14) & 0x7F);
        file_.put((tag_size >> 7) & 0x7F);
        file_.put(tag_size & 0x7F);
    }

private:
    std::fstream file_;
    int tag_start_ = -1;
    int tag_end_ = -1;

    void read_id3_tag()
    {
        char id3_tag[3];
        file_.read(id3_tag, 3);
        if (strncmp(id3_tag, "ID3", 3) == 0)
        {
            char tag_size_bytes[4];
            file_.read(tag_size_bytes, 4);
            int tag_size = 0;
            for (int i = 0; i < 4; i++)
            {
                tag_size = (tag_size << 7) | (tag_size_bytes[i] & 0x7F);
            }
            tag_start_ = file_.tellg();
            tag_end_ = tag_start_ + tag_size;
        }
    }

    void set_id3_text_field(const char *field_name, const char *value)
    {
        if (tag_start_ < 0)
        {
            throw std::runtime_error("ID3 tag not found.");
        }
        int field_size = strlen(value) + 1;
        char buf[10];
        file_.seekg(tag_start_, std::ios::beg);
        while (file_.tellg() < tag_end_)
        {
            file_.read(buf, 4);
            int frame_size = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
            if (strncmp(buf, field_name, 4) == 0)
            {
                file_.seekp(-4, std::ios::cur);
                if (field_size > frame_size)
                {
                    char *new_buf = new char[tag_end_ - file_.tellp() - 10 + field_size];
                    file_.read(buf, 10);
                    char* next_frame = new_buf;
                    memcpy(next_frame, buf, 10);
                    next_frame += 10;
                    int read_size = 10;
                    while (read_size < frame_size) {
                        int chunk_size = std::min(frame_size - read_size, 1024);
                        file_.read(next_frame, chunk_size);
                        next_frame += chunk_size;
                        read_size += chunk_size;
                    }
                    memcpy(next_frame, value, field_size);
                    next_frame += field_size - 1;
                    file_.write(new_buf, next_frame - new_buf);
                    delete[] new_buf;
                }
                else
                {
                    file_.write(field_name, 4);
                    file_.write("\x00\x00\x00\x00", 4);
                    file_.put(0);
                    file_.write(value, field_size);
                    int padding_size = frame_size - field_size - 1;
                    if (padding_size > 0)
                    {
                        file_.write("\x00", 1);
                        for (int i = 0; i < padding_size; i++)
                        {
                            file_.write("\x00", 1);
                        }
                    }
                }
                return;
            }
            file_.seekg(frame_size, std::ios::cur);
        }
        // Field not found, create a new one
        file_.seekp(tag_end_, std::ios::beg);
        char new_field[10];
        strncpy(new_field, field_name, 4);
        new_field[4] = 0;
        new_field[5] = 0;
        new_field[6] = 0;
        new_field[7] = field_size & 0xFF;
        new_field[8] = 0;
        new_field[9] = 0;
        file_.write(new_field, 10);
        file_.put(0);
        file_.write(value, field_size);
    }
};