#include "StdAfx.h"

#include <timeapi.h>

#include "FpsCounter.hpp"

using namespace ShaderPlayground;

void FpsCounter::Initialize()
{
    _fps = 0;
    _count = 0;
    _startTime = timeGetTime();
}

void FpsCounter::Frame()
{
    _count++;

    if (timeGetTime() >= (_startTime + 1000))
    {
        _fps = _count;
        _count = 0;
        _startTime = timeGetTime();
    }
}

int FpsCounter::GetFps() const
{
    return _fps;
}
