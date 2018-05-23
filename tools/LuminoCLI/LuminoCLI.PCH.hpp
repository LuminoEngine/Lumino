#pragma once

#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>

enum class Result
{
	Failed = 0,
	OK = 1,
};

template<class T>
using Ref = ln::Ref<T>;
