#include <node.h>
#include <exception>
#include <string>
#include <codecvt>
#include "iTunesCOMInterface.h"

namespace local_itunes_win32
{
    using node::AtExit;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;

    static const char* wstringToChar(const std::wstring& wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
        return cvt.to_bytes(wstr).c_str();
    }

    static const char* getItunesCurrentTrack()
    {
        IiTunes* itunes = nullptr;
        IITTrack* track = nullptr;
        BSTR trackNameBuf = nullptr;

        try {
            auto res = CoCreateInstance(CLSID_iTunesApp, nullptr, CLSCTX_LOCAL_SERVER, IID_IiTunes, reinterpret_cast<void**>(&itunes));
            if (res != S_OK) {
                throw new std::exception("CoCreateInstance failed.");
            }

            itunes->get_CurrentTrack(&track);
            if (track == nullptr) {
                throw new std::exception("get_CurrentTrack failed.");
            }

            {
                track->get_Name(&trackNameBuf);
                if (trackNameBuf == nullptr) {
                    throw new std::exception("track->get_Name failed.");
                }

                std::wstring ws(trackNameBuf, SysStringLen(trackNameBuf));
                const char* result = wstringToChar(ws);

                SysFreeString(trackNameBuf);
                trackNameBuf = nullptr;
                track->Release();
                itunes->Release();

                return result;
            }
        }
        catch (...) {
            if (itunes != nullptr) itunes->Release();
            if (track != nullptr) track->Release();
            if (trackNameBuf != nullptr) SysFreeString(trackNameBuf);
            throw;
        }
    }

    static void getCurrentTrack(const FunctionCallbackInfo<Value>& args)
    {
        Isolate* isolate = args.GetIsolate();
        args.GetReturnValue().Set(String::NewFromUtf8(isolate, getItunesCurrentTrack()));
    }

    static void onExit(void*)
    {
        CoUninitialize();
    }

    void init(Local<Object> exports) {
        CoInitialize(nullptr);
        NODE_SET_METHOD(exports, "getCurrentTrack", getCurrentTrack);
        AtExit(onExit);
    }

    NODE_MODULE(addon, init)
}