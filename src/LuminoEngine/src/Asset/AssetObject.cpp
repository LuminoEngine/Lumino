
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

bool AssetObject::init(const Path& requiredLoadPath)
{
	if (!Object::init()) return false;
	m_requiredLoadPath = requiredLoadPath;
	reload();
	return true;
}

//void AssetObject::setAssetPath(const detail::AssetPath& value)
//{
//	m_assetFilePath = value;
//	//if (!value.hasExtension()) {
//	//	m_assetFilePath = value.replaceExtension(AssetModel::AssetFileExtension);
//	//}
//	//else {
//	//	m_assetFilePath = value;
//	//}
//}
//
void AssetObject::reload()
{
	auto* manager = detail::EngineDomain::assetManager();

	m_assetFilePath.clear();

	if (m_requiredLoadPath.isAbsolute()) {
		// フルパスの場合は File スキーマで AssetPath を作る。
		// 必ずローカルファイルとして扱うので、ファイルの存在確認をする。
		if (detail::AssetPath::isAssetFilePath(m_requiredLoadPath) &&
			FileSystem::existsFile(m_requiredLoadPath)) {
			// 拡張子まで明示されていればそのまま使う
			m_assetFilePath = detail::AssetPath::makeFromLocalFilePath(m_requiredLoadPath);
		}
		else if (!m_requiredLoadPath.hasExtension()) {
			// 拡張子を持っていない場合は .yml を付けて存在確認
			Path actualPath = m_requiredLoadPath.replaceExtension(AssetModel::AssetFileExtension);
			if (FileSystem::existsFile(actualPath)) {
				m_assetFilePath = detail::AssetPath::makeFromLocalFilePath(m_requiredLoadPath);
			}
		}
	}
	else {
		// 相対パスの場合は Archive から .yml を検索する
		auto path = manager->findAssetPath(m_requiredLoadPath);
		if (path) {
			m_assetFilePath = *path;
		}
	}

	// .yml が要求されていれば、読み込みを試してみる
	bool loaded = false;
	if (m_assetFilePath.hasValue()) {
		loaded = manager->loadAssetModelFromAssetPathToInstance(this, m_assetFilePath);
	}

	// .yml でなければ、.png など、サブクラス側に検索とロードを任せる
	if (!loaded) {
		onLoadResourceFile();
	}
}

void AssetObject::onLoadResourceFile()
{
	// Texture2D や StaticMeshModel などは実装し、
	// サブクラス側で持っているファイルパスを元にオブジェクトを再構築する。
	LN_UNREACHABLE();
}

} // namespace ln

