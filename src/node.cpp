#include <nan.h>
#include "itunes.h"

class GetNowplayingAsyncWorker : public Nan::AsyncWorker
{
public:
    GetNowplayingAsyncWorker(Nan::Callback* callback)
        : Nan::AsyncWorker(callback)
    {}

    void Execute()
    {
        try {
            if (!itunes_win::iTunesProcessExists()) {
                throw std::exception("iTunes process not found.");
            }
            itunes_win::getNowplaying(nowplayingTrack);
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
                Nan::New(nowplayingTrack.name).ToLocalChecked());
            obj->Set(Nan::New("artist").ToLocalChecked(),
                Nan::New(nowplayingTrack.artist).ToLocalChecked());
            obj->Set(Nan::New("artworkCount").ToLocalChecked(),
				Nan::New(nowplayingTrack.artworkCount));
			obj->Set(Nan::New("artworkFormat").ToLocalChecked(),
				Nan::New(nowplayingTrack.artworkFormat).ToLocalChecked());

            callbackArgs[1] = obj;
        }
        callback->Call(2, callbackArgs);
    }
private:
    itunes_win::Track nowplayingTrack;
    std::string error_message;
};

class SaveNowplayingArtworkToFileAsyncWorker : public Nan::AsyncWorker
{
public:
	SaveNowplayingArtworkToFileAsyncWorker(std::string outPath, Nan::Callback* callback)
		: outPath(outPath), Nan::AsyncWorker(callback)
	{}

	void Execute()
	{
		try {
			if (!itunes_win::iTunesProcessExists()) {
				throw std::exception("iTunes process not found.");
			}
			itunes_win::saveNowplayingArtworkToFile(outPath);
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
		};

		if (hasError) {
			auto err = Nan::New(error_message).ToLocalChecked();
			callbackArgs[0] = err;
		}
		callback->Call(1, callbackArgs);
	}
private:
	std::string outPath;
	std::string error_message;
};

NAN_METHOD(GetNowplaying)
{
    try {
        auto callback = new Nan::Callback(info[0].As<v8::Function>());
        Nan::AsyncQueueWorker(new GetNowplayingAsyncWorker(callback));
    }
    catch (const std::exception& ex) {
        Nan::ThrowError(ex.what());
    }
}

NAN_METHOD(SaveNowplayingArtworkToFile)
{
	try {
		auto outPath = std::string(*Nan::Utf8String(info[0]));
		auto callback = new Nan::Callback(info[1].As<v8::Function>());
        Nan::AsyncQueueWorker(new SaveNowplayingArtworkToFileAsyncWorker(outPath, callback));
	}
	catch (const std::exception& ex) {
		Nan::ThrowError(ex.what());
	}
}

NAN_MODULE_INIT(InitModule)
{
    Nan::Set(target, Nan::New("getNowplaying").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(GetNowplaying)).ToLocalChecked());

	Nan::Set(target, Nan::New("saveNowplayingArtworkToFile").ToLocalChecked(),
		Nan::GetFunction(Nan::New<v8::FunctionTemplate>(SaveNowplayingArtworkToFile)).ToLocalChecked());
}

NODE_MODULE(itunes_win, InitModule)