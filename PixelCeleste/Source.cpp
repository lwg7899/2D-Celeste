#pragma once
#include "Engine.h"
#include "SoundFmod.h"
//TEST 0906

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) 
    {
        ErrorLogger::Log(hr, "Fialwed to dcall CoIntitalize");
        return - 1;
    }

    SoundFmod&  sound = SoundFmod::getInstance();
    if (!sound.init()) 
    {
        std::cerr << "Failed to initialize SoundFmod" << std::endl;
        return 1;
    }
    sound.PlayBackGroundMusic();


    Engine engine;
    engine.Initialize(hInstance, "Title", "MyWindowClass", 800 ,600);

    while (engine.ProcessMessages() == true)
    {
        engine.Update();
        engine.RenderFrame();
    }

    return 0;
}