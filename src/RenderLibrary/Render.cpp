#include "StdAfx.h"

#include <filesystem>
#include <list>
#include <stddef.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "Memory.hpp"
#include "RenderState.hpp"
#include "Render.hpp"
#include "DepthShader.hpp"
#include "RenderTarget.hpp"
#include "Timer.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "ShadowShader.hpp"
#include "DX11ViewRender.hpp"
#include "Buffer.hpp"
#include "Helpers.hpp"
#include "Utils.hpp"
#include "DX11ViewRender.hpp"

using namespace ShaderPlayground;

Render::Render()
{
    _pd3dDevice = nullptr;
    _pImmediateContext = nullptr;
    _pSwapChain = nullptr;
    _pRenderTargetView = nullptr;
    _pDepthStencilView = nullptr;
    _renderstate = nullptr;
}

Render::~Render()
{
}

void Render::Resize()
{
    //RECT rc;
    //GetClientRect(_hwnd, &rc);
    //_width = rc.right - rc.left;
    //_height = rc.bottom - rc.top;
}

bool Render::CreateDevice(HWND hwnd)
{
    _hwnd = hwnd;

    //Log::Get()->Debug("%s", __FUNCTION__);

    //Resize();

    if (!Createdevice())
    {
        //Log::Get()->Err("Не удалось создать DirectX Device");
        return false;
    }

    if (!Createdepthstencil())
    {
        //Log::Get()->Err("Не удалось создать буфер глубины");
        return false;
    }

    _renderstate = new RenderState(_pd3dDevice, _pImmediateContext);
    if (!_renderstate->Init())
    {
        //Log::Get()->Err("Не удалось создать render state");
        return false;
    }

    _pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);

    D3D11_VIEWPORT vp{};
    vp.Width = (FLOAT)_width;
    vp.Height = (FLOAT)_height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

    Initmatrix();

    DX11ViewRender::GetDX11ViewRender()->Init();

    return true;
}

bool Render::Createdevice()
{
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _width;
    sd.BufferDesc.Height = _height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd,
        &_pSwapChain, &_pd3dDevice, NULL, &_pImmediateContext);
    if (FAILED(hr))
    {
        return false;
    }

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRenderTargetView);
    _RELEASE(pBackBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool Render::Createdepthstencil()
{
    ID3D11Texture2D* pDepthStencil = nullptr;
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = _width;
    descDepth.Height = _height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    HRESULT hr = _pd3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
    if (FAILED(hr))
    {
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = _pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &_pDepthStencilView);
    if (FAILED(hr))
    {
        return false;
    }

    _RELEASE(pDepthStencil);

    return true;
}

void Render::Initmatrix()
{
    float aspect = (float) _width/ (float) _height;
    _projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, aspect, 1.0f, 1000.0f);
    _ortho = XMMatrixOrthographicLH((float) _width, (float) _height, 0.0f, 1.0f);
}

void Render::BeginFrame()
{
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
    _pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Render::EndFrame()
{
    _pSwapChain->Present(this->UVsyncFlag, 0);
}

void Render::Shutdown()
{
    DX11ViewRender::GetDX11ViewRender()->Close();

    if (_pImmediateContext)
    {
        _pImmediateContext->ClearState();
    }

    _CLOSE(_renderstate);
    _RELEASE(_pDepthStencilView);
    _RELEASE(_pRenderTargetView);
    _RELEASE(_pSwapChain);
    _RELEASE(_pImmediateContext);
    _RELEASE(_pd3dDevice);
}

void Render::TurnZBufferOn()
{
    _renderstate->TurnZBufferOn();
}

void Render::TurnZBufferOff()
{
    _renderstate->TurnZBufferOff();
}

void Render::TurnOnAlphaBlending()
{
    _renderstate->TurnOnAlphaBlending();
}

void Render::TurnOffAlphaBlending()
{
    _renderstate->TurnOffAlphaBlending();
}
