
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/Assets.hpp>

namespace ln {

//=============================================================================
// Asset

bool Assets::existsFile(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->existsFile(filePath);
}

Ref<Texture2D> Assets::loadTexture(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->loadTexture(filePath);
}

Ref<Shader> Assets::loadShader(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->loadShader(filePath);
}

Ref<ByteBuffer> Assets::readAllBytes(const StringRef& filePath)
{
	return detail::EngineDomain::assetManager()->readAllBytes(filePath);
}

} // namespace ln

