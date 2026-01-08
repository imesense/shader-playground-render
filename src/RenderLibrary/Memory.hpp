#pragma once

namespace ShaderPlayground
{
    class Allocator
    {
    public:
        RENDERLIBRARY_API Allocator()
        {
        }
        RENDERLIBRARY_API ~Allocator()
        {
        }

        RENDERLIBRARY_API void* operator new(std::size_t size);
        RENDERLIBRARY_API void operator delete(void* ptr) noexcept;

        RENDERLIBRARY_API void* operator new[](std::size_t size);
        RENDERLIBRARY_API void operator delete[](void* ptr) noexcept;

        RENDERLIBRARY_API static void AddToCollection(void* ptr);
        RENDERLIBRARY_API static void RemoveFromCollection(void* ptr);
        RENDERLIBRARY_API static void PrintCollection();

    private:
        static std::unordered_set<void*> objectCollection;
    };
}

extern RENDERLIBRARY_API ShaderPlayground::Allocator Memory;
