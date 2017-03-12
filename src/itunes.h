#pragma once
#include <comdef.h>
#include <string>
#include <memory>

struct com_deleter
{
    void operator()(IUnknown* i)
    {
        i->Release();
    }
};

template <typename T>
using com_unique_ptr = std::unique_ptr<T, com_deleter>;

namespace itunes_win
{
    struct Track 
    {
        std::string name;
        std::string artist;
		int artworkCount;
        std::string artworkFormat;
    };

    // export functions
    const bool iTunesProcessExists();
    void getNowplaying(Track& result);
	void saveNowplayingArtworkToFile(std::string outPath);
}