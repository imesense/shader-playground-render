#pragma once

namespace ShaderPlayground
{
    class Shader
    {
    private:
        Render* _render;

        ID3D11VertexShader* _vertexShader;
        ID3D11PixelShader* _pixelShader;
        ID3D11InputLayout* _layout;
        std::vector<ID3D11ShaderResourceView*> _textures;
        D3D11_INPUT_ELEMENT_DESC* _layoutformat;
        unsigned int _numlayout;

    private:
        HRESULT Compileshaderfromfile(const char* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut);

    public:
        RENDERLIBRARY_API Shader(Render* render);

        // данную функцию мы должны вызывать до вызова CreateShader
        RENDERLIBRARY_API void AddInputElementDesc(const char* SemanticName, DXGI_FORMAT format);
        RENDERLIBRARY_API void AddInputElementDesc(const char* SemanticName, unsigned int SemanticIndex, DXGI_FORMAT format,
            unsigned int InputSlot = 0, bool AlignedByteOffset = true,
            D3D11_INPUT_CLASSIFICATION InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            unsigned int InstanceDataStepRate = 0);

        RENDERLIBRARY_API bool CreateShader(const char* namevs, const char* nameps);
        RENDERLIBRARY_API bool AddTexture(const wchar_t* name);

        RENDERLIBRARY_API void Draw();
        RENDERLIBRARY_API void Close();
    };
}
