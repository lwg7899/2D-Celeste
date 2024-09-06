#include "WindowContainer.h"

WindowContainer::WindowContainer()
{
    std::cout << "WindowContainer 생성자 호출" << std::endl;
    static bool raw_input_initialized = false;

    if (raw_input_initialized == false)
    {

        RAWINPUTDEVICE rid; //원시입력장치정보를 담는다.

        //이벤트를 얻어오고싶은 기기에 대한 정보를 등록해주는 코드이다.
        rid.usUsagePage = 0x01; //Generic Desktop Controls(0x01)를 설정
        rid.usUsage = 0x02;     //시 입력 장치의 사용 용도를 설정합니다. 여기서는 마우스(0x02)를 나타낸다.
        rid.dwFlags = 0;
        rid.hwndTarget = NULL;

        //RegisterRawInputDevices를 통해 이벤트를 얻어오고 싶은 Device를 등록시켜주면 해당 프로시저에 WM_INPUT 메세지를 보내준다.
        if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
        {
            ErrorLogger::Log(GetLastError(), "Faioled to register raw input devices");
            exit(-1);
        }

        raw_input_initialized = true;

    }
}

//lParam 좌표값, 마우스, 키보드에서 wParam 어떤 키가 눌러졌는지
LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        //KeyBoard Message
        case WM_KEYDOWN:
        {
            unsigned char keycode = static_cast<unsigned char>(wParam);
            if (keyboard.IsKeysAutoRepeat()) 
            {
                keyboard.OnKeyPressed(keycode);
            }
            else 
            {
                const bool wasPressed = lParam & 0x40000000;
                if (!wasPressed)
                {
                    keyboard.OnKeyPressed(keycode);
                }
            }
            return 0;
        }

        case WM_KEYUP: 
        {
            unsigned char keycode = static_cast<unsigned char>(wParam);
            keyboard.OnKeyReleased(keycode);
            return 0;
        }

        case WM_CHAR:
        {
            unsigned char ch = static_cast<unsigned char>(wParam);
            if (keyboard.IsCharsAutoRepeat()) 
            {
                keyboard.OnChar(ch);
            }
            else 
            {
                const bool wasPressed = lParam & 0x400000000;
                if (!wasPressed) 
                {
                    keyboard.OnChar(ch);
                }
            }

            return 0;

        }

        //Mouyyse Messages
        case WM_MOUSEMOVE: 
        {
            //프로그래밍을 하다 보면 32bit 자료형인 DWORD를 쪼개야 할 때가 생긴다.
            //마우스 메세지의 lParam변수에는 마우스 좌표가 들어 있는데, X축은 하위 16비트 Y축은 상위 16비트에 존재한다.
            // 이때 상위, 하위 16비트를 쪼개어야 X축 Y축을 알 수 있다. 
            int x = LOWORD(lParam); //((((DWORD)lParam)) & 0xffff);
            int y = HIWORD(lParam); //((((DWORD)lParam >> 16)) & 0xffff);
            mouse.OnMouseMove(x,y);
            return 0;
        }

        case WM_LBUTTONDOWN: 
        {
            int x = LOWORD(lParam); //((((DWORD)lParam)) & 0xffff);
            int y = HIWORD(lParam); //((((DWORD)lParam >> 16)) & 0xffff);
            mouse.OnLeftPressed(x, y);
            return 0;
        }

        case WM_RBUTTONDOWN:
        {
            int x = LOWORD(lParam); //((((DWORD)lParam)) & 0xffff);
            int y = HIWORD(lParam); //((((DWORD)lParam >> 16)) & 0xffff);
            mouse.OnRightPressed(x, y);
            return 0;
        }

        case WM_MBUTTONDOWN:
        {
            int x = LOWORD(lParam); //((((DWORD)lParam)) & 0xffff);
            int y = HIWORD(lParam); //((((DWORD)lParam >> 16)) & 0xffff);
            mouse.OnMiddlePressed(x, y);
            return 0;
        }

        case WM_LBUTTONUP:
        {
            int x = LOWORD(lParam); //((((DWORD)lParam)) & 0xffff);
            int y = HIWORD(lParam); //((((DWORD)lParam >> 16)) & 0xffff);
            mouse.OnLeftReleased(x, y);
            return 0;
        }

        case WM_RBUTTONUP:
        {
            int x = LOWORD(lParam); //((((DWORD)lParam)) & 0xffff);
            int y = HIWORD(lParam); //((((DWORD)lParam >> 16)) & 0xffff);
            mouse.OnRightReleased(x, y);
            return 0;
        }

        case WM_MBUTTONUP:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam); //((((DWORD)lParam >> 16)) & 0xffff);
            mouse.OnMiddleReleased(x, y);
            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            int x = LOWORD(lParam);
            int y = LOWORD(lParam);
            if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) 
            {
                mouse.OnWheelUp(x, y);
            }
            else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
            {
                mouse.OnWheelDown(x, y);
            }
            break;
        }

        case WM_INPUT:
        {
            UINT dataSize;
            GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

            if (dataSize > 0)
            {
                std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
                if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
                {
                    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
                    if (raw->header.dwType == RIM_TYPEMOUSE)
                    {
                        mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                    }
                }
            }

            return DefWindowProc(hwnd, uMsg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages

        }

        default:
        {
            //OutputDebugStringA("WINDOW PROC FROM WINDOW CONTAINER\n");
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

    }

}