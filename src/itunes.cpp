#include <string>
#include <codecvt>
#include <tchar.h>
#include "itunes.h"
#include "iTunesCOMInterface.h"

namespace itunes_win
{
    void setup()
    {
        CoInitialize(nullptr);
    }

    void tear_down()
    {
        CoUninitialize();
    }

    static std::string wstring_to_utf8_string(const std::wstring& wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
        return cvt.to_bytes(wstr);
    }

    static com_unique_ptr<IITTrack> get_current_track()
    {
        com_unique_ptr<IiTunes> itunes;
        if (CoCreateInstance(CLSID_iTunesApp, nullptr, CLSCTX_LOCAL_SERVER, IID_IiTunes, reinterpret_cast<void**>(&itunes)) != S_OK || !itunes) {
            throw std::exception("CoCreateInstance failed.");
        }
        
        com_unique_ptr<IITTrack> track;
        if (itunes->get_CurrentTrack(reinterpret_cast<IITTrack**>(&track)) != S_OK || !track) {
            itunes->Release(); // why?
            throw std::exception("get_CurrentTrack failed.");
        }
        return std::move(track);
    }

    bool itunes_process_exists()
    {
        return FindWindow(_T("iTunes"), _T("iTunes"));
    }

    const std::string get_current_track_name()
    {
        auto track = get_current_track();

        _bstr_t trackName;
        if (track->get_Name(trackName.GetAddress()) != S_OK) {
            throw std::exception("track->get_Name failed.");
        }

        std::wstring ws(trackName, trackName.length());
        return wstring_to_utf8_string(ws);
    }
}