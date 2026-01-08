#include "StdAfx.h"

#include <iostream>
#include <new>
#include <unordered_set>

#include "Memory.hpp"

#pragma section(".Hook", read)

using namespace ShaderPlayground;

std::unordered_set<void*> Allocator::objectCollection;

void* Allocator::operator new(std::size_t size)
{
    void* ptr = std::malloc(size);
    AddToCollection(ptr);
    return ptr;
}

void Allocator::operator delete(void* ptr) noexcept
{
    std::free(ptr);
}

void* Allocator::operator new[](std::size_t size)
{
    void* ptr = std::malloc(size);
    return ptr;
}

void Allocator::operator delete[](void* ptr) noexcept
{
    std::free(ptr);
}

void Allocator::AddToCollection(void* ptr)
{
    objectCollection.insert(ptr);
}

void Allocator::RemoveFromCollection(void* ptr)
{
    objectCollection.erase(ptr);
}

void Allocator::PrintCollection()
{
    for (const auto& obj : objectCollection)
    {
        //Log::Get()->Debug("~ Object at address: %p", obj);
    }
}

#pragma init_seg(lib)
__declspec(allocate(".Hook"))
Allocator Memory;
