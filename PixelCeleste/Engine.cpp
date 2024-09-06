#pragma once
#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
    {
        return false;
    } 

    if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
    {
        return false;
    }

    return true;
}

bool Engine::ProcessMessages()
{
    return this->render_window.ProcessMessages();
}

void Engine::Update()
{
    while (!keyboard.CharBufferIsEmpty()) 
    {
        unsigned char ch = keyboard.ReadChar();
    }

    while (!keyboard.KeyBufferIsEmpty())
    {
        KeyBoardEvent kbe = keyboard.ReadKey();
        unsigned char keycode = kbe.GetKeyCode();
    }

    while (!mouse.EventBufferIsEmpty()) 
    {
        MouseEvent mouseEvent = mouse.ReadEvent();

      /*  switch (mouseEvent.GetType())
        {
            case MouseEvent::EventType::WheelUp:
            {
                OutputDebugStringA("MouseWheelUp\n");
                break;
            }
            case MouseEvent::EventType::WheeelDown:
            {
                OutputDebugStringA("MouseWheelDown\n");
                break;
            }
            case MouseEvent::EventType::LPress:
            {
                OutputDebugStringA("LPress\n");
                break;
            }
            case MouseEvent::EventType::LRelease:
            {
                OutputDebugStringA("LRelease\n");
                break;
            }
            case MouseEvent::EventType::RPress:
            {
                OutputDebugStringA("RPress\n");
                break;
            }
            case MouseEvent::EventType::RRelease:
            {
                OutputDebugStringA("RRelease\n");
                break;
            }
            case MouseEvent::EventType::MPress:
            {
                OutputDebugStringA("MPress\n");
                break;
            }
            case MouseEvent::EventType::MRelease:
            {
                OutputDebugStringA("MRelease\n");
                break;
            }

            case MouseEvent::EventType::RAW_MOVE:
            {
                OutputDebugStringA("RAW_MOVE\n");
                std::string outmsg = "X: ";
                outmsg += std::to_string(mouseEvent.GetPosX());
                outmsg += ", Y: ";
                outmsg += std::to_string(mouseEvent.GetPosY());
                outmsg += "\n";
                OutputDebugStringA(outmsg.c_str());

                break;
            }

            case MouseEvent::EventType::Move:
            {
                OutputDebugStringA("MOVE\n");
                break;
            }
        }*/

        if (mouse.IsRightDown())
        {
            if (mouseEvent.GetType() == MouseEvent::EventType::RAW_MOVE)
            {
                this->gfx.camera.AdjustRotation((float)mouseEvent.GetPosY() * 0.01f, (float)mouseEvent.GetPosX() * 0.01f, 0);
            }
        }
    }

    float cameraSpeed = 0.02f;

    if (keyboard.KeyIsPressed('W'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * cameraSpeed);
    }
    if (keyboard.KeyIsPressed('S'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * cameraSpeed);
    }
    if (keyboard.KeyIsPressed('A'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * cameraSpeed);
    }
    if (keyboard.KeyIsPressed('D'))
    {
        this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * cameraSpeed);
    }
    if (keyboard.KeyIsPressed('Q'))
    {
        this->gfx.camera.AdjustPosition(0.0f, -cameraSpeed, 0.0f);
    }
    if (keyboard.KeyIsPressed('E'))
    {
        this->gfx.camera.AdjustPosition(0.0f, cameraSpeed, 0.0f);
    }
}

void Engine::RenderFrame()

{
    gfx.RenderFrame();
}
