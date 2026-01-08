#pragma once

namespace ShaderPlayground
{
    class FpsCounter
    {
    private:
        int _fps;
        int _count;
        unsigned long _startTime;

    public:
        RENDERLIBRARY_API void Initialize();
        RENDERLIBRARY_API void Frame();
        RENDERLIBRARY_API int GetFps() const;
    };
}
