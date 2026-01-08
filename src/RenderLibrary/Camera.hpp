#pragma once

namespace ShaderPlayground
{
    class Camera
    {
    private:
        Timer _time;

        XMMATRIX _viewMatrix;

        XMFLOAT3 _pos;
        XMFLOAT3 _rot;

        float _frameTime;

        float _forwardSpeed, _backwardSpeed;
        float _upwardSpeed, _downwardSpeed;
        float _leftTurnSpeed, _rightTurnSpeed;
        float _leftSpeed, _rightSpeed;
        float _lookUpSpeed, _lookDownSpeed;

    public:
        RENDERLIBRARY_API Camera();

        RENDERLIBRARY_API void Render();

        RENDERLIBRARY_API void MoveForward(bool keydown);
        RENDERLIBRARY_API void MoveBackward(bool keydown);
        RENDERLIBRARY_API void MoveLeft(bool keydown);
        RENDERLIBRARY_API void MoveRight(bool keydown);

        RENDERLIBRARY_API void MoveUpward(bool keydown);
        RENDERLIBRARY_API void MoveDownward(bool keydown);

        RENDERLIBRARY_API void RotateLeft(bool keydown);
        RENDERLIBRARY_API void RotateRight(bool keydown);

        RENDERLIBRARY_API void LookUpward(bool keydown);
        RENDERLIBRARY_API void LookDownward(bool keydown);

        RENDERLIBRARY_API void SetPos(float x, float y, float z);
        RENDERLIBRARY_API void SetRot(float x, float y, float z);

        RENDERLIBRARY_API XMFLOAT3 GetPos()
        {
            return _pos;
        }
        RENDERLIBRARY_API XMFLOAT3 GetRot()
        {
            return _rot;
        }

        RENDERLIBRARY_API CXMMATRIX GetViewMatrix();
    };
}
