
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/Asset.hpp>

namespace ln {

//=============================================================================
// Asset

bool Asset::existsFile(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->existsFile(filePath);
}

Ref<Shader> Asset::loadShader(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->loadShader(filePath);
}

Ref<ByteBuffer> Asset::readAllBytes(const StringRef& filePath)
{
	return detail::EngineDomain::assetManager()->readAllBytes(filePath);
}

} // namespace ln

