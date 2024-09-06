#include "Windows.h"
#include "AudioClip.h"

AudioClip::AudioClip() : mSound(nullptr), mChannel(nullptr)
{

}

AudioClip::~AudioClip()
{
}

HRESULT Resource::Load(const std::wstring& path)
{
    return E_NOTIMPL;
}
