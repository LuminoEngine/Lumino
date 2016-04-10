
#pragma once
#include <Lumino/Base/RefObject.h>

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class Assets
{
public:

	static Texture2DPtr LoadTexture(const StringRef& filePath);

};

LN_NAMESPACE_END
