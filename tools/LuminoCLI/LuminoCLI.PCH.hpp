#pragma once

#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <Shlobj.h>
#endif

#include "CLI.hpp"
#include <LuminoCore/Base/Result.hpp>
using Result = ln::Result;

//enum class Result
//{
//	Failed = 0,
//	OK = 1,
//};
//
//template<class T>
//using Ref = ln::Ref<T>;
