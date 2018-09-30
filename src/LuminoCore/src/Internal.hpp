#pragma once
#include <LuminoCore/Base/Common.hpp>
#include <LuminoCore/Base/Assertion.hpp>
#include <stdlib.h>
#include <string.h>

#ifdef LN_OS_FAMILY_UNIX
#include <unistd.h>
#endif

#ifdef LN_OS_WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include "Base/CRTHelper.h"

