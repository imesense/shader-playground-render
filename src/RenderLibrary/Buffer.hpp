#pragma once

namespace ShaderPlayground
{
    class Buffer
    {
    public:
        RENDERLIBRARY_API static ID3D11Buffer* CreateVertexBuffer(ID3D11Device* device, int size, bool dynamic, const void* Mem);
        RENDERLIBRARY_API static ID3D11Buffer* CreateIndexBuffer(ID3D11Device* device, int size, bool dynamic, const void* Mem);
        RENDERLIBRARY_API static ID3D11Buffer* CreateConstantBuffer(ID3D11Device* device, int size, bool dynamic);
    };
}
