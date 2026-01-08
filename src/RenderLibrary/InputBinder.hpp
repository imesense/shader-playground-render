#pragma once

namespace ShaderPlayground
{
    class DX11ViewRender;

    class InputBinder : public InputListener, public Allocator
    {
    private:
        DX11ViewRender* _render;

    public:
        RENDERLIBRARY_API InputBinder(DX11ViewRender* render);

        RENDERLIBRARY_API bool KeyPressed(const KeyEvent& arg);
        RENDERLIBRARY_API bool KeyReleased(const KeyEvent& arg);
    };
}
