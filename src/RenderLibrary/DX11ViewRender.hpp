#pragma once

namespace ShaderPlayground
{
    class DX11ViewRender : public Render, public Allocator
    {
    private:
        friend DepthShader;
        friend RenderTarget;
        friend ShadowShader;
        friend class MyInput;

        Camera _cam;
        Light _Light;

        RenderTarget* _RenderTexture;
        DepthShader* _DepthShader;
        ShadowShader* _ShadowShader;

        D3D11_VIEWPORT _viewport;

        static DX11ViewRender* GetInstance;

        // индексный и вершинный буферы для ящика и поверхности
        ID3D11Buffer* _vb_ground;
        ID3D11Buffer* _ib_ground;
        ID3D11Buffer* _vb_box;
        ID3D11Buffer* _ib_box;

        // текстуры
        ID3D11ShaderResourceView* _texture_ground;
        ID3D11ShaderResourceView* _texture_box1;
        ID3D11ShaderResourceView* _texture_box2;

        // позиции первого и второго ящика
        XMFLOAT3 _posbox1;
        XMFLOAT3 _posbox2;

    public:
        bool _key_up;
        bool _key_down;
        bool _key_left;
        bool _key_right;
        bool _key_a;
        bool _key_z;
        bool _key_s;
        bool _key_x;

    public:
        RENDERLIBRARY_API DX11ViewRender();

        RENDERLIBRARY_API bool Init();
        RENDERLIBRARY_API bool Draw();
        RENDERLIBRARY_API void Close();

        RENDERLIBRARY_API void RenderSceneToTexture();
        RENDERLIBRARY_API void RenderSceneToWindow();

        static DX11ViewRender* GetDX11ViewRender()
        {
            return GetInstance;
        }
    };
}
