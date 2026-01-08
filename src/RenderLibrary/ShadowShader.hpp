#pragma once

namespace ShaderPlayground
{
    class DX11ViewRender;

    class ShadowShader : public Allocator
    {
    private:
        DX11ViewRender* _render;
        Shader* _shader;
        ID3D11SamplerState* _sampleStateWrap;
        ID3D11SamplerState* _sampleStateClamp;
        ID3D11Buffer* _matrixBuffer;
        ID3D11Buffer* _lightBuffer;
        ID3D11Buffer* _lightBuffer2;

    public:
        RENDERLIBRARY_API ShadowShader(DX11ViewRender* render);

        RENDERLIBRARY_API bool Init();
        RENDERLIBRARY_API void Close();

        RENDERLIBRARY_API void Render(int indexCount, CXMMATRIX worldMatrix, CXMMATRIX WVP, CXMMATRIX WVPlight,
            ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture,
            Light& light);
    };
}
