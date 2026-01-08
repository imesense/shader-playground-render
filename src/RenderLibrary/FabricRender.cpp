#include "StdAfx.h"

#include <list>
#include <string>
#include <unordered_set>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Memory.hpp"
#include "InputCodes.hpp"
#include "InputManager.hpp"
#include "Window.hpp"
#include "RenderState.hpp"
#include "Render.hpp"
#include "Helpers.hpp"

#include "Shader.hpp"
#include "RenderTarget.hpp"
#include "DepthShader.hpp"
#include "Buffer.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "ShadowShader.hpp"
#include "DX11ViewRender.hpp"
#include "InputListener.hpp"
#include "InputBinder.hpp"
#include "FabricRender.hpp"

using namespace ShaderPlayground;

bool CFabricRender::bFabricInit = false;

struct Fabric
{
    DX11ViewRender* render = nullptr;
    InputBinder* inputBinder = nullptr;
    Window* window = nullptr;
    InputManager* inputManager = nullptr;
}
fabric;

CFabricRender::CFabricRender()
{
    this->bFabricInit = false;
}

CFabricRender::~CFabricRender()
{
}

int CFabricRender::CreateFabricRender()
{
    //Log::Get()->Debug("%s", __FUNCTION__);

    if (!this->bFabricInit)
    {
        this->bFabricInit = true;
    }
    else
    {
        exit(NULL);
        return ErrorDescription::E_REINITIALIZING_FACTORY;
    }

    fabric.render = new DX11ViewRender();
    fabric.inputBinder = new InputBinder(fabric.render);
    fabric.window = new Window();

    fabric.inputManager = new InputManager();
    fabric.inputManager->Initialize();

    if (!fabric.window->Create())
    {
        //Log::Get()->Err("Не удалось создать окно");
        return false;
    }

    fabric.window->SetInputMgr(fabric.inputManager);

    if (!fabric.render->CreateDevice(fabric.window->GetHWND()))
    {
        //Log::Get()->Err("Не удалось создать рендер");
        return false;
    }

    fabric.inputManager->AddListener(fabric.inputBinder);

    return ErrorDescription::E_OK;
}

bool CFabricRender::RunRender()
{
    fabric.window->RunEvent();

    if (!fabric.window->IsActive())
    {
        return true;
    }

    if (fabric.window->IsExit())
    {
        return false;
    }

    if (fabric.window->IsResize())
    {
    }

    fabric.render->BeginFrame();

    if (!DX11ViewRender::GetDX11ViewRender()->Draw())
    {
        return false;
    }

    fabric.render->EndFrame();

    return true;
}

void CFabricRender::DestroyFabricRender()
{
    this->bFabricInit = false;
    fabric.render->Shutdown();

    _DELETE(fabric.render);
    _CLOSE(fabric.window);
    _CLOSE(fabric.inputManager);
}
