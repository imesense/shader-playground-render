#pragma once

namespace ShaderPlayground
{
    // События мыши
    struct MouseEvent
    {
    public:
        RENDERLIBRARY_API MouseEvent(int nx, int ny)
            : x(nx), y(ny)
        {
        }

        // Координаты мыши
        int x;
        int y;
    };

    // событие нажатия кнопки мыши
    struct MouseEventClick : public MouseEvent
    {
    public:
        RENDERLIBRARY_API MouseEventClick(MouseKeyCodes b, int nx, int ny) 
            : MouseEvent(nx, ny), btn(b)
        {
        }

        const MouseKeyCodes btn; // Клавиша
    };

    // событие прокрутки мыши
    struct MouseEventWheel : public MouseEvent
    {
    public:
        RENDERLIBRARY_API MouseEventWheel(int nwheel, int nx, int ny)
            : MouseEvent(nx, ny), wheel(nwheel)
        {
        }

        int wheel;
    };

    // событие клавиши
    struct KeyEvent
    {
    public:
        RENDERLIBRARY_API KeyEvent(wchar_t c, KeyCodes kc)
            : wc(c), code(kc)
        {
        }

        const wchar_t wc;
        const KeyCodes code;
    };

    class InputListener
    {
    public:
        // если методы возращают true - событие больше никем не обрабатывается

        // кнопка нажата
        RENDERLIBRARY_API virtual bool MousePressed(const MouseEventClick& arg)
        {
            return false;
        }
        // кнопка отпущена
        RENDERLIBRARY_API virtual bool MouseReleased(const MouseEventClick& arg)
        {
            return false;
        }
        // вращение колесика
        RENDERLIBRARY_API virtual bool MouseWheel(const MouseEventWheel& arg)
        {
            return false;
        }
        // движение мыши
        RENDERLIBRARY_API virtual bool MouseMove(const MouseEvent& arg)
        {
            return false;
        }

        // кнопка нажата
        RENDERLIBRARY_API virtual bool KeyPressed(const KeyEvent& arg)
        {
            return false;
        }
        // кнопка отпущена
        RENDERLIBRARY_API virtual bool KeyReleased(const KeyEvent& arg)
        {
            return false;
        }
    };
}
