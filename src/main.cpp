#include "globals.h"

#include <sstream>
#include <initializer_list>
#include <filesystem>
#include "network/JsonTool.hpp"
#include "network/NetWorkManager.h"

#include "apiplatform/MusicAPI.hpp"

#define LRYIC_URL "http://cloud-music.pl-fe.cn/lyric?id={}"
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
            DownloadPlayList();
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
    try{
    std::filesystem::create_directory("musics");
    std::filesystem::create_directory("playlists");
    }catch(std::exception)
    { }
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

    PlayListInfo pinfo;
    _api->FetchPlayListInfo(playlist.c_str(), pinfo);

    DrawInfo({ "PlayList Info ->", "PlayList Name :", pinfo.Name.c_str(), "PlayList Info :",  pinfo.Info.c_str(), "Play List Music Count :", std::to_string(pinfo.Ids.size()).c_str(), ""});
    
    int index = 1;
    _api->DownloadPlayList(pinfo, [](const PlayListInfo& info){
        printf("=> Download Index %i/%i\n", info.index, info.max);
    });

    playlist.clear();
}

static std::string musicid;
void DownloadMusic()
{
    DrawQuestion([](const char* data) {musicid = data;}, {"Please Enter Music Id -> ", ">? How To Get The Id?", "> Using Mode 1 To Search Music Id" });
    _api->DownloadMusic(musicid.c_str());
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
