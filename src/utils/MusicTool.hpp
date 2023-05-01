#pragma once

#include "globals.h"

#include <filesystem>
#include <fstream>

#include <taglib.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <id3v2frame.h>
#include <fileref.h>
#include <tbytevector.h>
#include <tpropertymap.h>

#include <attachedpictureframe.h>
#include <textidentificationframe.h>
#include <unsynchronizedlyricsframe.h>
#include <synchronizedlyricsframe.h>

namespace mp3tool
{
    struct AlbumImage
    {
        TagLib::ByteVector _image;
    };
    using AlbumImageHandler = AlbumImage *;

    struct MusicFileInfo
    {
        std::string path;

        std::string title;
        std::string artist;
        std::string album;

        std::string lryic;
        AlbumImageHandler image;
    };

    static void CreateImage(MusicFileInfo &info, const char *path)
    {
        TagLib::ByteVector coverData;
        std::ifstream coverFile(path, std::ios::binary);
        if (coverFile.is_open())
        {
            coverFile.seekg(0, std::ios::end);
            coverData.resize(coverFile.tellg());
            coverFile.seekg(0, std::ios::beg);
            coverFile.read(coverData.data(), coverData.size());
            coverFile.close();
        }
        auto image = new AlbumImage();
        image->_image = coverData;

        info.image = image;
    }

    static void Load(MusicFileInfo &info)
    {
        if (info.path.size() == 0)
            throw std::runtime_error("Music File Path Is Empty");

        if (!std::filesystem::exists(info.path))
            throw std::runtime_error("Music File Can't Find");

        TagLib::MPEG::File file(info.path.c_str());
        TagLib::Tag *tag = file.tag();

        info.album.clear();
        info.artist.clear();
        info.title.clear();

        info.album.append(tag->album().toCString());
        info.title.append(tag->title().toCString());
        info.artist.append(tag->artist().toCString());

        DELOBJ(file);
    }

    static void Save(MusicFileInfo &info)
    {
        if (info.path.size() == 0)
            throw std::runtime_error("Music File Path Is Empty");

        if (!std::filesystem::exists(info.path))
            throw std::runtime_error("Music File Can't Find");

        TagLib::MPEG::File file(info.path.c_str());

        auto tag = file.ID3v2Tag(true);

        TagLib::String album(info.album, TagLib::String::Type::UTF8);
        tag->setAlbum(album);

        TagLib::String artist(info.artist, TagLib::String::Type::UTF8);
        tag->setArtist(artist);

        TagLib::String title(info.title, TagLib::String::Type::UTF8);
        tag->setTitle(title);

        TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame();
        frame->setMimeType("image/jpeg");
        frame->setPicture(info.image->_image);
        tag->addFrame(frame);

        TagLib::ID3v2::UnsynchronizedLyricsFrame *frame2 = new TagLib::ID3v2::UnsynchronizedLyricsFrame();
        frame2->setText(info.lryic);
        frame2->setDescription("LRC");

        file.save();
        DELOBJ(file);
    }

    static void Free(MusicFileInfo &info)
    {
        delete info.image;
        memset(&info, 0, sizeof(info));
    }
}