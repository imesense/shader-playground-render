#pragma once

namespace ShaderPlayground
{
    class InputListener;

    class InputManager
    {
    private:
        std::list<InputListener*> _listener;

        RECT _windowrect;
        int _curx;
        int _cury;
        int _mouseWheel;

    private:
        // событие движения мыши
        void EventCursor();
        // событие кнопки мыши
        void EventMouse(const MouseKeyCodes keyCode, bool press);
        // событие вращения колесика
        void MouseWheel(short Value);
        // обработка события клавиши
        void EventKey(const KeyCodes keyCode, const wchar_t ch, bool press);

    public:
        RENDERLIBRARY_API void Initialize();
        RENDERLIBRARY_API void Close();

        RENDERLIBRARY_API void Run(const UINT& msg, WPARAM wParam, LPARAM lParam);

        RENDERLIBRARY_API void AddListener(InputListener* listener);

        // зона окна
        RENDERLIBRARY_API void SetWinRect(const RECT& winrect);
    };
}
