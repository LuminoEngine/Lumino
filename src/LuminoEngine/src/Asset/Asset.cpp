
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

} // namespace ln

