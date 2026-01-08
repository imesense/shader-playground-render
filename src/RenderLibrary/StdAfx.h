#ifndef STDAFX_H
#define STDAFX_H

#ifdef RENDERLIBRARY_EXPORTS
#define RENDERLIBRARY_API __declspec(dllexport)
#else
#define RENDERLIBRARY_API __declspec(dllimport)
#endif

#include "framework.hpp"

#endif // STDAFX_H
