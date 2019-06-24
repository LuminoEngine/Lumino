#pragma once
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine.hpp>
#include "gtest/gtest.h"

#include "../Core/SymbolDatabase.hpp"
#include "../Core/HeaderParser.hpp"

template<class T>
using Ref = ln::Ref<T>;
