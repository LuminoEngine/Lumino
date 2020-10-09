
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
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

bool AssetObject::initLoad(const Path& requiredLoadPath)
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

void AssetObject::serialize(Serializer2& ar)
{
	Object::serialize(ar);
	ar & makeNVP(u"file", m_resourceFilePath);
}

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
		if (m_requiredLoadPath.hasExtension(AssetModel::AssetFileExtension)) {
			// 相対パスの場合は Archive から .yml を検索する
			auto path = manager->findAssetPath(m_requiredLoadPath);
			if (path) {
				m_assetFilePath = *path;
			}
			else {
				// .yml が明示的に指定されていたが、Archive 内に見つからなかった。
				// ネットワークファイルかもしれないが、これは未対応。TODO:
				LN_ERROR("Network file not supported.");
				return;
			}
		}
		else {
			// .yml 以外の普通のファイル
		}
	}

	// .yml が要求されていれば、deserialize を試してみる
	bool loaded = false;
	if (m_assetFilePath.hasValue()) {
		loaded = manager->loadAssetModelFromAssetPathToInstance(this, m_assetFilePath);
	}

	// .png 等のリソース読み込み
	{
		detail::AssetPath assetPath;
		if (m_assetFilePath.hasValue()) {
			// .yml のパスが特定できていれば、それを基準に AssetPath を作る
			assetPath = detail::AssetPath::combineAssetPath(m_assetFilePath.getParentAssetPath(), m_resourceFilePath);
		}
		else {
			// 候補を検索
			const std::vector<const Char*> candidateExts = resourceExtensions();
			const Char* const* d = candidateExts.data();
			const auto path = manager->findAssetPath(m_requiredLoadPath, d, candidateExts.size());
			if (path) {
				assetPath = *path;
			}
			else {
				// TODO: Archive 内にはないのでネットワークファイルとしてパスを作る
				LN_ERROR("Network file not supported.");
				return;
			}
		}

		// Open Stream
		Ref<Stream> stream;
		if (!assetPath.isNull()) {
			stream = manager->openStreamFromAssetPath(assetPath);
		}
		else {
			LN_WARNING(u"Asset not found: " + m_resourceFilePath.str());    // TODO: operator
		}

		// Load Resource
		if (stream) {
			onLoadResourceFile(stream, assetPath);
		}
		else {
			LN_WARNING(u"Asset access denied: " + m_resourceFilePath.str());    // TODO: operator
		}
	}
}

const std::vector<const Char*>& AssetObject::resourceExtensions() const
{
	LN_UNREACHABLE();
	static const std::vector<const Char*> dummy;
	return dummy;
}

void AssetObject::onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath)
{
	// Texture2D や StaticMeshModel などは実装し、
	// サブクラス側で持っているファイルパスを元にオブジェクトを再構築する。
	LN_UNREACHABLE();
}

} // namespace ln

