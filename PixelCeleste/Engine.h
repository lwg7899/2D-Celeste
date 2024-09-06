#pragma once
#include "WindowContainer.h"

class Engine :public WindowContainer
{
public:
    Engine() 
    {
        std::cout << "Engine 생성자 호출" << std::endl;
    }

    bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
    bool ProcessMessages();

    void Update();
    void RenderFrame();
};

