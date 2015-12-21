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
    void setup();
    void tear_down();

    // export functions
    bool itunes_process_exists();
    const std::string get_current_track_name();
}