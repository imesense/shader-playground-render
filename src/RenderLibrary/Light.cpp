#include "StdAfx.h"

#include <DirectXMath.h>

using namespace DirectX;

#include "Light.hpp"

using namespace ShaderPlayground;

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
    _ambientColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
    _diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetPosition(float x, float y, float z)
{
    _position = XMFLOAT3(x, y, z);
}

void Light::SetLookAt(float x, float y, float z)
{
    _lookAt = XMFLOAT3(x, y, z);
}

XMFLOAT4 Light::GetAmbientColor()
{
    return _ambientColor;
}

XMFLOAT4 Light::GetDiffuseColor()
{
    return _diffuseColor;
}

XMFLOAT3 Light::GetPosition()
{
    return _position;
}

void Light::GenerateViewMatrix()
{
    XMVECTOR camPos = XMVectorSet(_position.x, _position.y, _position.z, 0.0f);
    XMVECTOR camLookAt = XMVectorSet(_lookAt.x, _lookAt.y, _lookAt.z, 0.0f);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    _viewMatrix = XMMatrixLookAtLH(camPos, camLookAt, up);
}

void Light::GenerateProjectionMatrix(float screenNear, float screenDepth)
{
    float fieldOfView = (float) XM_PI / 2.0f;
    float screenAspect = 1.0f;
    _projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
}

XMMATRIX Light::GetViewMatrix()
{
    return _viewMatrix;
}

XMMATRIX Light::GetProjectionMatrix()
{
    return _projectionMatrix;
}
