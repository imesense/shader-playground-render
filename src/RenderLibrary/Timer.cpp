#include "StdAfx.h"

#include "Timer.hpp"

using namespace ShaderPlayground;

bool Timer::Init()
{
    QueryPerformanceFrequency((LARGE_INTEGER*) &_frequency);
    if (_frequency == 0)
    {
        return false;
    }

    _ticksPerMs = (float) (_frequency / 1000);

    QueryPerformanceCounter((LARGE_INTEGER*) &_startTime);

    return true;
}

void Timer::Frame()
{
    INT64 currentTime = 0;
    QueryPerformanceCounter((LARGE_INTEGER*) &currentTime);

    float timeDifference = (float) (currentTime - _startTime);

    _frameTime = timeDifference / _ticksPerMs;
    _startTime = currentTime;
}

float Timer::GetTime() const
{
    return _frameTime;
}
