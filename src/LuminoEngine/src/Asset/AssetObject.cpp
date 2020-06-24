
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

namespace ln {

//=============================================================================
// AssetObject

LN_OBJECT_IMPLEMENT(AssetObject, Object) {}

AssetObject::AssetObject()
{
}

bool AssetObject::init()
{
	return Object::init();
}

void AssetObject::setAssetPath(const Path& value)
{
	m_assetFilePath = value;
}

void AssetObject::reload()
{
	if (detail::AssetPath::isAssetFilePath(m_assetFilePath)) {
		LN_NOTIMPLEMENTED();
	}
	else {
		onLoadSourceFile();
	}
}

} // namespace ln

