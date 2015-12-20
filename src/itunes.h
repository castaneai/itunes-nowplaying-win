#pragma once
#include <comdef.h>
#include <string>
#include <memory>

struct com_deleter
{
    void operator()(IUnknown* i)
    {
        if (i != nullptr) {
            i->Release();
        }
    }
};

template <typename T>
using com_unique_ptr = std::unique_ptr<T, com_deleter>;

namespace itunes_win
{
    // lifecycle
    void setup();
    void tear_down();

    // export functions
    // TODO:
    const std::string get_current_track_name();
}