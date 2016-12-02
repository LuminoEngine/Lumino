
#pragma once
#include <memory>
#include <Lumino/Base/List.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/ElapsedTimer.h>
#include <Lumino/Threading/Mutex.h>

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class EngineDiag
{
public:

	static int GetGraphicsDeviceDrawCount();

	static int GetVisualNodeDrawCount();
};

LN_NAMESPACE_END
