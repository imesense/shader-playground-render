#include "StdAfx.h"

#include <list>
#include <string>
#include <unordered_set>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include <Memory.hpp>
#include <InputCodes.hpp>
#include <InputManager.hpp>
#include <Window.hpp>
#include <RenderState.hpp>
#include <Render.hpp>
#include <DepthShader.hpp>
#include <RenderTarget.hpp>
#include <Light.hpp>
#include <ShadowShader.hpp>
#include <Timer.hpp>
#include <Camera.hpp>
#include <DX11ViewRender.hpp>
#include <InputListener.hpp>
#include <InputBinder.hpp>

using namespace ShaderPlayground;

#include "Framework.h"
#include "Program.h"
#include "StartUp.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    Start::Launch();

    return 1;
}
