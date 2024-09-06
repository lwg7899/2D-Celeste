#pragma once
#include "SoundFmod.h"
#include <string>

enum class eLayerType
{
    None,
    BackGround,
    Player,
    Max = 16,
}; 


enum class eResourceType
{
    Texture,
    AudioClip,
    Prefab,
    End,
};

class Resource
{
private:
    std::wstring mName;

    eResourceType mType;
    std::wstring mPath;
public:
    Resource(eResourceType type) : mType(type) 
    {
    
    }
    virtual ~Resource() {}

    virtual HRESULT Load(const std::wstring& path); 
    const std::wstring& GetPath() { return mPath; }
    void SetPath(const std::wstring& path) { mPath = path; }
    void SetName(const std::wstring& name){    mName = name;   }

    std::wstring& GetName() { return mName; }
};

class AudioClip
{
private:
    FMOD::Sound* mSound;
    FMOD::Channel* mChannel;

public:
    AudioClip();
    virtual ~AudioClip();




};

