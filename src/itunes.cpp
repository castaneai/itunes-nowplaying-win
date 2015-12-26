#include <string>
#include <codecvt>
#include <tchar.h>
#include <cstdio>
#include <fstream>
#include <functional>
#include "itunes.h"
#include "iTunesCOMInterface.h"

namespace itunes_win
{
    namespace util
    {
        std::string wstringToUTF8String(const std::wstring& wstr)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
            return cvt.to_bytes(wstr);
        }

        std::string bstrToUTF8String(const _bstr_t& bstr)
        {
            return wstringToUTF8String(std::wstring(bstr, bstr.length()));
        }

        ArtworkFormat getArtworkFormat(IITArtwork* const artwork)
        {
            ITArtworkFormat format;
            if (artwork->get_Format(&format) != S_OK) throw std::exception("IITArtwork get_Format failed.");
            switch (format)
            {
            case ITArtworkFormatJPEG:
                return ArtworkFormat::JPEG;

            case ITArtworkFormatPNG:
                return ArtworkFormat::PNG;

            case ITArtworkFormatBMP:
                return ArtworkFormat::BMP;

            default:
                return ArtworkFormat::UNKNOWN;
            }
        }

        std::string readAllContentsFromFile(const std::string& filePath)
        {
            std::ifstream file(filePath, std::ios::in | std::ios::binary);
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            return content;
        }
    }

    const bool iTunesProcessExists()
    {
        return FindWindow(_T("iTunes"), _T("iTunes")) != 0;
    }

    const Track getCurrentTrack()
    {
        CoInitialize(nullptr);
        try {
            com_unique_ptr<IiTunes> itunes;
            auto res = CoCreateInstance(CLSID_iTunesApp, nullptr, CLSCTX_LOCAL_SERVER, IID_IiTunes, reinterpret_cast<void**>(&itunes));
            if (res != S_OK) {
                throw std::exception("CoCreateInstance failed.");
            }

            com_unique_ptr<IITTrack> pTrack;
            if (itunes->get_CurrentTrack(reinterpret_cast<IITTrack**>(&pTrack)) != S_OK) {
                throw std::exception("get_CurrentTrack failed.");
            }

            Track result;

            _bstr_t trackName;
            if (pTrack->get_Name(trackName.GetAddress()) != S_OK) throw std::exception("track->get_Name failed.");
            result.name = util::bstrToUTF8String(trackName);

            _bstr_t trackArtist;
            if (pTrack->get_Artist(trackArtist.GetAddress()) != S_OK) throw std::exception("track->get_Artist failed.");
            result.artist = util::bstrToUTF8String(trackArtist);

            com_unique_ptr<IITArtworkCollection> pArtworks;
            if (pTrack->get_Artwork(reinterpret_cast<IITArtworkCollection**>(&pArtworks)) != S_OK) throw std::exception("track->get_Artwork failed.");

            // TODO: multiple artworks
            long artworkCount = 0;
            if (pArtworks->get_Count(&artworkCount) != S_OK) throw std::exception("IITArtworkCollection get_Count failed.");
            if (artworkCount > 0) {
                com_unique_ptr<IITArtwork> pArtwork;
                if (pArtworks->get_Item(1, reinterpret_cast<IITArtwork**>(&pArtwork)) != S_OK) throw std::exception("IITArtworkCollection get_Item failed.");
                std::string tempFilePath  = std::tmpnam(nullptr);
                _bstr_t bstrPath = tempFilePath.c_str();
                if (pArtwork->SaveArtworkToFile(bstrPath) != S_OK) std::exception("IITArtwork SaveArtworkToFile failed.");
                result.artworkDataBytes = util::readAllContentsFromFile(tempFilePath);
            }
            CoUninitialize();
            return result;
        }
        catch (...) {
            CoUninitialize();
            throw;
        }
    }
}