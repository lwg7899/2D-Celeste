#include "SoundFmod.h"
#include "iostream"
#include <string>
#include <chrono>
#include <thread>

//https://gist.github.com/mitchcurtis/4a004db5df944d606190d3b0f6022716

FMOD::Studio::System* SoundFmod::mSystem = nullptr;
FMOD::System* SoundFmod::mCoreSystem = nullptr;
FMOD_RESULT SoundFmod::result = FMOD_OK; // 초기화

bool succeededOrWarn(const std::string& message, FMOD_RESULT result)
{
    if (result != FMOD_OK) {
        std::cerr << message << ": " << result << " " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    return true;
}

FMOD_RESULT F_CALLBACK SoundFmod::channelGroupCallback(
    FMOD_CHANNELCONTROL* channelControl, 
    FMOD_CHANNELCONTROL_TYPE controlType,
    FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, 
    void* commandData1, void* commandData2) 
{
    
    if (callbackType == FMOD_CHANNELCONTROL_CALLBACK_END)
    {
        // 음악이 끝나면 다시 재생합니다.
        getInstance().PlayBackGroundMusic();
    }
    std::cout << "Callback called for " << controlType << std::endl;
    return FMOD_OK;
}


bool SoundFmod::init()
{

    // Create the main system object.
    result = FMOD::System_Create(&system);
    if (!succeededOrWarn("FMOD: Failed to create system object", result))
        return false;

    // Initialize FMOD.
    result = system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (!succeededOrWarn("FMOD: Failed to initialise system object", result))
        return false;

    // Create the channel group.
    result = system->createChannelGroup("inGameSoundEffects", &channelGroup);
    if (!succeededOrWarn("FMOD: Failed to create in-game sound effects channel group", result))
        return false;

    // Create the sound.
    result = system->createSound(
     "Data/Sound/SANABI_ost.wav",
     FMOD_LOOP_NORMAL | FMOD_2D,
     nullptr,
     &backgroundMusic);
    if (!succeededOrWarn("FMOD: Failed to create Sound", result))
    {
        return false;
    }

    return true;
}

bool SoundFmod::PlayBackGroundMusic()
{
    // Play the sound.
    result = system->playSound(backgroundMusic, nullptr, false, &backgroundChannel);
    if (!succeededOrWarn("FMOD: Failed to play sound", result))
    {
        return false;
    }

    // Set a Volume.
    backgroundChannel->setVolume(0.03f);

    // Set a callback on the channel.
    result = backgroundChannel->setCallback(&channelGroupCallback);
    if (!succeededOrWarn("FMOD: Failed to set callback for sound", result))
    {
        return false;
    }

    //Assign the channel to the group.
    result = backgroundChannel->setChannelGroup(channelGroup);
    if (!succeededOrWarn("FMOD: Failed to set channel group on", result))
    {
        return false;
    }

    return true;
}

bool SoundFmod::StopBackGroundMusic()
{
    backgroundChannel->stop();
    return true;
}


bool SoundFmod::render()
{

    //bool isPlaying = false;
    //do {
    //    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    //    channel->isPlaying(&isPlaying);

    //    system->update();
    //} while (isPlaying);

    return true;
}


SoundFmod::SoundFmod()
{
    system = nullptr;
    backgroundMusic = nullptr;
    backgroundChannel = nullptr;
    channelGroup = nullptr;
}
SoundFmod::~SoundFmod()
{
    if (mCoreSystem)
    {
        // Clean up.
        backgroundMusic->release();
        channelGroup->release();
        system->release();

        mCoreSystem->close();
        mCoreSystem->release();
    }

}
