#pragma once
#include "WindowContainer.h"

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch(uMsg) 
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;

        default:
        {
            WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

}


LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM IParam)
{
    switch(message)
    {
    case WM_NCCREATE:
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(IParam);
        WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
        
        if (pWindow == nullptr) 
        {
            ErrorLogger::Log("Critical Error: Pointer to window container is null during WM_NCCREATE.");
            exit(-1);
        }

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
        //Error코드-> SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(pWindow)); 세 번째 인자는 윈도우 프로시저(Window Procedure)의 포인터여야 하기 때문이다. 

        OutputDebugStringA("The window was created\n");
        return pWindow->WindowProc(hWnd, message,wParam, IParam);
    }

    default:
        return DefWindowProc(hWnd, message, wParam, IParam);
    }

}

void RenderWindow::RegisterWindowClass()
{
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = HandleMessageSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = this->hInstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = this->window_class_wide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);

    RegisterClassEx(&wc);

};

RenderWindow::~RenderWindow() 
{
    if (this->handle!=NULL) 
    {
        UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
        DestroyWindow(handle);
    }
}

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    this->hInstance = hInstance;

    this->window_title = window_title;
    this->window_title_wide = StringConverter::StringToWide(window_title);
    
    this->window_class = window_class;
    this->window_class_wide = StringConverter::StringToWide(window_class);

    this->width = width;
    this->height = height;

    this->RegisterWindowClass();

    int centerScreenX = (GetSystemMetrics(SM_CXSCREEN)/2) - (this->width/2);
    int centerScreenY = (GetSystemMetrics(SM_CYSCREEN)/2) - (this->height/2);

    RECT wr;    //Window Rectangle
    wr.left = centerScreenX;    //Window X Position
    wr.top = centerScreenY;     //Window Y Position
    wr.right = wr.left + this->width;    //Window Width
    wr.bottom = wr.top + this->height;  //Window Height

    AdjustWindowRect(&wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    this->handle = CreateWindowEx(0,
        this->window_class_wide.c_str(),
        this->window_title_wide.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        wr.left,
        wr.top,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        this->hInstance,
        pWindowContainer);

    if (this->handle == NULL) 
    {
        ErrorLogger::Log(GetLastError(), "CreateWindowEX failed for window: " + this->window_title);
        return false;
    }

    ShowWindow(this->handle, SW_SHOW);
    SetForegroundWindow(this->handle);
    SetFocus(this->handle);

    return true;
}

bool RenderWindow::ProcessMessages()
{
    MSG msg;
    ZeroMemory(&msg,sizeof(MSG));

    while(PeekMessage(&msg, 
        this->handle,
        0,
        0,
        PM_REMOVE)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT)
    {
        if (!IsWindow(this->handle))    //윈도우 창이 파괴될 때 사용
        {
            this->handle = NULL;
            UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
            return false;
        }
    }

    return true;
}

HWND RenderWindow::GetHWND()
{
    return this->handle;
}

