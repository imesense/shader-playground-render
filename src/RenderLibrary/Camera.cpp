#include "StdAfx.h"

#include <unordered_set>
#include <DirectXMath.h>

using namespace DirectX;

#include "Memory.hpp"
#include "Timer.hpp"
#include "Camera.hpp"

using namespace ShaderPlayground;

Camera::Camera()
{
    _frameTime = 0.0f;

    _leftSpeed = 0.0f;
    _rightSpeed = 0.0f;
    _forwardSpeed = 0.0f;
    _backwardSpeed = 0.0f;
    _upwardSpeed = 0.0f;
    _downwardSpeed = 0.0f;
    _leftTurnSpeed = 0.0f;
    _rightTurnSpeed = 0.0f;
    _lookUpSpeed = 0.0f;
    _lookDownSpeed = 0.0f;

    _pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    _rot = XMFLOAT3(0.0f, 0.0f, 0.0f);

    _time.Init();
}

void Camera::Render()
{
    _time.Frame();
    _frameTime = _time.GetTime();

    float radians = _rot.y * 0.0174532925f;
    float lAtx = sinf(radians) + _pos.x;
    float lAty = _pos.y;
    float lAtz = cosf(radians) + _pos.z;

    float pitch = _rot.x * 0.0174532925f;
    float yaw = _rot.y * 0.0174532925f;
    float roll = _rot.z * 0.0174532925f;

    XMMATRIX rotmat = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    XMVECTOR camPos = XMVectorSet(_pos.x, _pos.y, _pos.z, 0.0f);
    XMVECTOR camLookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    camLookAt = XMVector3TransformCoord(camLookAt, rotmat);
    camLookAt = camPos + camLookAt;

    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    Up = XMVector3TransformCoord(Up, rotmat);

    _viewMatrix = XMMatrixLookAtLH(camPos, camLookAt, Up);
}

void Camera::MoveForward(bool keydown)
{
    if (keydown)
    {
        _forwardSpeed += _frameTime * 0.001f;
        if (_forwardSpeed > (_frameTime * 0.03f))
        {
            _forwardSpeed = _frameTime * 0.03f;
        }
    }
    else
    {
        _forwardSpeed -= _frameTime * 0.0007f;
        if (_forwardSpeed < 0.0f)
        {
            _forwardSpeed = 0.0f;
        }
    }

    float radians = _rot.y * 0.0174532925f;
    _pos.x += sinf(radians) * _forwardSpeed;
    _pos.z += cosf(radians) * _forwardSpeed;
}

void Camera::MoveBackward(bool keydown)
{
    if (keydown)
    {
        _backwardSpeed += _frameTime * 0.001f;
        if (_backwardSpeed > (_frameTime * 0.03f))
        {
            _backwardSpeed = _frameTime * 0.03f;
        }
    }
    else
    {
        _backwardSpeed -= _frameTime * 0.0007f;
        if (_backwardSpeed < 0.0f)
        {
            _backwardSpeed = 0.0f;
        }
    }

    float radians = _rot.y * 0.0174532925f;
    _pos.x -= sinf(radians) * _backwardSpeed;
    _pos.z -= cosf(radians) * _backwardSpeed;
}

void Camera::MoveLeft(bool keydown)
{
    if (keydown)
    {
        _leftSpeed += _frameTime * 0.001f;
        if (_leftSpeed > (_frameTime * 0.03f))
        {
            _leftSpeed = _frameTime * 0.03f;
        }
    }
    else
    {
        _leftSpeed -= _frameTime * 0.0007f;
        if (_leftSpeed < 0.0f)
        {
            _leftSpeed = 0.0f;
        }
    }

    float rad = _rot.y * 0.0174532925f;
    _pos.x -= cosf(rad) * _leftSpeed;
    _pos.z -= sinf(rad) * _leftSpeed;
}

void Camera::MoveRight(bool keydown)
{
    if (keydown)
    {
        _rightSpeed += _frameTime * 0.001f;
        if (_rightSpeed > (_frameTime * 0.03f))
        {
            _rightSpeed = _frameTime * 0.03f;
        }
    }
    else
    {
        _rightSpeed -= _frameTime * 0.0007f;
        if (_rightSpeed < 0.0f)
        {
            _rightSpeed = 0.0f;
        }
    }

    float rad = _rot.y * 0.0174532925f;
    _pos.x += cosf(rad) * _rightSpeed;
    _pos.z += sinf(rad) * _rightSpeed;
}

void Camera::MoveUpward(bool keydown)
{
    if (keydown)
    {
        _upwardSpeed += _frameTime * 0.003f;
        if (_upwardSpeed > (_frameTime * 0.03f))
        {
            _upwardSpeed = _frameTime * 0.03f;
        }
    }
    else
    {
        _upwardSpeed -= _frameTime * 0.002f;
        if (_upwardSpeed < 0.0f)
        {
            _upwardSpeed = 0.0f;
        }
    }

    _pos.y += _upwardSpeed;
}

void Camera::MoveDownward(bool keydown)
{
    if (keydown)
    {
        _downwardSpeed += _frameTime * 0.003f;
        if (_downwardSpeed > (_frameTime * 0.03f))
        {
            _downwardSpeed = _frameTime * 0.03f;
        }
    }
    else
    {
        _downwardSpeed -= _frameTime * 0.002f;
        if (_downwardSpeed < 0.0f)
        {
            _downwardSpeed = 0.0f;
        }
    }

    _pos.y -= _downwardSpeed;
}

void Camera::RotateLeft(bool keydown)
{
    if (keydown)
    {
        _leftTurnSpeed += _frameTime * 0.01f;
        if (_leftTurnSpeed > (_frameTime * 0.15f))
        {
            _leftTurnSpeed = _frameTime * 0.15f;
        }
    }
    else
    {
        _leftTurnSpeed -= _frameTime * 0.005f;
        if (_leftTurnSpeed < 0.0f)
        {
            _leftTurnSpeed = 0.0f;
        }
    }

    _rot.y -= _leftTurnSpeed;
    if (_rot.y < 0.0f)
    {
        _rot.y += 360.0f;
    }
}

void Camera::RotateRight(bool keydown)
{
    if (keydown)
    {
        _rightTurnSpeed += _frameTime * 0.01f;
        if (_rightTurnSpeed > (_frameTime * 0.15f))
        {
            _rightTurnSpeed = _frameTime * 0.15f;
        }
    }
    else
    {
        _rightTurnSpeed -= _frameTime * 0.005f;
        if (_rightTurnSpeed < 0.0f)
        {
            _rightTurnSpeed = 0.0f;
        }
    }

    _rot.y += _rightTurnSpeed;
    if (_rot.y > 360.0f)
    {
        _rot.y -= 360.0f;
    }
}

void Camera::LookUpward(bool keydown)
{
    if (keydown)
    {
        _lookUpSpeed += _frameTime * 0.01f;
        if (_lookUpSpeed > (_frameTime * 0.15f))
        {
            _lookUpSpeed = _frameTime * 0.15f;
        }
    }
    else
    {
        _lookUpSpeed -= _frameTime * 0.005f;
        if (_lookUpSpeed < 0.0f)
        {
            _lookUpSpeed = 0.0f;
        }
    }

    _rot.x -= _lookUpSpeed;
    if (_rot.x > 90.0f)
    {
        _rot.x = 90.0f;
    }
}

void Camera::LookDownward(bool keydown)
{
    if (keydown)
    {
        _lookDownSpeed += _frameTime * 0.01f;
        if (_lookDownSpeed > (_frameTime * 0.15f))
        {
            _lookDownSpeed = _frameTime * 0.15f;
        }
    }
    else
    {
        _lookDownSpeed -= _frameTime * 0.005f;
        if (_lookDownSpeed < 0.0f)
        {
            _lookDownSpeed = 0.0f;
        }
    }

    _rot.x += _lookDownSpeed;
    if (_rot.x < -90.0f)
    {
        _rot.x = -90.0f;
    }
}

void Camera::SetPos(float x, float y, float z)
{
    _pos.x = x;
    _pos.y = y;
    _pos.z = z;
}

void Camera::SetRot(float x, float y, float z)
{
    _rot.x = x;
    _rot.y = y;
    _rot.z = z;
}

CXMMATRIX Camera::GetViewMatrix()
{
    return _viewMatrix;
}
