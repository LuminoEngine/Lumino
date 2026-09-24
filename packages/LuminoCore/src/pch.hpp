#pragma once
#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#include <d3dcompiler.h>
#endif

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

#include <iostream>
#include <algorithm>
#include <numeric>
#include <optional>

#include <LuminoCore/Common.hpp>
