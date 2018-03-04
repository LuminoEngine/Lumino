
#pragma once
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public

#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#endif
#include <LuminoCore.h>
#include "../Include/Lumino/Graphics/Common.h"	// TODO: Internal.h には置きたくない
#include "EngineDomain.h"
