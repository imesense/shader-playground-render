#include "StdAfx.h"

#include <filesystem>
#include <unordered_set>
#include <d3d11.h>
#include <D3DX11.h>
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

using namespace ShaderPlayground;

struct Vertex
{
    Vertex(float x, float y, float z,
        float u, float v,
        float nx, float ny, float nz)
        : position(x, y, z), texture(u, v), normal(nx, ny, nz)
    {
    }

    XMFLOAT3 position;
    XMFLOAT2 texture;
    XMFLOAT3 normal;
};

DX11ViewRender* DX11ViewRender::GetInstance = NULL;

DX11ViewRender::DX11ViewRender()
{
    _key_up = false;
    _key_down = false;
    _key_left = false;
    _key_right = false;
    _key_a = false;
    _key_z = false;
    _key_s = false;
    _key_x = false;
    GetInstance = this;
}

bool DX11ViewRender::Init()
{
    //Log::Get()->Debug("%s", __FUNCTION__);

    // создаем вьюпорт окна. Вообще это неверное решение, так как данный 
    // вьюпорт мы создавали во фреймворке. Но так как там нет метода для его получения, мы создаем его еще раз
    _viewport.Width = (float) _width;
    _viewport.Height = (float) _height;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0.0f;
    _viewport.TopLeftY = 0.0f;
    _pImmediateContext->RSSetViewports(1, &_viewport);

    // настраиваем камеру
    _cam.SetPos(0.0f, 2.0f, -10.0f);

    // позиции ящиков
    _posbox1 = XMFLOAT3(-1.0f, 2.0f, 0.0f);
    _posbox2 = XMFLOAT3(2.0f, 2.0f, 0.0f);

    // настраиваем свет
    _Light.SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    _Light.SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    _Light.SetLookAt(0.0f, 0.0f, 0.0f);
    _Light.GenerateProjectionMatrix(1.0f, 100.0f);

    _RenderTexture = new RenderTarget(this);
    if (!_RenderTexture->Init(1.0f, 100.0f))
    {
        return false;
    }
    _DepthShader = new DepthShader(this);
    if (!_DepthShader->Init())
    {
        return false;
    }
    _ShadowShader = new ShadowShader(this);
    if (!_ShadowShader->Init())
    {
        return false;
    }

    // геометрия (plane)
    Vertex vert1[] =
    {
        Vertex(-5.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vertex(5.0f, 0.0f, 5.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f),
        Vertex(-5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vertex(-5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vertex(5.0f, 0.0f, 5.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vertex(5.0f, 0.0f, -5.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f)
    };
    unsigned long indices1[6] =
    {
        0,1,2,
        3,4,5
    };

    // создаем вершинный и индексный буферы
    _vb_ground = Buffer::CreateVertexBuffer(_pd3dDevice, sizeof(Vertex) * 6, false, &vert1);
    _ib_ground = Buffer::CreateIndexBuffer(_pd3dDevice, sizeof(unsigned long) * 6, false, &indices1);

    const char* test = "stone01.jpg";
    std::string Path = CombinedPathForResources(test);

    // грузим текстуру для поверхности
    D3DX11CreateShaderResourceViewFromFileA(_pd3dDevice, Path.c_str(), NULL, NULL, &_texture_ground, NULL);

    // геометрия (box)
    Vertex vert2[] =
    {
        Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f),
        Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),
        Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f),
        Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),

        Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),

        Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f),

        Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f),
        Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f),
        Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
        Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f),

        Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f),
        Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
        Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f),
        Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),

        Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),
        Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
    };

    unsigned long indices2[] =
    {
        0,  1,  2,
        0,  2,  3,

        4,  5,  6,
        4,  6,  7,

        8,  9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };

    _vb_box = Buffer::CreateVertexBuffer(_pd3dDevice, sizeof(Vertex) * 24, false, vert2);
    _ib_box = Buffer::CreateIndexBuffer(_pd3dDevice, sizeof(unsigned long) * 36, false, indices2);

    const char* test1 = "wall01.jpg";
    std::string Path1 = CombinedPathForResources(test1);

    const char* test2 = "ice.jpg";
    std::string Path2 = CombinedPathForResources(test2);

    // грузим текстуры для кубов
    D3DX11CreateShaderResourceViewFromFileA(_pd3dDevice, Path1.c_str(), NULL, NULL, &_texture_box1, NULL);
    D3DX11CreateShaderResourceViewFromFileA(_pd3dDevice, Path2.c_str(), NULL, NULL, &_texture_box2, NULL);

    return true;
}

bool DX11ViewRender::Draw()
{
    static float lightPositionX = -5.0f;
    static bool reverse = false;

    // изменяем позицию света по X
    if (!reverse)
    {
        lightPositionX += 0.002f;
    }
    else
    {
        lightPositionX -= 0.002f;
    }

    if (lightPositionX > 18.0f)
    {
        reverse = true;
    }
    else if (lightPositionX < -18.0f)
    {
        reverse = false;
    }

    _Light.SetPosition(lightPositionX, 8.0f, -5.0f);
    // Генерируем видовую матрицу для света
    _Light.GenerateViewMatrix();

    // управление камерой
    _cam.MoveForward(_key_up);
    _cam.MoveBackward(_key_down);
    _cam.RotateLeft(_key_left);
    _cam.RotateRight(_key_right);
    _cam.MoveUpward(_key_a);
    _cam.MoveDownward(_key_z);
    _cam.LookUpward(_key_s);
    _cam.LookDownward(_key_x);

    _cam.Render();

    // рендерим сцену в текстуру
    RenderSceneToTexture();

    // рендерим сцену на экран
    RenderSceneToWindow();

    return true;
}

void DX11ViewRender::RenderSceneToTexture()
{
    XMMATRIX WVP;
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;

    //Указываем что нужно рендерить в текстуру
    _RenderTexture->SetRenderTarget();
    // Очищаем ее
    _RenderTexture->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

    // Выводим первый ящик
    XMMATRIX wldMatrix = XMMatrixTranslation(_posbox1.x, _posbox1.y, _posbox1.z);
    WVP = wldMatrix * _Light.GetViewMatrix() * _Light.GetProjectionMatrix();
    _pImmediateContext->IASetVertexBuffers(0, 1, &_vb_box, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_ib_box, DXGI_FORMAT_R32_UINT, 0);
    _DepthShader->Render(36, WVP);

    // Выводим второй ящик
    wldMatrix = XMMatrixTranslation(_posbox2.x, _posbox2.y, _posbox2.z);
    WVP = wldMatrix * _Light.GetViewMatrix() * _Light.GetProjectionMatrix();
    _pImmediateContext->IASetVertexBuffers(0, 1, &_vb_box, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_ib_box, DXGI_FORMAT_R32_UINT, 0);
    _DepthShader->Render(36, WVP);

    // Выводим поверхность
    wldMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
    WVP = wldMatrix * _Light.GetViewMatrix() * _Light.GetProjectionMatrix();
    _pImmediateContext->IASetVertexBuffers(0, 1, &_vb_ground, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_ib_ground, DXGI_FORMAT_R32_UINT, 0);
    _DepthShader->Render(6, WVP);
}

void DX11ViewRender::RenderSceneToWindow()
{
    // Сбрасываем render target (теперь снова будет рисовать на экран)
    _pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
    // Сбрасываем вьюпорт
    _pImmediateContext->RSSetViewports(1, &_viewport);

    XMMATRIX camView = _cam.GetViewMatrix();
    XMMATRIX lightViewMatrix = _Light.GetViewMatrix();
    XMMATRIX lightProjectionMatrix = _Light.GetProjectionMatrix();

    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;
    XMMATRIX wvp;
    XMMATRIX wvplight;
    XMMATRIX wldMatrix;

    // Выводим первый куб
    wldMatrix = XMMatrixTranslation(_posbox1.x, _posbox1.y, _posbox1.z);
    wvp = wldMatrix * camView * _projection;
    wvplight = wldMatrix * lightViewMatrix * lightProjectionMatrix;
    _pImmediateContext->IASetVertexBuffers(0, 1, &_vb_box, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_ib_box, DXGI_FORMAT_R32_UINT, 0);
    _ShadowShader->Render(36, wldMatrix, wvp, wvplight, _texture_box1, _RenderTexture->GetShaderResourceView(), _Light);

    // Выводим второй куб
    wldMatrix = XMMatrixTranslation(_posbox2.x, _posbox2.y, _posbox2.z);
    wvp = wldMatrix * camView * _projection;
    wvplight = wldMatrix * lightViewMatrix * lightProjectionMatrix;
    _pImmediateContext->IASetVertexBuffers(0, 1, &_vb_box, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_ib_box, DXGI_FORMAT_R32_UINT, 0);
    _ShadowShader->Render(36, wldMatrix, wvp, wvplight, _texture_box2, _RenderTexture->GetShaderResourceView(), _Light);

    // выводим поверхность
    wldMatrix = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
    wvp = wldMatrix * camView * _projection;
    wvplight = wldMatrix * lightViewMatrix * lightProjectionMatrix;
    _pImmediateContext->IASetVertexBuffers(0, 1, &_vb_ground, &stride, &offset);
    _pImmediateContext->IASetIndexBuffer(_ib_ground, DXGI_FORMAT_R32_UINT, 0);
    _ShadowShader->Render(6, wldMatrix, wvp, wvplight, _texture_ground, _RenderTexture->GetShaderResourceView(), _Light);

}

void DX11ViewRender::Close()
{
    _CLOSE(_RenderTexture);
    _CLOSE(_DepthShader);
    _CLOSE(_ShadowShader);

    _RELEASE(_vb_ground);
    _RELEASE(_ib_ground);
    _RELEASE(_vb_box);
    _RELEASE(_ib_box);
    _RELEASE(_texture_ground);
    _RELEASE(_texture_box1);
    _RELEASE(_texture_box2);
}
