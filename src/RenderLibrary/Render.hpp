#pragma once

namespace ShaderPlayground
{
    class Render
    {
        friend class Shader;

    public:
        RENDERLIBRARY_API Render();
        RENDERLIBRARY_API virtual ~Render();

        RENDERLIBRARY_API bool CreateDevice(HWND hwnd);
        RENDERLIBRARY_API void BeginFrame();
        RENDERLIBRARY_API void EndFrame();
        RENDERLIBRARY_API void Shutdown();

        //RENDERLIBRARY_API virtual bool Init() = 0;
        //RENDERLIBRARY_API virtual bool Draw() = 0;
        //RENDERLIBRARY_API virtual void Close() = 0;

        RENDERLIBRARY_API void TurnZBufferOn();
        RENDERLIBRARY_API void TurnZBufferOff();

        RENDERLIBRARY_API void TurnOnAlphaBlending();
        RENDERLIBRARY_API void TurnOffAlphaBlending();

        //void* operator new(size_t i) {
        //    return _aligned_malloc(i, 16);
        //}

        //void operator delete(void* p) {
        //    _aligned_free(p);
        //}

    protected:
        bool Createdevice();
        bool Createdepthstencil();
        void Initmatrix();
        void Resize();

        RenderState* _renderstate;

        ID3D11Device* _pd3dDevice;
        ID3D11DeviceContext* _pImmediateContext;
        IDXGISwapChain* _pSwapChain;
        ID3D11RenderTargetView* _pRenderTargetView;
        ID3D11DepthStencilView* _pDepthStencilView;

        XMMATRIX _ortho;
        XMMATRIX _projection;

        HWND _hwnd;

        UINT _width = 640;
        UINT _height = 480;

        UINT UVsyncFlag = 0;
    };
}
