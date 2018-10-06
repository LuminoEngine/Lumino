
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include "AssetManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AssetManager

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::initialize(const Settings& settings)
{
}

void AssetManager::dispose()
{
}

bool AssetManager::existsFile(const StringRef& filePath) const
{
	// TODO: archive

	return FileSystem::existsFile(filePath);
}

Ref<Texture2D> AssetManager::loadTexture(const StringRef& filePath)
{
	// TODO: cache

	//auto ref = newObject<Texture2D>(filePath, TextureFormat::RGBA32, true);

	LN_NOTIMPLEMENTED();
	return nullptr;
}

} // namespace detail
} // namespace ln

