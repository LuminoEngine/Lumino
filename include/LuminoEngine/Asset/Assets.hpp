
#pragma once
#include "Common.hpp"

namespace ln {
class Shader;

class Assets
{
public:
    static bool existsFile(const StringRef& filePath);
    static Ref<Texture2D> loadTexture(const StringRef& filePath);
    static Ref<Shader> loadShader(const StringRef& filePath);
	static Ref<ByteBuffer> readAllBytes(const StringRef& filePath);
};

} // namespace ln
