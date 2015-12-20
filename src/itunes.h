#pragma once
#include <comip.h>
#include <comdef.h>

template <typename T>
struct com_ptr_t {
    using type = _com_ptr_t<_com_IIID<T, &__uuidof(T)>>;
};

template <typename T>
using com_ptr = typename com_ptr_t<T>::type;

namespace itunes_win
{
    // lifecycle
    void setup();
    void tear_down();

    // export functions
    // TODO:
    const std::string get_current_track_name();
}