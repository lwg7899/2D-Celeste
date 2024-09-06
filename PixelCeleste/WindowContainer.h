#pragma once
#include<iostream>
#include "RenderWindow.h"
#include "KeyboardClass.h"
#include "MouseClass.h" 
#include "Graphics.h"

class WindowContainer
{
public:  
    WindowContainer();
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    RenderWindow  render_window;
    KeyboardClass keyboard;
    MouseClass mouse;
    Graphics gfx;

};

