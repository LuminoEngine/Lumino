// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

namespace ln {
class CommandQueue;

/** Graphics */
LN_CLASS(Static)
class Graphics
{
public:
	/** activeGraphicsAPI */
	LN_METHOD(Property)
	static GraphicsAPI activeGraphicsAPI();


	static CommandQueue* graphicsQueue();
	static CommandQueue* computeQueue();

	// return: extensionId
	int registerExtension(INativeGraphicsExtension* extension);
	void unregisterExtension(INativeGraphicsExtension* extension);
};

} // namespace ln
