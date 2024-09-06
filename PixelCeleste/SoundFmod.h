#pragma once

//TEst
#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_common.h"
#include "fmod_codec.h"

#include "fmod_studio.hpp"

class SoundFmod
{

public:
    static SoundFmod& getInstance() 
    {
        static SoundFmod instance;
        return instance;
    }

private:
    static FMOD::Studio::System* mSystem;
    static FMOD::System* mCoreSystem;

    static FMOD_RESULT result;

    FMOD::System* system;
    FMOD::Sound* backgroundMusic;
    FMOD::Channel* backgroundChannel;
    FMOD::ChannelGroup* channelGroup;

private:
    SoundFmod();
    SoundFmod(const SoundFmod& ref) {}
    SoundFmod& operator=(const SoundFmod& ref) {}
    ~SoundFmod();

public:

    static FMOD_RESULT F_CALLBACK channelGroupCallback(FMOD_CHANNELCONTROL* channelControl, FMOD_CHANNELCONTROL_TYPE controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void* commandData1, void* commandData2);

    //SoundFmod(const char* path, bool loop);

    bool init();
    bool render();

    bool PlayBackGroundMusic();
    bool StopBackGroundMusic();


};

