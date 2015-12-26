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
    const long MaxArtworkBytesSize = 10 * 1024 * 1024; // 10MB
    struct Track 
    {
        std::string name;
        std::string artist;
        std::string artworkDataBytes;
    };

    enum class ArtworkFormat
    {
        UNKNOWN,
        JPEG,
        PNG,
        BMP,
    };

    // export functions
    const bool iTunesProcessExists();
    const Track getCurrentTrack();
}