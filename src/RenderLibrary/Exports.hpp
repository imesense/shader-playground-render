#pragma once

extern "C"
{
    RENDERLIBRARY_API HWND CreateFabricDirectXWindow();
    RENDERLIBRARY_API void CreateScene(HWND ptr);
    RENDERLIBRARY_API void MessageLoop();
    RENDERLIBRARY_API bool Frame();
    RENDERLIBRARY_API void DestroyFabricDirectXWindow();
}
