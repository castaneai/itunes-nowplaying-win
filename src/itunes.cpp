#include <tchar.h>
#include <string>
#include <codecvt>
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

		std::string bstrToUtf8String(const _bstr_t& bstr)
		{
			return wstringToUTF8String(std::wstring(bstr, bstr.length()));
		}

		const _bstr_t utf8StringToBstr(const std::string& str)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
			return _bstr_t(cvt.from_bytes(str).c_str());
		}

		std::string artworkFormatToString(const ITArtworkFormat& fmt)
		{
			switch (fmt) {
			case ITArtworkFormatJPEG:
				return "JPEG";

			case ITArtworkFormatPNG:
				return "PNG";

			case ITArtworkFormatBMP:
				return "BMP";

			default:
				return "UNKNOWN";
			}
		}
	}

	const bool iTunesProcessExists()
	{
		return FindWindow(_T("iTunes"), _T("iTunes")) != 0;
	}

	const int getArtworkCount(const IITArtworkCollection* const& artworks)
	{
		long artworkCount = 0;
		if (const_cast<IITArtworkCollection*&>(artworks)->get_Count(&artworkCount) != S_OK) throw std::exception("IITArtworkCollection get_Count failed.");
		return static_cast<int>(artworkCount);
	}

	const ITArtworkFormat getArtworkFormat(const IITArtwork* const& artwork)
	{
		ITArtworkFormat format;
		if (const_cast<IITArtwork*&>(artwork)->get_Format(&format) != S_OK) throw std::exception("IITArtwork get_Format failed.");
		return format;
	}

	void iTunesContext(std::function<void(const com_unique_ptr<IiTunes>&)> action)
	{
		CoInitialize(nullptr);
		try {
			com_unique_ptr<IiTunes> itunes;
			auto res = CoCreateInstance(CLSID_iTunesApp, nullptr, CLSCTX_LOCAL_SERVER, IID_IiTunes, reinterpret_cast<void**>(&itunes));
			if (res != S_OK) throw std::exception("CoCreateInstance failed.");
			action(itunes);
			// 2回め以降のCoInitializeのコストを下げるため、正常完了のときはCoUninitializeしない
		}
		catch (...) {
			CoUninitialize();
			throw;
		}
	}

	/**
	 * 現在再生中の曲情報を取得
	 */
	void getNowplaying(Track& result)
	{
		iTunesContext([&](const com_unique_ptr<IiTunes>& itunes) {
			com_unique_ptr<IITTrack> track;
			if (itunes->get_CurrentTrack(reinterpret_cast<IITTrack**>(&track)) != S_OK) throw std::exception("get_CurrentTrack failed.");

			_bstr_t trackName;
			if (track->get_Name(trackName.GetAddress()) != S_OK) throw std::exception("track->get_Name failed.");
			result.name = util::bstrToUtf8String(trackName);

			_bstr_t trackArtist;
			if (track->get_Artist(trackArtist.GetAddress()) != S_OK) throw std::exception("track->get_Artist failed.");
			result.artist = util::bstrToUtf8String(trackArtist);

			com_unique_ptr<IITArtworkCollection> artworks;
			if (track->get_Artwork(reinterpret_cast<IITArtworkCollection**>(&artworks)) != S_OK) throw std::exception("track->get_Artwork failed.");

			auto artworkCount = getArtworkCount(artworks.get());
			result.artworkCount = artworkCount;
			result.artworkFormat = "";

			if (artworkCount > 0) {
				com_unique_ptr<IITArtwork> artwork;
				if (artworks->get_Item(1, reinterpret_cast<IITArtwork**>(&artwork)) != S_OK) throw std::exception("IITArtworkCollection->get_Item failed.");
				result.artworkFormat = util::artworkFormatToString(getArtworkFormat(artwork.get()));
			}
		});
	}

	/**
	 * 現在再生中の曲のアートワーク画像を指定パスに保存
	 */
	void saveNowplayingArtworkToFile(std::string outPath)
	{
		iTunesContext([&](const com_unique_ptr<IiTunes>& itunes) {
			com_unique_ptr<IITTrack> track;
			if (itunes->get_CurrentTrack(reinterpret_cast<IITTrack**>(&track)) != S_OK) throw std::exception("get_CurrentTrack failed.");

			com_unique_ptr<IITArtworkCollection> artworks;
			if (track->get_Artwork(reinterpret_cast<IITArtworkCollection**>(&artworks)) != S_OK) throw std::exception("track->get_Artwork failed.");

			auto artworkCount = getArtworkCount(artworks.get());
			if (artworkCount < 1) throw std::exception("artwork not found");

			com_unique_ptr<IITArtwork> artwork;
			if (artworks->get_Item(1, reinterpret_cast<IITArtwork**>(&artwork)) != S_OK) throw std::exception("IITArtworkCollection->get_Item failed.");
			if (artwork->SaveArtworkToFile(util::utf8StringToBstr(outPath)) != S_OK) throw std::exception("pArtwork->SaveArtworkToFIle failed.");
		});
	}
}