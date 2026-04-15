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
#include <cassert>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <memory>
#include <mutex>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

#include <LuminoC/lumino_types.h>
#include <LuminoBase.hpp>
