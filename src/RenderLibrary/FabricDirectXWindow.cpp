#include "StdAfx.h"

/* Disable rarely used windows stuff */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <list>
#include <stddef.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Memory.hpp"
#include "InputCodes.hpp"
#include "InputListener.hpp"
#include "InputBinder.hpp"
#include "RenderState.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "DepthShader.hpp"
#include "RenderTarget.hpp"
#include "Light.hpp"
#include "ShadowShader.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "DX11ViewRender.hpp"
#include "InputManager.hpp"
#include "Window.hpp"
#include "Exports.hpp"
#include "Helpers.hpp"

#include "FabricDirectXWindow.hpp"

using namespace ShaderPlayground;

struct FabricDirectXWindow
{
    MSG msg;
    HWND window;
    RECT clientRect;

    int posx;
    int posy;

    int width;
    int height;

    bool resizing;

    bool _isexit;
    bool _active;
    bool _minimized;
    bool _maximized;
    bool _isresize;

    DX11ViewRender* render;
    InputBinder* inputBinder;
}
fabric_directx_window;

void UpdateWindowState()
{
    fabric_directx_window.clientRect.left = fabric_directx_window.posx;
    fabric_directx_window.clientRect.top = fabric_directx_window.posy;
    fabric_directx_window.clientRect.right = fabric_directx_window.width;
    fabric_directx_window.clientRect.bottom = fabric_directx_window.height;
    //if (_inputmgr) {
    //    _inputmgr->SetWinRect(clientRect);
    //}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //switch (message) {
    //case WM_COMMAND:
    //    break;
    //case WM_PAINT:
    //{
    //    PAINTSTRUCT ps;
    //    HDC hdc = BeginPaint(hWnd, &ps);

    //    fabric_directx_window.render->BeginFrame();
    //    DX11ViewRender::GetDX11ViewRender()->Draw();
    ////    if (!DX11ViewRender::GetDX11ViewRender()->Draw()) {
    ////    return 0;
    ////}
    //    fabric_directx_window.render->EndFrame();

    //    EndPaint(hWnd, &ps);
    //}
    //break;
    //case WM_DESTROY:
    //    PostQuitMessage(0);
    //    break;
    //default:
    //    return DefWindowProc(hWnd, message, wParam, lParam);
    //}
    //return 0;

    switch (message)
    {
    case WM_CREATE:
        return 0;

    case WM_CLOSE:
        fabric_directx_window._isexit = true;
        return 0;

    /*
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        fabric_directx_window.render->BeginFrame();
        if (!fabric_directx_window.render->Draw())
            return 0;
        fabric_directx_window.render->EndFrame();

        EndPaint(hWnd, &ps);
    }
    break;
    */

    case WM_ACTIVATE:
        /*
        if (LOWORD(wparam) != WA_INACTIVE)
        {
            objControl._active = true;
        }
        else
        {
            objControl._active = false;
        }
        */
        //fabric_directx_window.render->BeginFrame();
        //if (!fabric_directx_window.render->Draw()) {
        //    return 0;
        //}
        //fabric_directx_window.render->EndFrame();
        return 0;

    case WM_MOVE:
        fabric_directx_window.posx = LOWORD(lParam);
        fabric_directx_window.posy = HIWORD(lParam);
        UpdateWindowState();
        return 0;

    case WM_SIZE:
        if (!fabric_directx_window.resizing)
        {
            return 0;
        }
        fabric_directx_window.width = LOWORD(lParam);
        fabric_directx_window.height = HIWORD(lParam);
        fabric_directx_window._isresize = true;
        if (wParam == SIZE_MINIMIZED)
        {
            fabric_directx_window._active = false;
            fabric_directx_window._minimized = true;
            fabric_directx_window._maximized = false;
        }
        else if (wParam == SIZE_MAXIMIZED)
        {
            fabric_directx_window._active = true;
            fabric_directx_window._minimized = false;
            fabric_directx_window._maximized = true;
        }
        else if (wParam == SIZE_RESTORED)
        {
            if (fabric_directx_window._minimized)
            {
                fabric_directx_window._active = true;
                fabric_directx_window._minimized = false;
            }
            else if (fabric_directx_window._maximized)
            {
                fabric_directx_window._active = true;
                fabric_directx_window._maximized = false;
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
        //Log::Get()->Print("WM_KEYUP");
        /*if (_inputmgr) {
            _inputmgr->Run(msg, wParam, lParam);
        }*/
        return 0;
    }

    //Log::Get()->Print("ehehehehe");

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

//void RunEvent()
//{
//    while (PeekMessage(&fabric_directx_window.msg, 0, 0, 0, PM_REMOVE))
//    {
//        TranslateMessage(&fabric_directx_window.msg);
//        DispatchMessage(&fabric_directx_window.msg);
//        //Log::Get()->Print("Active3");
//    }
//    //Log::Get()->Print("Active5");
//}

RENDERLIBRARY_API HWND CreateFabricDirectXWindow()
{
    const wchar_t CLASS_NAME[] = L"DirectX11";

    fabric_directx_window.render = new DX11ViewRender();
    fabric_directx_window.inputBinder = new InputBinder(fabric_directx_window.render);

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = 0;
    wc.hbrBackground = CreateSolidBrush(RGB(0, 156, 65));
    wc.lpszClassName = CLASS_NAME;

    RegisterClassExW(&wc);

    // Create the window.
    //RECT rect{};

    //const int top = rect.top;
    //const int left = rect.left;
    //const int init_width = rect.right - left;
    //const int init_height = rect.bottom - top;

    fabric_directx_window.window = CreateWindowW(
        CLASS_NAME, // Window class
        L"DirectX11 Window",     // Window text

        // Window style
        WS_POPUP | WS_VISIBLE | WS_SYSMENU,

        // Size and position
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,

        nullptr, // Parent window
        nullptr, // Menu
        nullptr, // Instance handle
        nullptr  // Additional application data
    );
    //fabric_directx_window.window = CreateWindowW(
    //    0,						// extended style
    //    wc.lpszClassName,		// window class name
    //    "DirectX11 Window",				// instance title
    //    WS_CHILD,				// window style
    //    left,					// initial window left position
    //    top,					// initial window top position
    //    init_width,				// initial window width
    //    init_height,			// initial window height
    //    NULL,					// handle to parent window
    //    NULL,					// handle to menu
    //    NULL,					// instance of this application
    //    NULL					// extra creation parameters
    //);
    //fabric_directx_window._render = fabric_directx_window.render;

    //if (!fabric_directx_window.render->CreateDevice(fabric_directx_window.window)) {
       // Log::Get()->Err("Не удалось создать рендер");
    //    return NULL;
    //}
    //else
    //{
    //   // Log::Get()->Err("DirectX11 создан");
    //}

    ShowWindow(fabric_directx_window.window, SW_SHOWNORMAL);
    //if (!fabric_directx_window.render->CreateDevice(fabric_directx_window.window))
    //{
    //    return NULL;
   // }
    UpdateWindow(fabric_directx_window.window);

    return fabric_directx_window.window;
}

RENDERLIBRARY_API void CreateScene(HWND ptr)
{
    fabric_directx_window.render->CreateDevice(ptr);
}

RENDERLIBRARY_API void MessageLoop()
{
    /*while (GetMessage(&fabric_directx_window.msg, 0, 0, 0))
    {
        TranslateMessage(&fabric_directx_window.msg);
        DispatchMessage(&fabric_directx_window.msg);
    }*/
    while (PeekMessage(&fabric_directx_window.msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&fabric_directx_window.msg);
        DispatchMessage(&fabric_directx_window.msg);
       // Log::Get()->Print("Active3");
    }
    //Log::Get()->Print("Active5");
}

RENDERLIBRARY_API void DestroyFabricDirectXWindow()
{
    delete fabric_directx_window.window;
}

RENDERLIBRARY_API bool Frame()
{
    MessageLoop();
     //обрабатываем события окна
    //_wnd->RunEvent();
    //Log::Get()->Print("- %i", __LINE__);
    // если окно неактивно - завершаем кадр
    /*if (!objControl._active) {
        return true;
    }*/
   // Log::Get()->Print("- %i", __LINE__);
    // если окно было закрыто, завершаем работу движка
    if (fabric_directx_window._isexit)
    {
        return false;
    }
    //Log::Get()->Print("- %i", __LINE__);
    // если окно изменило размер
    if (fabric_directx_window.resizing)
    {
    }
    //Log::Get()->Print("- %i", __LINE__);
    fabric_directx_window.render->BeginFrame();
    if (!fabric_directx_window.render->Draw())
    {
        return false;
    }
    fabric_directx_window.render->EndFrame();

    //Log::Get()->Print("Active0");
   // Log::Get()->Print("- %i", __LINE__);
    return true;
}
