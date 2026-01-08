#pragma once

#include <string.h>
#include <stdlib.h>
#include <string>

#define _DELETE(p) { \
    if (p) \
    { \
        delete (p); \
        (p) = nullptr; \
    } \
}
#define _DELETE_ARRAY(p) { \
    if (p) \
    { \
        delete[] (p); \
        (p) = nullptr; \
    } \
}
#define _RELEASE(p) { \
    if (p) \
    { \
        (p)->Release(); \
        (p) = nullptr; \
    } \
}
#define _CLOSE(p) { \
    if (p) \
    { \
        (p)->Close(); \
        delete (p); \
        (p) = nullptr; \
    } \
}

#define _VERSION_FRAMEWORK 7

inline wchar_t* CharToWChar(char* mbString)
{
    int len = 0;
    len = (int) strlen(mbString) + 1;
    wchar_t* ucString = new wchar_t[len];

    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, ucString, len, mbString, len - 1);

    return ucString;
}

inline std::wstring intToStr(int i)
{
    wchar_t str[255];
    swprintf_s(str, 255, L"%d", i);
    return str;
}
