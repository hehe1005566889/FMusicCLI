#include "globals.h"

#include <sstream>
#include <initializer_list>
#include <filesystem>
#include "network/JsonTool.hpp"
#include "network/NetWorkManager.h"

#include "apiplatform/MusicAPI.hpp"

#define SERACH_URL "http://cloud-music.pl-fe.cn/search?keywords="
#define LOGIN_URL "http://cloud-music.pl-fe.cn/login/cellphone?"

#define DEBUG(TEXT) printf("=> [DEBUG] %s\n", TEXT);

void ClearAll();
void DrawInfo(std::initializer_list<const char *> infos);
void DrawMessageBox(const char *title, std::initializer_list<const char *> infos);
void DrawQuestion(void (*callback)(const char *select), std::initializer_list<const char *> qeustions);
void DrawMenu(void (*callback)(int select), std::initializer_list<const char *> menus);

static ref<NetWorkManager> Manager;
static bool _p_call_exit = false;
static bool _p_login = false;

void CheckStage();

void SearchMusicForId();
void DownloadPlayList();
void DownloadMusic();
void Login();

static ref<MusicAPI> _api;

int main(int argc, char **argv)
{
    _api = MusicAPI::CreateMusicAPI(API::NetEase);

    Manager.reset(new NetWorkManager());
    CheckStage();
    ClearAll();

    while (!_p_call_exit)
    {

        DrawInfo({"NetEaseTool V1", "Select Mode"});
        DrawMenu([](int id)
                 {
        switch(id)
        {
            case 1:
            SearchMusicForId();
            break;
            case 2:
            DownloadMusic();
            break;
            case 3:

            break;
            case 4:
            Login();
            break;
            case 5:
            _p_call_exit = !_p_call_exit;
            break;
            
        } }, {"Search Music For Id", "Download Music[Need Login]", "Download PlayList", "Login", "Exit"});
    }
    return EXIT_SUCCESS;
}

void ClearAll()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void DrawInfo(std::initializer_list<const char *> infos)
{
    printf("[Info] =================================\n");
    for (auto text : infos)
    {
        printf("= %s\n", text);
    }
}

void DrawMessageBox(const char *title, std::initializer_list<const char *> infos)
{
    printf("[%s] =================================\n", title);
    for (auto text : infos)
    {
        printf("= %s\n", text);
    }
    printf("======================================\n", title);
}

void DrawQuestion(void (*callback)(const char *select), std::initializer_list<const char *> qeustions)
{
    printf("[QuestionBox] =================================\n");
    for (auto text : qeustions)
    {
        printf("= %s\n", text);
    }
    printf("> ");

    std::cin.ignore();
    std::string text;
    std::getline(std::cin, text);
    std::cout << std::endl;

    callback(text.c_str());
}

void DrawMenu(void (*callback)(int select), std::initializer_list<const char *> menus)
{
    printf("[Menu] =================================\n");
    int id = 0;
    for (auto text : menus)
    {
        printf("- %i. %s\n", id + 1, text);
        id++;
    }
    printf("> ");
    std::cin >> id;
    printf("\n");
    callback(id);
}

void CheckStage()
{
    _p_login = std::filesystem::exists("login.def");
    std::filesystem::create_directory("musics");
    std::filesystem::create_directory("playlists");
}

static std::string keyword;
void SearchMusicForId()
{
    DrawQuestion([](const char *result){ assert(result); keyword = result; }, {"Song Name Or Artist Name:"});
    
    std::vector<SearchResult> result;
    _api->SearchMusic(keyword.c_str(), result);

    for(auto item : result)
    printf("=> [ID:%s] [Name:%s] [Artist:%s] [Album:%s]\n", item.Id.c_str(), item.Name.c_str(), item.Artist.c_str(), item.Album.c_str());
    printf("\n");

    keyword.clear();
}

static std::string playlist;
void DownloadPlayList()
{
    DrawQuestion([](const char* data) {playlist = data;}, {"Please Enter PlayList Id -> ", ">? How To Get The Id?", "> At The End Of Your PlayList WebPage's Uri" });


    ManagerInfo info;
    std::string uri("http://music.163.com/api/song/enhance/player/url?id=123456&ids=[");
    uri.append(playlist).append("]&br=3200000");
    info.uri = uri;

    Manager->SendRequestSync(info);

    printf("=> Debug %s\n", info.output.c_str());
}

static std::string musicid;
void DownloadMusic()
{
    DrawQuestion([](const char* data) {musicid = data;}, {"Please Enter Music Id -> ", ">? How To Get The Id?", "> Using Mode 1 To Search Music Id" });

    ManagerInfo info;
    info.uri = CreateUri("http://music.163.com/api/song/enhance/player/url?id=123456&ids=[{}]&br=3200000", { musicid.c_str() });

    Manager->SendRequestSync(info);
    auto root = json::parse(info.output);

    auto url = root["data"][0]["url"];
    DEBUG(url.asCString());
    auto type = root["data"][0]["type"];

    ManagerInfo info1;
    info1.uri = CreateUri("http://cloud-music.pl-fe.cn/song/detail?ids={}", {musicid.c_str()});

    Manager->SendRequestSync(info1);
    auto root2 = json::parse(info1.output);
    auto songname = root2["songs"][0]["name"].asCString();

    std::string artists;
    for (auto item : root2["songs"][0]["ar"])
    {
        artists.append(item["name"].asCString()).append("-");
    }
    artists.substr(artists.find_last_of("-") - 1);
    DELOBJ(info1);

    ManagerInfo info2;
    info2.uri = (url.asCString());

    std::string name;
    name.append(songname).append("=").append(artists).append(".").append(type.asCString()); //todo replace m4a filetype

    Manager->SendRequestSyncForFile(info2, name.c_str());
    musicid.clear();
    DELOBJ(info);
    DELOBJ(info2);

    DrawInfo({"Download Started...", "If There No Any 'File Open Error' Or 'Err XXXXXX' Notification", "That Mean Music Download Succeed"});
}

static std::string username;
static std::string password;
void Login()
{

    DrawQuestion([](const char *un)
                 { username = un; },
                 {"Please Enter Your NetEase Account UserName -> "});
    DrawQuestion([](const char *pw)
                 { password = pw; },
                 {"Please Enter Your NetEase Account PassWord -> "});

    ManagerInfo info1;

    std::string url1(LOGIN_URL);
    printf("=> Debug %s,%s\n\n", username.c_str(), password.c_str());
    url1.append("phone=").append(username).append("&password=").append(password);
    info1.uri = url1;

    printf("=> uri: %s\n", url1.c_str());
    Manager->SendRequestSync(info1);

    auto result = info1.output;
    printf("=> Result %s \n", result.c_str());

    username.clear();
    password.clear();
}
