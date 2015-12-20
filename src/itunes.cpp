#include <string>
#include <codecvt>
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

    const std::string get_current_track_name()
    {
        com_ptr<IiTunes> itunes;
        if (CoCreateInstance(CLSID_iTunesApp, nullptr, CLSCTX_LOCAL_SERVER, IID_IiTunes, reinterpret_cast<void**>(&itunes)) != S_OK) {
            throw std::exception("CoCreateInstance failed.");
        }

        com_ptr<IITTrack> track;
        if (itunes->get_CurrentTrack(&track) != S_OK) {
            throw std::exception("get_CurrentTrack failed.");
        }

        _bstr_t trackName;
        if (track->get_Name(trackName.GetAddress()) != S_OK) {
            throw std::exception("track->get_Name failed.");
        }

        std::wstring ws(trackName, trackName.length());
        return wstring_to_utf8_string(ws);
    }
}