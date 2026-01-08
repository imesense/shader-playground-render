#pragma once

namespace ShaderPlayground
{
    class Frustum
    {
    private:
        XMVECTOR _planes[6];

    public:
        RENDERLIBRARY_API void ConstructFrustum(float screenDepth, CXMMATRIX projectionMatrix, CXMMATRIX viewMatrix);

        RENDERLIBRARY_API bool CheckPoint(float x, float y, float z);
        RENDERLIBRARY_API bool CheckCube(float xCenter, float yCenter, float zCenter, float size);
        RENDERLIBRARY_API bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
        RENDERLIBRARY_API bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
    };
}
