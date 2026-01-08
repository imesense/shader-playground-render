#include "StdAfx.h"

#include <unordered_set>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Memory.hpp"
#include "InputCodes.hpp"
#include "InputListener.hpp"
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
#include "InputBinder.hpp"
#include "Helpers.hpp"

using namespace ShaderPlayground;

InputBinder::InputBinder(DX11ViewRender* render)
{
    _render = render;
}

bool InputBinder::KeyPressed(const KeyEvent& arg)
{
    if (arg.code == KEY_UP)
    {
        _render->_key_up = true;
    }
    else if (arg.code == KEY_DOWN)
    {
        _render->_key_down = true;
    }
    else if (arg.code == KEY_LEFT)
    {
        _render->_key_left = true;
    }
    else if (arg.code == KEY_RIGHT)
    {
        _render->_key_right = true;
    }
    else if (arg.code == KEY_A)
    {
        _render->_key_a = true;
    }
    else if (arg.code == KEY_Z)
    {
        _render->_key_z = true;
    }
    else if (arg.code == KEY_S)
    {
        _render->_key_s = true;
    }
    else if (arg.code == KEY_X)
    {
        _render->_key_x = true;
    }

    return true;
}

bool InputBinder::KeyReleased(const KeyEvent& arg)
{
    if (arg.code == KEY_UP)
    {
        _render->_key_up = false;
    }
    else if (arg.code == KEY_DOWN)
    {
        _render->_key_down = false;
    }
    else if (arg.code == KEY_LEFT)
    {
        _render->_key_left = false;
    }
    else if (arg.code == KEY_RIGHT)
    {
        _render->_key_right = false;
    }
    else if (arg.code == KEY_A)
    {
        _render->_key_a = false;
    }
    else if (arg.code == KEY_Z)
    {
        _render->_key_z = false;
    }
    else if (arg.code == KEY_S)
    {
        _render->_key_s = false;
    }
    else if (arg.code == KEY_X)
    {
        _render->_key_x = false;
    }

    return true;
}
