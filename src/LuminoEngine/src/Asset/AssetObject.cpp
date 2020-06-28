
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
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
	if (!value.hasExtension()) {
		m_assetFilePath = value.replaceExtension(AssetModel::AssetFileExtension);
	}
	else {
		m_assetFilePath = value;
	}
}

void AssetObject::reload()
{
	if (detail::AssetPath::isAssetFilePath(m_assetFilePath)) {
		Assets::reloadAsset(m_assetFilePath, this);
	}
	else {
		onLoadSourceFile();
	}
}

void AssetObject::onLoadSourceFile()
{
	// Texture2D や StaticMeshModel などは実装し、
	// サブクラス側で持っているファイルパスを元にオブジェクトを再構築する。
	LN_UNREACHABLE();
}

} // namespace ln

