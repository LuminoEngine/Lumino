// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

namespace ln {
class CommandQueue;

/**  */
class LN_API Graphics
{
public:
	static CommandQueue* graphicsQueue();
	static CommandQueue* computeQueue();
	 
};

} // namespace ln
