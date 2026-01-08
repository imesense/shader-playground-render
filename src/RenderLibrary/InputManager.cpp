#include "StdAfx.h"

#include <list>
#include <unordered_set>

#include "Memory.hpp"
#include "InputCodes.hpp"
#include "InputManager.hpp"
#include "InputListener.hpp"

using namespace ShaderPlayground;

void InputManager::Initialize()
{
    _mouseWheel = _curx = _cury = 0;

    //Log::Get()->Debug("%s", __FUNCTION__);
}

void InputManager::Close()
{
    if (!_listener.empty())
    {
        _listener.clear();
    }
}

void InputManager::SetWinRect(const RECT& winrect)
{
    _windowrect.left = winrect.left;
    _windowrect.right = winrect.right;
    _windowrect.top = winrect.top;
    _windowrect.bottom = winrect.bottom;
}

void InputManager::AddListener(InputListener* listener)
{
    _listener.push_back(listener);
}

void InputManager::Run(const UINT& msg, WPARAM wParam, LPARAM lParam)
{
    if (_listener.empty())
    {
        return;
    }

    KeyCodes KeyIndex;
    wchar_t buffer[1];
    BYTE lpKeyState[256];

    EventCursor();// событие движения мыши

    switch (msg)
    {
    case WM_KEYDOWN:
        KeyIndex = static_cast<KeyCodes>(wParam);
        GetKeyboardState(lpKeyState);
        ToUnicode(wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
        EventKey(KeyIndex, buffer[0], true);
        break;

    case WM_KEYUP:
        KeyIndex = static_cast<KeyCodes>(wParam);
        GetKeyboardState(lpKeyState);
        ToUnicode(wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
        EventKey(KeyIndex, buffer[0], false);
        break;

    case WM_LBUTTONDOWN:
        EventMouse(MOUSE_LEFT, true);
        break;

    case WM_LBUTTONUP:
        EventMouse(MOUSE_LEFT, false);
        break;

    case WM_RBUTTONDOWN:
        EventMouse(MOUSE_RIGHT, true);
        break;

    case WM_RBUTTONUP:
        EventMouse(MOUSE_RIGHT, false);
        break;

    case WM_MBUTTONDOWN:
        EventMouse(MOUSE_MIDDLE, true);
        break;

    case WM_MBUTTONUP:
        EventMouse(MOUSE_MIDDLE, false);
        break;

    case WM_MOUSEWHEEL:
        MouseWheel((short) GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
        break;
    }
}

void InputManager::EventCursor()
{
    POINT Position;
    GetCursorPos(&Position); // получаем текущую позицию курсора

    Position.x -= _windowrect.left;
    Position.y -= _windowrect.top;

    if (_curx == Position.x && _cury == Position.y)
    {
        return;
    }

    _curx = Position.x;
    _cury = Position.y;

    for (auto it = _listener.begin(); it != _listener.end(); ++it)
    {
        if (!(*it))
        {
            continue;
        }
        else if ((*it)->MouseMove(MouseEvent(_curx, _cury)) == true)
        {
            return;
        }
    }
}

void InputManager::EventMouse(const MouseKeyCodes code, bool press)
{
    for (auto it = _listener.begin(); it != _listener.end(); ++it)
    {
        if (!(*it))
        {
            continue;
        }

        // кнопка нажата
        if (press == true)
        {
            if ((*it)->MousePressed(MouseEventClick(code, _curx, _cury)) == true)
            {
                return;
            }
        }
        // кнопка отпущена
        else
        {
            if ((*it)->MouseReleased(MouseEventClick(code, _curx, _cury)) == true)
            {
                return;
            }
        }
    }
}

void InputManager::MouseWheel(short value)
{
    if (_mouseWheel == value)
    {
        return;
    }

    _mouseWheel = value;

    for (auto it = _listener.begin(); it != _listener.end(); ++it)
    {
        if (!(*it))
        {
            continue;
        }
        else if ((*it)->MouseWheel(MouseEventWheel(_mouseWheel, _curx, _cury)) == true)
        {
            return;
        }
    }
}

void InputManager::EventKey(const KeyCodes keyCode, const wchar_t ch, bool press)
{
    for (auto it = _listener.begin(); it != _listener.end(); ++it)
    {
        if (!(*it))
        {
            continue;
        }

        // кнопка нажата
        if (press == true)
        {
            if ((*it)->KeyPressed(KeyEvent(ch, keyCode)) == true)
            {
                return;
            }
        }
        // кнопка отпущена
        else
        {
            if ((*it)->KeyReleased(KeyEvent(ch, keyCode)) == true)
            {
                return;
            }
        }
    }
}
