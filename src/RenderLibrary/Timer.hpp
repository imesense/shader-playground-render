#pragma once

namespace ShaderPlayground
{
    class Timer
    {
    private:
        INT64 _frequency;
        INT64 _startTime;

        float _ticksPerMs;
        float _frameTime;

    public:
        RENDERLIBRARY_API bool Init();
        RENDERLIBRARY_API void Frame();

        RENDERLIBRARY_API float GetTime() const;
    };
}
