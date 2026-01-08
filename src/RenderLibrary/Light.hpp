#pragma once

namespace ShaderPlayground
{
    class Light
    {
    private:
        XMMATRIX _viewMatrix;
        XMMATRIX _projectionMatrix;

        XMFLOAT4 _ambientColor;
        XMFLOAT4 _diffuseColor;
        XMFLOAT3 _position;
        XMFLOAT3 _lookAt;

    public:
        RENDERLIBRARY_API void SetAmbientColor(float, float, float, float);
        RENDERLIBRARY_API void SetDiffuseColor(float, float, float, float);
        RENDERLIBRARY_API void SetPosition(float, float, float);
        RENDERLIBRARY_API void SetLookAt(float, float, float);

        RENDERLIBRARY_API XMFLOAT4 GetAmbientColor();
        RENDERLIBRARY_API XMFLOAT4 GetDiffuseColor();
        RENDERLIBRARY_API XMFLOAT3 GetPosition();

        RENDERLIBRARY_API void GenerateViewMatrix();
        RENDERLIBRARY_API void GenerateProjectionMatrix(float, float);

        RENDERLIBRARY_API XMMATRIX GetViewMatrix();
        RENDERLIBRARY_API XMMATRIX GetProjectionMatrix();
    };
}
