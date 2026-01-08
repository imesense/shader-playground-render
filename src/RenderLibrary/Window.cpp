#include "StdAfx.h"

#include <string>
#include <list>

#include "InputCodes.hpp"
#include "InputManager.hpp"
#include "Window.hpp"

using namespace ShaderPlayground;

Window* Window::_wndthis = nullptr;

Window::Window(void) :
    _inputmgr(nullptr),
    _hwnd(0),
    _isexit(false),
    _active(true),
    _minimized(false),
    _maximized(false),
    _isresize(false), 
    width(640),
    height(480),
    posx(200),
    posy(20),
    resizing(true)
{
    if (!_wndthis)
    {
        _wndthis = this;
    }
    else
    {
        //Log::Get()->Err("Window уже был создан");
    }
}

bool Window::Create()
{
    //Log::Get()->Debug("%s", __FUNCTION__);

    //_desc = this;

    WNDCLASSEXW wnd{};

    wnd.cbSize = sizeof(WNDCLASSEXW);
    wnd.style = CS_HREDRAW | CS_VREDRAW;
    wnd.lpfnWndProc = StaticWndProc;
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hInstance = 0;
    wnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wnd.hIconSm = wnd.hIcon;
    wnd.hCursor = LoadCursor(0, IDC_ARROW);
    wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wnd.lpszMenuName = NULL;
    wnd.lpszClassName = L"D3D11F";
    wnd.cbSize = sizeof(WNDCLASSEX);

    if (!RegisterClassEx(&wnd))
    {
        //Log::Get()->Err("Не удалось зарегистрировать окно");
        return false;
    }

    RECT rect = { 0, 0, width, height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

    long lwidth = rect.right - rect.left;
    long lheight = rect.bottom - rect.top;

    long lleft = (long)posx;
    long ltop = (long)posy;

    _hwnd = CreateWindowEx(
        NULL,
        wnd.lpszClassName,
        L"DX11 Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        lleft, 
        ltop, 
        lwidth, 
        lheight, 
        NULL, 
        NULL, 
        NULL, 
        NULL
    );

    if (!_hwnd)
    {
        //Log::Get()->Err("Не удалось создать окно");
        return false;
    }

    ShowWindow(_hwnd, SW_SHOW);
    UpdateWindow(_hwnd);

    return true;
}

void Window::RunEvent()
{
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::Close()
{
    if (_hwnd)
    {
        DestroyWindow(_hwnd);
    }

    _hwnd = nullptr;
}

LRESULT Window::WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    switch (nMsg)
    {
    case WM_CREATE:
        return 0;

    case WM_CLOSE:
        _isexit = true;
        return 0;

    case WM_ACTIVATE:
        if (LOWORD(wParam) != WA_INACTIVE)
        {
            _active = true;
        }
        else
        {
            _active = false;
        }
        return 0;

    case WM_MOVE:
        posx = LOWORD(lParam);
        posy = HIWORD(lParam);
        UpdateWindowState();
        return 0;

    case WM_SIZE:
        if (!resizing)
        {
            return 0;
        }
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        _isresize = true;
        if (wParam == SIZE_MINIMIZED)
        {
            _active = false;
            _minimized = true;
            _maximized = false;
        }
        else if (wParam == SIZE_MAXIMIZED)
        {
            _active = true;
            _minimized = false;
            _maximized = true;
        }
        else if (wParam == SIZE_RESTORED)
        {
            if (_minimized)
            {
                _active = true;
                _minimized = false;
            }
            else if (_maximized)
            {
                _active = true;
                _maximized = false;
            }
        }
        UpdateWindowState();
        return 0;

    case WM_MOUSEMOVE:
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_MOUSEWHEEL:
    case WM_KEYDOWN:
    case WM_KEYUP:
        if (_inputmgr)
        {
            _inputmgr->Run(nMsg, wParam, lParam);
        }
        return 0;
    }

    return DefWindowProcW(hwnd, nMsg, wParam, lParam);
}

void Window::SetInputMgr(InputManager* inputmgr)
{
    _inputmgr = inputmgr;
    UpdateWindowState();
}

void Window::UpdateWindowState()
{
    RECT clientRect{};
    clientRect.left = posx;
    clientRect.top = posy;
    clientRect.right = width;
    clientRect.bottom = height;
    if (_inputmgr)
    {
        _inputmgr->SetWinRect(clientRect);
    }
}

LRESULT CALLBACK ShaderPlayground::StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    return Window::Get()->WndProc(hwnd, nMsg, wParam, lParam);
}
