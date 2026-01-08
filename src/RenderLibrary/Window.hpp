#pragma once

namespace ShaderPlayground
{
    class Window
    {
    private:
        static Window* _wndthis;

        InputManager* _inputmgr;
        HWND _hwnd; // дескриптор окна

        int posx;
        int posy;

        int width; // ширина клиентской части окна
        int height; // высота клиентской части окна
        bool resizing;

        bool _isexit; // флаг сообщающий о событии выхода
        bool _active; // окно активно?
        bool _minimized;
        bool _maximized;
        bool _isresize; // если окно изменило размер

    private:
        void UpdateWindowState();

    public:
        RENDERLIBRARY_API Window();

        RENDERLIBRARY_API static Window* Get()
        {
            return _wndthis;
        }

        RENDERLIBRARY_API bool Create();

        // Обработка событий окна
        RENDERLIBRARY_API void RunEvent();

        // Закрыть окно.
        RENDERLIBRARY_API void Close();

        RENDERLIBRARY_API void SetInputMgr(InputManager* inputmgr);

        RENDERLIBRARY_API HWND GetHWND() const
        {
            return _hwnd;
        }

        RENDERLIBRARY_API int GetLeft() const
        {
            return posx;
        }
        RENDERLIBRARY_API int GetTop() const
        {
            return posy;
        }
        RENDERLIBRARY_API int GetWidth() const
        {
            return width;
        }
        RENDERLIBRARY_API int GetHeight() const
        {
            return height;
        }

        // сообщает, было ли сообщение о выходе
        RENDERLIBRARY_API bool IsExit() const
        {
            return _isexit;
        }

        // сообщает об активности окна
        RENDERLIBRARY_API bool IsActive() const
        {
            return _active;
        }

        // сообщает об изменении окна
        // предупреждение: после вызова оповещает окно об обработке события
        RENDERLIBRARY_API bool IsResize()
        {
            bool ret = _isresize;
            _isresize = false;
            return ret;
        }

        // обработка событий
        RENDERLIBRARY_API LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
    };

    // обработка событий
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
}
