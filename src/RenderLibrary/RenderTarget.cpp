#include "StdAfx.h"

#include <unordered_set>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Memory.hpp"
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
#include "Helpers.hpp"

using namespace ShaderPlayground;

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;

RenderTarget::RenderTarget(DX11ViewRender* render)
{
    _RTTexture = nullptr;
    _RTV = nullptr;
    _SRV = nullptr;
    _DSTexture = nullptr;
    _DSV = nullptr;
    _render = render;
}

bool RenderTarget::Init(float screenNear, float screenDepth)
{
    //Log::Get()->Debug("%s", __FUNCTION__);

    // Сначала мы создаем текстуру в которую будем выводить shadow map

    // создаем текстуру размером SHADOWMAP_WIDTH х SHADOWMAP_HEIGHT
    // данная текстура будет использоваться как render target (установили
    // флаг - D3D11_BIND_RENDER_TARGET)
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = SHADOWMAP_WIDTH;
    textureDesc.Height = SHADOWMAP_HEIGHT;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    if (FAILED(_render->_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &_RTTexture)))
    {
        return false;
    }

    // Создаем render target view
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;
    if (FAILED(_render->_pd3dDevice->CreateRenderTargetView(_RTTexture, &renderTargetViewDesc, &_RTV)))
    {
        return false;
    }

    // создаем shader resource view c ранее созданной текстуры
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;
    if (FAILED(_render->_pd3dDevice->CreateShaderResourceView(_RTTexture, &shaderResourceViewDesc, &_SRV)))
    {
        return false;
    }

    // Создаем Depth Stencil View

    // Создаем текстуру глубины
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Width = SHADOWMAP_WIDTH;
    depthBufferDesc.Height = SHADOWMAP_HEIGHT;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;
    if (FAILED(_render->_pd3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &_DSTexture)))
    {
        return false;
    }

    // создаем depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format = depthBufferDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    if (FAILED(_render->_pd3dDevice->CreateDepthStencilView(_DSTexture, &depthStencilViewDesc, &_DSV)))
    {
        return false;
    }

    // Задаем вьюпорт по размеру нашей текстуры
    _viewport.Width = (float) SHADOWMAP_WIDTH;
    _viewport.Height = (float) SHADOWMAP_HEIGHT;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0.0f;
    _viewport.TopLeftY = 0.0f;

    // Создаем проекционную матрицу
    _projectionMatrix = XMMatrixPerspectiveFovLH(((float) XM_PI / 4.0f), ((float) SHADOWMAP_WIDTH / (float) SHADOWMAP_HEIGHT), screenNear, screenDepth);

    return true;
}

void RenderTarget::Close()
{
    _RELEASE(_DSV);
    _RELEASE(_DSTexture);
    _RELEASE(_SRV);
    _RELEASE(_RTV);
    _RELEASE(_RTTexture);
}

void RenderTarget::SetRenderTarget()
{
    // Биндим RTV и буфер глубины к конвееру
    _render->_pImmediateContext->OMSetRenderTargets(1, &_RTV, _DSV);
    // Устанавливаем вьюпорт
    _render->_pImmediateContext->RSSetViewports(1, &_viewport);
}

void RenderTarget::ClearRenderTarget(float red, float green, float blue, float alpha)
{
    float color[4] = { red, green, blue, alpha };
    _render->_pImmediateContext->ClearRenderTargetView(_RTV, color);
    _render->_pImmediateContext->ClearDepthStencilView(_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderTarget::GetShaderResourceView()
{
    return _SRV;
}

XMMATRIX RenderTarget::GetProjectionMatrix()
{
    return _projectionMatrix;
}
