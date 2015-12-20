#include <nan.h>
#include "itunes.h"

void get_current_track(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    auto obj = Nan::New<v8::Object>();
    obj->Set(Nan::New("name").ToLocalChecked(), Nan::New(itunes_win::get_current_track_name()).ToLocalChecked());
    info.GetReturnValue().Set(obj);
}

void init(v8::Handle<v8::Object> exports)
{
    itunes_win::setup();
    node::AtExit([](void*) { itunes_win::tear_down(); });

    exports->Set(Nan::New("getCurrentTrack").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(get_current_track)->GetFunction());
}

NODE_MODULE(itunes_win, init)