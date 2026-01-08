#include "StdAfx.h"

#include <filesystem>
#include <unordered_set>
#include <vector>
#include <d3d11.h>
#include <DirectXmath.h>

using namespace DirectX;

#include "Memory.hpp"
#include "RenderState.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "RenderTarget.hpp"
#include "DepthShader.hpp"
#include "Buffer.hpp"
#include "Helpers.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "ShadowShader.hpp"
#include "DX11ViewRender.hpp"
#include "Utils.hpp"

using namespace ShaderPlayground;

struct MatrixBufferType
{
    XMMATRIX WVP;
};

DepthShader::DepthShader(DX11ViewRender* render)
{
    _shader = nullptr;
    _matrixBuffer = nullptr;
    _render = render;
}

bool DepthShader::Init()
{
    //Log::Get()->Debug("%s", __FUNCTION__);

    // Инициализируем шейдер и входной формат
    _shader = new Shader(_render);
    _shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);

    const char* vertexShader = "depth.vs";
    const char* pixelShader = "depth.ps";

    std::string vsPath = CombinedPathForResources(vertexShader);
    std::string psPath = CombinedPathForResources(pixelShader);
    if (!_shader->CreateShader(vsPath.c_str(), psPath.c_str()))
    {
        return false;
    }

    // Создаем константный буфер
    _matrixBuffer = Buffer::CreateConstantBuffer(_render->_pd3dDevice, sizeof(MatrixBufferType), false);

    return true;
}

void DepthShader::Render(int index, CXMMATRIX wvp)
{
    MatrixBufferType cb{};
    cb.WVP = XMMatrixTranspose(wvp);
    _render->_pImmediateContext->UpdateSubresource(_matrixBuffer, 0, NULL, &cb, 0, 0);
    _render->_pImmediateContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);

    _shader->Draw();
    _render->_pImmediateContext->DrawIndexed(index, 0, 0);
}

void DepthShader::Close()
{
    _CLOSE(_shader);
    _RELEASE(_matrixBuffer);
}
