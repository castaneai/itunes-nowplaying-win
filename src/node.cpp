#include <nan.h>
#include "itunes.h"

class ITunesAsyncWorker : public Nan::AsyncWorker
{
public:
    ITunesAsyncWorker(Nan::Callback* callback)
        : Nan::AsyncWorker(callback)
    {}

    void Execute()
    {
        try {
            if (!itunes_win::iTunesProcessExists()) {
                throw std::exception("iTunes process not found.");
            }
            currentTrack = itunes_win::getCurrentTrack();
        }
        catch (const std::exception& ex) {
            error_message = ex.what();
        }
    }

    void HandleOKCallback()
    {
        bool hasError = !error_message.empty();
        v8::Local<v8::Value> callbackArgs[] = {
            Nan::Null(),
            Nan::Null(),
        };

        if (hasError) {
            auto err = Nan::New(error_message).ToLocalChecked();
            callbackArgs[0] = err;
        }
        else {
            auto obj = Nan::New<v8::Object>();
            obj->Set(Nan::New("name").ToLocalChecked(),
                Nan::New(currentTrack.name).ToLocalChecked());
            obj->Set(Nan::New("artist").ToLocalChecked(),
                Nan::New(currentTrack.artist).ToLocalChecked());
            obj->Set(Nan::New("artwork").ToLocalChecked(),
                Nan::NewBuffer(
                    const_cast<char*>(currentTrack.artworkDataBytes.data()),
                    currentTrack.artworkDataBytes.size()
                    ).ToLocalChecked());
            callbackArgs[1] = obj;
        }
        callback->Call(2, callbackArgs);
    }
private:
    itunes_win::Track currentTrack;
    std::string error_message;
};

NAN_METHOD(GetCurrentTrack)
{
    try {
        auto callback = new Nan::Callback(info[0].As<v8::Function>());
        Nan::AsyncQueueWorker(new ITunesAsyncWorker(callback));
    }
    catch (const std::exception& ex) {
        Nan::ThrowError(ex.what());
    }
}

NAN_MODULE_INIT(init)
{
    Nan::Set(target, Nan::New("getCurrentTrack").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(GetCurrentTrack)).ToLocalChecked());
}

NODE_MODULE(itunes_win, init)