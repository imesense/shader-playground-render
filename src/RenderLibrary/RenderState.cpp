#include "StdAfx.h"

#include <d3d11.h>

#include "RenderState.hpp"
#include "Helpers.hpp"

using namespace ShaderPlayground;

RenderState::RenderState(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext)
{
    _pd3dDevice = pd3dDevice;
    _pImmediateContext = pImmediateContext;

    _pDepthEnableStencilState = nullptr;
    _pDepthDisabledStencilState = nullptr;
    _pAlphaEnableBlendingState = nullptr;
    _pAlphaDisableBlendingState = nullptr;
    _pSampleState = nullptr;

    _depthenable = false;
    _alphaenable = false;
}

bool RenderState::Init()
{
    //Log::Get()->Debug("%s", __FUNCTION__);

    if (!Createdepthstencilstate())
    {
        return false;
    }
    if (!Createblendingstate())
    {
        return false;
    }
    if (!Createsamplerstate())
    {
        return false;
    }

    TurnZBufferOn();
    TurnOnAlphaBlending();

    _pImmediateContext->PSSetSamplers(0, 1, &_pSampleState);
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

bool RenderState::Createdepthstencilstate()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    HRESULT hr = _pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &_pDepthEnableStencilState);
    if (FAILED(hr))
    {
        return false;
    }

    depthStencilDesc.DepthEnable = false;
    hr = _pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &_pDepthDisabledStencilState);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool RenderState::Createblendingstate()
{
    D3D11_BLEND_DESC blendStateDescription;
    ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
    blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
    blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

    if (FAILED(_pd3dDevice->CreateBlendState(&blendStateDescription, &_pAlphaEnableBlendingState)))
    {
        return false;
    }

    blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
    if (FAILED(_pd3dDevice->CreateBlendState(&blendStateDescription, &_pAlphaDisableBlendingState)))
    {
        return false;
    }

    return true;
}

bool RenderState::Createsamplerstate()
{
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

    if (FAILED(_pd3dDevice->CreateSamplerState(&samplerDesc, &_pSampleState)))
    {
        return false;
    }

    return true;
}

void RenderState::Close()
{
    _RELEASE(_pAlphaEnableBlendingState);
    _RELEASE(_pAlphaDisableBlendingState);
    _RELEASE(_pDepthEnableStencilState);
    _RELEASE(_pDepthDisabledStencilState);
    _RELEASE(_pSampleState);
}

void RenderState::TurnZBufferOn()
{
    if (!_depthenable)
    {
        _pImmediateContext->OMSetDepthStencilState(_pDepthEnableStencilState, 1);
        _depthenable = true;
    }
}

void RenderState::TurnZBufferOff()
{
    if (_depthenable)
    {
        _pImmediateContext->OMSetDepthStencilState(_pDepthDisabledStencilState, 1);
        _depthenable = false;
    }
}

void RenderState::TurnOnAlphaBlending()
{
    if (!_alphaenable)
    {
        float blendFactor[4] = { 0.0f };
        _pImmediateContext->OMSetBlendState(_pAlphaEnableBlendingState, blendFactor, 0xffffffff);
        _alphaenable = true;
    }

}

void RenderState::TurnOffAlphaBlending()
{
    if (_alphaenable)
    {
        float blendFactor[4] = { 0.0f };
        _pImmediateContext->OMSetBlendState(_pAlphaDisableBlendingState, blendFactor, 0xffffffff);
        _alphaenable = false;
    }
}
