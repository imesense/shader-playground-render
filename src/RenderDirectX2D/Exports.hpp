#pragma once 

#ifdef RENDER_DIRECTX_2D
#define RENDERLIBRARY_API __declspec(dllexport)
#else
#define RENDERLIBRARY_API __declspec(dllimport)
#endif

extern "C"
{
    RENDERLIBRARY_API int START_2D();
}
