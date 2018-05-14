#pragma once

#include <LuminoEngine.hpp>

enum class Result
{
	Failed = 0,
	OK = 1,
};

template<class T>
using Ref = ln::Ref<T>;
