#pragma once
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine.hpp>
#include "gtest/gtest.h"
#include "../../../build/BuildCache/Streams/source/Stream.h"

template<class T>
using Ref = ln::Ref<T>;
