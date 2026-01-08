#pragma once

namespace ShaderPlayground
{
    class DX11ViewRender;

    class RenderTarget
    {
    private:
        XMMATRIX _projectionMatrix;
        XMMATRIX _orthoMatrix;
        D3D11_VIEWPORT _viewport;

        DX11ViewRender* _render;

        ID3D11Texture2D* _RTTexture;
        ID3D11Texture2D* _DSTexture;
        ID3D11RenderTargetView* _RTV;
        ID3D11ShaderResourceView* _SRV;
        ID3D11DepthStencilView* _DSV;

    public:
        RENDERLIBRARY_API RenderTarget(DX11ViewRender* render);

        RENDERLIBRARY_API bool Init(float screenNear, float screenDepth);
        RENDERLIBRARY_API void Close();

        RENDERLIBRARY_API void SetRenderTarget();
        RENDERLIBRARY_API void ClearRenderTarget(float, float, float, float);

        // Получаем текстуру RT в виде shader resource view
        RENDERLIBRARY_API ID3D11ShaderResourceView* GetShaderResourceView();

        RENDERLIBRARY_API XMMATRIX GetProjectionMatrix();

        //void* operator new(size_t i) {
        //    return _aligned_malloc(i, 16);
        //}

        //void operator delete(void* p) {
        //    _aligned_free(p);
        //}
    };
}
