
#include "Internal.hpp"
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
#include <LuminoEngine/Asset/AssetObject.hpp>

namespace ln {

//==============================================================================
// AssetImportSettings

LN_OBJECT_IMPLEMENT(AssetImportSettings, Object) {}

//==============================================================================
// AssetObject

LN_OBJECT_IMPLEMENT(AssetObject, Object) {}

AssetObject::AssetObject()
{
}

bool AssetObject::init()
{
	return Object::init();
}

//bool AssetObject::initLoad(const Path& requiredLoadPath)
//{
//	if (!Object::init()) return false;
//	m_requiredLoadPath = requiredLoadPath;
//	reload();
//	return true;
//}

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

	String file = (m_data) ? m_data->resourceFilePath.str() : String::Empty;

	ar & makeNVP(_TT("file"), file);

	if (ar.isLoading()) {
		LN_NOTIMPLEMENTED();
		// TODO: .yml から読み込んだということなので、他のフィールドも設定できる
		m_data->resourceFilePath = file;
	}
}

bool AssetObject::_resolveAssetRequiredPathSet(const detail::AssetPath* baseDir, const Path& requiredLoadPath, const std::vector<const Char*> candidateExts, detail::AssetRequiredPathSet* outPathSet)
{
	auto* manager = detail::AssetManager::instance();

	outPathSet->requiredLoadPath = requiredLoadPath;
	outPathSet->assetFilePath.clear();

	if (requiredLoadPath.isAbsolute()) {
		// フルパスの場合は File スキーマで AssetPath を作る。
		// 必ずローカルファイルとして扱うので、ファイルの存在確認をする。
		if (detail::AssetPath::isAssetFilePath(requiredLoadPath) &&
			FileSystem::existsFile(requiredLoadPath)) {
			// 拡張子まで明示されていればそのまま使う
			outPathSet->assetFilePath = detail::AssetPath::makeFromLocalFilePath(requiredLoadPath);
		}
		else if (!requiredLoadPath.hasExtension()) {
			// 拡張子を持っていない場合は .yml を付けて存在確認
			Path actualPath = requiredLoadPath.replaceExtension(AssetModel::AssetFileExtension);
			if (FileSystem::existsFile(actualPath)) {
				outPathSet->assetFilePath = detail::AssetPath::makeFromLocalFilePath(requiredLoadPath);
			}
		}
	}
	//else if (baseDir) {
	//	outPathSet->assetFilePath = detail::AssetPath::combineAssetPath(*baseDir, requiredLoadPath);
	//}
	else {
		if (requiredLoadPath.hasExtension(AssetModel::AssetFileExtension)) {
			// 相対パスの場合は Archive から .yml を検索する
			auto path = manager->findAssetPath(requiredLoadPath);
			if (path) {
				outPathSet->assetFilePath = *path;
			}
			else {
				// .yml が明示的に指定されていたが、Archive 内に見つからなかった。
				// ネットワークファイルかもしれないが、これは未対応。TODO:
				LN_ERROR(U"Network file not supported. " + requiredLoadPath);
				return false;
			}
		}
		else {
			// .yml 以外の普通のファイル
		}
	}

	outPathSet->finalResourceAssetFilePath.clear();
	detail::AssetPath assetPath;
	if (outPathSet->assetFilePath.hasValue()) {
		// .yml のパスが特定できていれば、それを基準に AssetPath を作る
		outPathSet->finalResourceAssetFilePath = detail::AssetPath::combineAssetPath(outPathSet->assetFilePath.getParentAssetPath(), outPathSet->resourceFilePath);
	}
	else if (baseDir) {
		outPathSet->finalResourceAssetFilePath = detail::AssetPath::combineAssetPath(*baseDir, requiredLoadPath);
	}
	else {
		// .yml 以外のファイル。候補を検索
		const Char* const* d = candidateExts.data();
		const auto path = manager->findAssetPath(requiredLoadPath, d, candidateExts.size());
		if (path) {
			outPathSet->finalResourceAssetFilePath = *path;
		}
		else {
			// TODO: Archive 内にはないのでネットワークファイルとしてパスを作る
			LN_ERROR(U"Network file not supported. " + requiredLoadPath);
			return false;
		}
	}

	return true;
}

//bool AssetObject::resolveAssetPathFromResourceFile(const detail::AssetPath& basePath, const Path& localPath, detail::AssetRequiredPathSet* outPathSet)
//{
//	return false;
//}

void AssetObject::reload()
{
	if (LN_REQUIRE(m_data)) return;

	auto* manager = detail::AssetManager::instance();

	// .yml が要求されていれば、deserialize を試してみる
	if (m_data->assetFilePath.hasValue()) {
		manager->loadAssetModelFromAssetPathToInstance(this, m_data->assetFilePath);

		// serialize 完了していれば m_data->finalResourceAssetFilePath 等が更新されている
	}

	// .png 等のリソース読み込み
	{
		// Open Stream
		Ref<Stream> stream;
		if (!m_data->finalResourceAssetFilePath.isNull()) {
			stream = manager->openStreamFromAssetPath(m_data->finalResourceAssetFilePath);
		}
		else {
			LN_WARNING(_TT("Asset not found: ") + m_data->resourceFilePath.str());    // TODO: operator
		}

		// Load Resource
		if (stream) {
			onLoadResourceFile(stream, m_data->finalResourceAssetFilePath);
		}
		else {
			LN_WARNING(_TT("Asset access denied: ") + m_data->resourceFilePath.str());    // TODO: operator
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
	// Texture2D や MeshModel などは実装し、
	// サブクラス側で持っているファイルパスを元にオブジェクトを再構築する。
	LN_UNREACHABLE();
}

} // namespace ln

