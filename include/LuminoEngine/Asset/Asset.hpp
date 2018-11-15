
#pragma once
#include "Common.hpp"

namespace ln {

class Asset
{
public:
    static bool existsFile(const StringRef& filePath);
	static Ref<ByteBuffer> readAllBytes(const StringRef& filePath);
};

} // namespace ln
