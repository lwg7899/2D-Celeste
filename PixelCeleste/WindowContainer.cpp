#include "WindowContainer.h"

WindowContainer::WindowContainer()
{
    std::cout << "WindowContainer ������ ȣ��" << std::endl;
    static bool raw_input_initialized = false;

    if (raw_input_initialized == false)
    {

        RAWINPUTDEVICE rid; //�����Է���ġ������ ��´�.

        //�̺�Ʈ�� ��������� ��⿡ ���� ������ ������ִ� �ڵ��̴�.
        rid.usUsagePage = 0x01; //Generic Desktop Controls(0x01)�� ����
        rid.usUsage = 0x02;     //�� �Է� ��ġ�� ��� �뵵�� �����մϴ�. ���⼭�� ���콺(0x02)�� ��Ÿ����.
        rid.dwFlags = 0;
        rid.hwndTarget = NULL;

        //RegisterRawInputDevices�� ���� �̺�Ʈ�� ������ ���� Device�� ��Ͻ����ָ� �ش� ���ν����� WM_INPUT �޼����� �����ش�.
        if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
        {
            ErrorLogger::Log(GetLastError(), "Faioled to register raw input devices");
            exit(-1);
        }

        raw_input_initialized = true;

    }
}

//lParam ��ǥ��, ���콺, Ű���忡�� wParam � Ű�� ����������
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
            //���α׷����� �ϴ� ���� 32bit �ڷ����� DWORD�� �ɰ��� �� ���� �����.
            //���콺 �޼����� lParam�������� ���콺 ��ǥ�� ��� �ִµ�, X���� ���� 16��Ʈ Y���� ���� 16��Ʈ�� �����Ѵ�.
            // �̶� ����, ���� 16��Ʈ�� �ɰ���� X�� Y���� �� �� �ִ�. 
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