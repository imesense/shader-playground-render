#include "StdAfx.h"

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <D3DCompiler.h>
#include <D3DX11async.h>

using namespace DirectX;

#include "RenderState.hpp"
#include "Render.hpp"
#include "Shader.hpp"
#include "Helpers.hpp"

#define MAXLAYOUT 8

using namespace ShaderPlayground;

Shader::Shader(Render* render)
{
    _render = render;
    _vertexShader = nullptr;
    _pixelShader = nullptr;
    _layout = nullptr;
    _layoutformat = nullptr;
    _numlayout = 0;
}

void Shader::AddInputElementDesc(const char* SemanticName, DXGI_FORMAT format)
{
    if (!_numlayout)
    {
        _layoutformat = new D3D11_INPUT_ELEMENT_DESC[MAXLAYOUT];
        if (!_layoutformat)
        {
            return;
        }
    }
    else if (_numlayout >= MAXLAYOUT)
    {
        return;
    }

    D3D11_INPUT_ELEMENT_DESC& layout = _layoutformat[_numlayout];
    layout.SemanticName = SemanticName;
    layout.SemanticIndex = 0;
    layout.Format = format;
    layout.InputSlot = 0;
    if (!_numlayout)
    {
        layout.AlignedByteOffset = 0;
    }
    else
    {
        layout.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    }
    layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout.InstanceDataStepRate = 0;

    _numlayout++;
}

void Shader::AddInputElementDesc(const char* SemanticName, unsigned int SemanticIndex, DXGI_FORMAT format,
    unsigned int InputSlot, bool AlignedByteOffset,
    D3D11_INPUT_CLASSIFICATION InputSlotClass,
    unsigned int InstanceDataStepRate)
{
    if (!_numlayout)
    {
        _layoutformat = new D3D11_INPUT_ELEMENT_DESC[MAXLAYOUT];
        if (!_layoutformat)
        {
            return;
        }
    }
    else if (_numlayout >= MAXLAYOUT)
    {
        return;
    }

    D3D11_INPUT_ELEMENT_DESC& layout = _layoutformat[_numlayout];
    layout.SemanticName = SemanticName;
    layout.SemanticIndex = SemanticIndex;
    layout.Format = format;
    layout.InputSlot = InputSlot;
    if (!_numlayout || !AlignedByteOffset)
    {
        layout.AlignedByteOffset = 0;
    }
    else
    {
        layout.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    }
    layout.InputSlotClass = InputSlotClass;
    layout.InstanceDataStepRate = InstanceDataStepRate;

    _numlayout++;
}

bool Shader::CreateShader(const char* namevs, const char* nameps)
{
    HRESULT hr = S_OK;
    ID3DBlob* vertexShaderBuffer = nullptr;
    hr = Compileshaderfromfile(namevs, "VS", "vs_4_0", &vertexShaderBuffer);
    if (FAILED(hr))
    {
        //Log::Get()->Err("Не удалось загрузить вершинный шейдер %ls", namevs);
        return false;
    }

    ID3DBlob* pixelShaderBuffer = nullptr;
    hr = Compileshaderfromfile(nameps, "PS", "ps_4_0", &pixelShaderBuffer);
    if (FAILED(hr))
    {
        //Log::Get()->Err("Не удалось загрузить пиксельный шейдер %ls", nameps);
        return false;
    }

    hr = _render->_pd3dDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
    if (FAILED(hr))
    {
        //Log::Get()->Err("Не удалось создать вершинный шейдер");
        return false;
    }

    hr = _render->_pd3dDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
    if (FAILED(hr))
    {
        //Log::Get()->Err("Не удалось создать пиксельный шейдер");
        return false;
    }

    hr = _render->_pd3dDevice->CreateInputLayout(_layoutformat, _numlayout, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
    if (FAILED(hr))
    {
        //Log::Get()->Err("Не удалось создать формат ввода");
        return false;
    }

    _DELETE_ARRAY(_layoutformat);

    _RELEASE(vertexShaderBuffer);
    _RELEASE(pixelShaderBuffer);

    return true;
}

HRESULT Shader::Compileshaderfromfile(const char* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
    ShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DX11CompileFromFileA(FileName, NULL, NULL, EntryPoint, ShaderModel, ShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
    if (FAILED(hr) && pErrorBlob)
    {
        //Log::Get()->Err((char*)pErrorBlob->GetBufferPointer());
    }

    _RELEASE(pErrorBlob);
    return hr;
}

bool Shader::AddTexture(const wchar_t* name)
{
    ID3D11ShaderResourceView* texture = nullptr;
    HRESULT hr = D3DX11CreateShaderResourceViewFromFile(_render->_pd3dDevice, name, NULL, NULL, &texture, NULL);
    if (FAILED(hr))
    {
        //Log::Get()->Err("Не удалось загрузить текстуру %ls", name);
        return false;
    }

    _textures.push_back(texture);

    return true;
}

void Shader::Draw()
{
    _render->_pImmediateContext->IASetInputLayout(_layout);
    _render->_pImmediateContext->VSSetShader(_vertexShader, NULL, 0);
    _render->_pImmediateContext->PSSetShader(_pixelShader, NULL, 0);
    if (!_textures.empty())
    {
        _render->_pImmediateContext->PSSetShaderResources(0, static_cast<UINT>(_textures.size()), &_textures[0]);
    }
}

void Shader::Close()
{
    _RELEASE(_vertexShader);
    _RELEASE(_pixelShader);
    _RELEASE(_layout);

    for (int i = 0; i < _textures.size(); i++)
    {
        _RELEASE(_textures[i]);
    }

    _textures.clear();
}
