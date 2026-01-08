#pragma once

namespace ShaderPlayground
{
    class DX11ViewRender;

    class DepthShader
    {
    private:
        DX11ViewRender* _render;

        Shader* _shader;
        ID3D11Buffer* _matrixBuffer;

    public:
        RENDERLIBRARY_API DepthShader(DX11ViewRender* render);

        RENDERLIBRARY_API bool Init();
        RENDERLIBRARY_API void Close();
        RENDERLIBRARY_API void Render(int index, CXMMATRIX wvp);
    };
}
