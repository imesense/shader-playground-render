#include "StdAfx.h"

#include <filesystem>
#include <unordered_set>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Memory.hpp"
#include "RenderState.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "Light.hpp"
#include "DepthShader.hpp"
#include "RenderTarget.hpp"
#include "ShadowShader.hpp"
#include "Shader.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "DX11ViewRender.hpp"
#include "Buffer.hpp"
#include "Light.hpp"
#include "Helpers.hpp"
#include "Utils.hpp"

using namespace ShaderPlayground;

struct MatrixBufferType
{
    XMMATRIX world;
    XMMATRIX WVP;
    XMMATRIX wvplight;
};

struct LightBufferType
{
    XMFLOAT4 ambientColor;
    XMFLOAT4 diffuseColor;
};

struct LightBufferType2
{
    XMFLOAT3 lightPosition;
    float padding;
};

ShadowShader::ShadowShader(DX11ViewRender* render)
{
    _render = render;
    _shader = nullptr;
    _sampleStateWrap = nullptr;
    _sampleStateClamp = nullptr;
    _matrixBuffer = nullptr;
    _lightBuffer = nullptr;
    _lightBuffer2 = nullptr;
}

bool ShadowShader::Init()
{
    //Log::Get()->Debug("%s", __FUNCTION__);

    // инициализируем шейдер и входной формат
    _shader = new Shader(_render);
    _shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
    _shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
    _shader->AddInputElementDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);

    const char* vertexShader = "shadow.vs";
    const char* pixelShader = "shadow.ps";

    std::string vsPath = CombinedPathForResources(vertexShader);
    std::string psPath = CombinedPathForResources(pixelShader);

    if (!_shader->CreateShader(vsPath.c_str(), psPath.c_str()))
    {
        return false;
    }

    // Создаем sampler state для того чтобы 
    // установить режим адресации текстуры как WRAP
    // wrap (обертывание) - текстура будет повторяться 
    // если координаты будут выходить за пределы 0-1.
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    if (FAILED(_render->_pd3dDevice->CreateSamplerState(&samplerDesc, &_sampleStateWrap)))
    {
        return false;
    }

    // Создаем sampler state для того чтобы 
    // установить режим адресации текстуры как CLAMP
    // clamp (одиночное наложение) - текстура будет 
    // выводиться только в пределах 0-1..
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    if (FAILED(_render->_pd3dDevice->CreateSamplerState(&samplerDesc, &_sampleStateClamp)))
    {
        return false;
    }

    // создаем константные буферы
    _matrixBuffer = Buffer::CreateConstantBuffer(_render->_pd3dDevice, sizeof(MatrixBufferType), false);
    _lightBuffer = Buffer::CreateConstantBuffer(_render->_pd3dDevice, sizeof(LightBufferType), false);
    _lightBuffer2 = Buffer::CreateConstantBuffer(_render->_pd3dDevice, sizeof(LightBufferType2), false);

    return true;
}

void ShadowShader::Render(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX WVP, CXMMATRIX WVPlight,
    ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture, Light& light)
{
    MatrixBufferType cb{};
    cb.world = XMMatrixTranspose(worldMatrix);
    cb.WVP = XMMatrixTranspose(WVP);
    cb.wvplight = XMMatrixTranspose(WVPlight);
    _render->_pImmediateContext->UpdateSubresource(_matrixBuffer, 0, NULL, &cb, 0, 0);
    _render->_pImmediateContext->VSSetConstantBuffers(0, 1, &_matrixBuffer);

    LightBufferType2 lb2{};
    lb2.lightPosition = light.GetPosition();
    lb2.padding = 0.0f;
    _render->_pImmediateContext->UpdateSubresource(_lightBuffer2, 0, NULL, &lb2, 0, 0);
    _render->_pImmediateContext->VSSetConstantBuffers(1, 1, &_lightBuffer2);

    LightBufferType lb{};
    lb.ambientColor = light.GetAmbientColor();
    lb.diffuseColor = light.GetDiffuseColor();
    _render->_pImmediateContext->UpdateSubresource(_lightBuffer, 0, NULL, &lb, 0, 0);
    _render->_pImmediateContext->PSSetConstantBuffers(0, 1, &_lightBuffer);

    // Передаем в шейдер две текстуры
    _render->_pImmediateContext->PSSetShaderResources(0, 1, &texture);
    _render->_pImmediateContext->PSSetShaderResources(1, 1, &depthMapTexture);

    // Передаем в шейдер оба sampler state
    _render->_pImmediateContext->PSSetSamplers(0, 1, &_sampleStateClamp);
    _render->_pImmediateContext->PSSetSamplers(1, 1, &_sampleStateWrap);

    _shader->Draw();
    _render->_pImmediateContext->DrawIndexed(indexCount, 0, 0);
}

void ShadowShader::Close()
{
    _CLOSE(_shader);

    _RELEASE(_sampleStateWrap);
    _RELEASE(_sampleStateClamp);
    _RELEASE(_matrixBuffer);
    _RELEASE(_lightBuffer);
    _RELEASE(_lightBuffer2);
}
