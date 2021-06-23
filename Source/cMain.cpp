#include "cMain.h"
#include <cpr/cpr.h>
#include <json/json.h>
#include <random>

constexpr auto title = "Random Hentai Tool";
const std::string credits1 = "Made by https://github.com/dehoisted";
const std::string credits2 = "https://github.com/dehoisted/Random-Hentai";
const std::string hentai_api = "https://nekos.life/api/v2/img/hentai";

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(69, OnChangeBackground)
EVT_MENU(70, OnResetBackground)
EVT_MENU(71, OnGetCredits)
wxEND_EVENT_TABLE()

bool PathExist(const std::string& s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

namespace Hentai 
{
    const int f_length = 6;
    std::string data, url, filepath;
    std::string api = hentai_api;
    std::string randstr("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    void Check()
    {
        if (!PathExist("null.jpg"))
        {
            std::ofstream null("null.jpg");
            null << "0";
            null.close();
        }
    }

    size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
    }

    bool CheckBackground(const wchar_t* filepath)
    {
        if (SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (PVOID)filepath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE) != 0)
        {
            return true;
        }
        return false;
    }

    std::string GetHentaiUrl()
    {
        cpr::Response r = cpr::Get(cpr::Url{ hentai_api });
        return r.text;
    }

    std::string random_string(int arg)
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(randstr.begin(), randstr.end(), generator);

        return randstr.substr(0, arg);
    }

    std::wstring ExePath() {
        TCHAR buffer[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
        return std::wstring(buffer).substr(0, pos);
    }

    void GetRandomImage()
    {
        filepath = "";
        data = GetHentaiUrl();
        std::ofstream HR("0-responses.json");
        HR << data << std::endl;
        HR.close();

        Json::Value info_file;
        std::ifstream res("0-responses.json");
        res >> info_file;
        url = info_file["url"].asString();
        res.close();

        std::wstring temp1 = Hentai::ExePath();
        std::string exep(temp1.begin(), temp1.end());
        filepath = exep; filepath.append("\\");
        filepath.append(random_string(f_length));
        filepath.append(".jpg");

        std::ofstream log("hentai_log.txt");
        log << url << ", file name: " << filepath << std::endl;
        log.close();

        CURL* curl;
        FILE* fp;
        CURLcode res1;
        const char* outfilename = filepath.c_str();
        curl = curl_easy_init();
        if (curl) {
            fp = fopen(outfilename, "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res1 = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
        }
    }
}

cMain::cMain() : wxFrame(nullptr, wxID_ANY, title, wxPoint(60, 60), wxSize(440, 325))
{
    Hentai::Check();

    extra = new wxMenuBar();
    this->SetMenuBar(extra);

    wxMenu* ex_menu1 = new wxMenu();
    ex_menu1->Append(70, "Reset Background");
    ex_menu1->Append(71, "Credits");
    extra->Append(ex_menu1, "Extra");

	info_text1 = new wxButton(this, wxID_ANY, "Change desktop background to random hentai tool", wxPoint(10, 10), wxSize(285, 55));
    change_bg = new wxButton(this, 69, "Change Background", wxPoint(10, 150), wxSize(160, 55));
	data_log = new wxListBox(this, wxID_ANY, wxPoint(175, 70), wxSize(230, 190));
	data_log->Append(wxT("LOG"));
    data_log->Append("");
}

cMain::~cMain()
{
    wxMessageBox("Bye, Enjoy the Hentai", "Alert");
}

void cMain::OnChangeBackground(wxCommandEvent& evt)
{
    int r;
    Hentai::GetRandomImage();
    data_log->Append(Hentai::url);
    std::wstring temp1 = std::wstring(Hentai::filepath.begin(), Hentai::filepath.end());
    const wchar_t* wpath = temp1.c_str();
    r = SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)wpath, SPIF_UPDATEINIFILE);
    
    switch (Hentai::CheckBackground(wpath))
    {
    case true:
        data_log->Append("Changed background successfully!");
        wxMessageBox("Success!", "Alert");
        break;

    case false:
        data_log->Append("Failed changing background!");
        MessageBoxA(NULL, "Failed!", "Alert", MB_ICONERROR);
        break;
    }
}

void cMain::OnResetBackground(wxCommandEvent& evt)
{
    int r;
    r = SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)"null.jpg", SPIF_UPDATEINIFILE);
    data_log->Append("Reset Background!");
    wxMessageBox("Success!", "Alert");
}

void cMain::OnGetCredits(wxCommandEvent& evt)
{
    wxMessageBox(credits1, "Credits");
    std::string cmd = "start " + credits2;
    system(std::string("start " + credits2).c_str());
}