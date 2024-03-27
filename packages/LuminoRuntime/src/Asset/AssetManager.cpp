
#include "Internal.hpp"
#include "../../../../lumino/LuminoCore/src/IO/PathHelper.hpp"
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
#include "AssetArchive.hpp"
#include <LuminoEngine/Asset/detail/AssetManager.hpp>

namespace ln {
namespace detail {

//==============================================================================
// AssetManager

const String AssetManager::AssetPathPrefix = _TT("asset://");
const String AssetManager::LocalhostPrefix = _TT("local");

//AssetManager* AssetManager::initialize(const Settings& settings)
//{
//    if (instance()) return instance();
//
//    auto m = Ref<AssetManager>(LN_NEW detail::AssetManager(), false);
//    if (!m->init(settings)) return nullptr;
//
//    EngineContext2::instance()->registerModule(m);
//    EngineContext2::instance()->assetManager = m;
//    return m;
//}
//
//void AssetManager::terminate()
//{
//    if (instance()) {
//        instance()->dispose();
//        EngineContext2::instance()->unregisterModule(instance());
//        EngineContext2::instance()->assetManager = nullptr;
//    }
//}

AssetManager::AssetManager()
	: m_storageAccessPriority(AssetStorageAccessPriority::DirectoryFirst)
{
}

AssetManager::~AssetManager()
{
}

bool AssetManager::init(const Settings& settings)
{
    LN_LOG_DEBUG("AssetManager Initialization started.");

    m_storageAccessPriority = settings.assetStorageAccessPriority;

    // "file" scheme でアクセスする Archive を登録する
    if (m_storageAccessPriority != AssetStorageAccessPriority::ArchiveOnly) {
        m_localFileSystemArchive = makeRef<FileSystemReader>();
        m_requestedArchives.add(m_localFileSystemArchive);
        refreshActualArchives();
    }

    LN_LOG_DEBUG("AssetManager Initialization ended.");
    return true;
}

void AssetManager::dispose()
{
    for (auto& archive : m_requestedArchives) {
        archive->close();
    }
	m_requestedArchives.clear();
}

void AssetManager::addAssetDirectory(const StringView& path)
{
    // "asset" scheme でアクセスする Archive を登録する
	auto archive = makeRef<FileSystemReader>();
	archive->setRootPath(path);
	m_requestedArchives.add(archive);
    m_fileSystemArchives.add(archive);
	refreshActualArchives();

    //if (m_primaryLocalAssetDirectory.isEmpty()) {
    //    m_primaryLocalAssetDirectory = Path(path);
    //}

    LN_LOG_INFO(U"Asset directory added: " + path);
}

void AssetManager::mountAssetArchive(const StringView& filePath, const StringView& password)
{
    auto archive = makeRef<CryptedAssetArchiveReader>();
    bool result = archive->open(filePath, password, true);
    if (LN_ENSURE(result)) return;
	m_requestedArchives.add(archive);
	refreshActualArchives();

    LN_LOG_INFO(U"Asset archive added: " + filePath);
}

void AssetManager::removeAllAssetDirectory()
{
    m_requestedArchives.removeAllIf([](auto& x) { return x->storageKind() == AssetArchiveStorageKind::AssetDirectory; });
    refreshActualArchives();
}

//
//void AssetManager::setAssetStorageAccessPriority(AssetStorageAccessPriority value)
//{
//	m_storageAccessPriority = value;
//	refreshActualArchives();
//}
//

//Optional<AssetPath> AssetManager::findAssetPath(const AssetPath& assetPath, const Char** exts, int extsCount) const
//{
//    const Char* begin = filePath.data();
//    const Char* end = filePath.data() + filePath.length();
//    if (detail::PathTraits::getExtensionBegin(begin, end, false) != end) {
//        // has extension
//        paths->add(Path(filePath).unify());
//    }
//    else {
//        for (int i = 0; i < extsCount; i++) {
//            paths->add(Path(String(filePath) + exts[i]).unify());   // TODO: operator StringView + Char*
//        }
//    }
//}

Optional_deprecated<AssetPath> AssetManager::findAssetPath(const StringView& filePath, const Char* const* exts, int extsCount) const
{
    List<Path> paths;
    paths.reserve(extsCount);
    makeFindPaths(filePath, exts, extsCount, &paths);

    AssetPath result;
    for (auto& path : paths) {
        //if (m_storageAccessPriority == AssetStorageAccessPriority::AllowLocalDirectory) {
        //    auto localPath = path.canonicalize();
        //    if (FileSystem::existsFile(localPath)) {
        //        result = localPath;// String::concat(LocalhostPrefix, _TT("/", localPath.unify().str());
        //    }
        //}
        //else
        {
            for (auto& archive : m_actualArchives) {
                if (archive->existsFile(path)) {
                    result = AssetPath(archive->scheme(), archive->name(), path);
                    break;
                }
            }
        }
        if (!result.isNull()) break;
    }

    if (!result.isNull())
        return result;
        // TODO: atchive 内のファイルには host もほしい
        //return AssetPath::makeFromLocalFilePath(result);
        //return AssetPathPrefix + result;
    else
        return std::nullopt;
}

bool AssetManager::existsAsset(const AssetPath& assetPath) const
{
	//String archiveName;
	//Path path;
	//if (tryParseAssetPath(assetPath, &archiveName, &path)) {
		//if (String::compare(assetPath.scheme(), _TT("file", CaseSensitivity::CaseInsensitive) == 0) {
		//	if (m_storageAccessPriority == AssetStorageAccessPriority::AllowLocalDirectory) {
		//		return FileSystem::existsFile(assetPath.path());
		//	}
		//}
		//else
        {
			for (auto& archive : m_actualArchives) {
				if (archive->existsFile(assetPath.path())) {
					return true;
				}
			}
		}
	//}

	return false;
}

Ref<Stream> AssetManager::openStreamFromAssetPath(const AssetPath& assetPath) const
{
    //String archiveName;
    //Path path;
    //if (tryParseAssetPath(assetPath, &archiveName, &path)) {
        //if (String::compare(archiveName, LocalhostPrefix, CaseSensitivity::CaseInsensitive) == 0) {
		//if (String::compare(assetPath.scheme(), _TT("file", CaseSensitivity::CaseInsensitive) == 0) {
  //          if (m_storageAccessPriority == AssetStorageAccessPriority::AllowLocalDirectory) {
  //              return FileStream::create(assetPath.path(), FileOpenMode::Read);
  //          }
  //      }
  //      else
        {
            for (auto& archive : m_actualArchives) {
                auto stream = archive->openFileStream(assetPath.path());
                if (stream) {
                    return stream;
                }
            }
        }
    //}

    return nullptr;
}

AssetPath AssetManager::resolveAssetPath(const AssetPath& assetPath, const Char** exts, int extsCount) const
{
    // scheme や host が指定されている場合はそこを検索したい
    AssetArchive* priorityArchive = nullptr;
    if (assetPath.scheme() == AssetPath::FileSchemeName) {
        if (m_localFileSystemArchive) {
            priorityArchive = m_localFileSystemArchive;
        }
        else {
            return AssetPath();
        }
    }
    else if (!assetPath.host().isEmpty()) {
        priorityArchive = m_actualArchives.findIf([&](const AssetArchive* ar) { return ar->name() == assetPath.host(); }).valueOr(nullptr);
        if (!priorityArchive) {
            return AssetPath();
        }
    }


    AssetPath result;
    for (int i = 0; i < extsCount; i++) {
        Path localPath = (assetPath.path().hasExtension()) ? assetPath.path() : Path(assetPath.path() + exts[i]);

        if (priorityArchive) {
            if (priorityArchive->existsFile(localPath)) {
                result = AssetPath(priorityArchive->scheme(), priorityArchive->name(), localPath);
                break;
            }
        }
        else {
            for (auto& archive : m_actualArchives) {
                if (archive->existsFile(localPath)) {
                    result = AssetPath(archive->scheme(), archive->name(), localPath);
                    break;
                }
            }
        }
    }

    return result;
}

Ref<AssetModel> AssetManager::loadAssetModelFromLocalFile(const String& filePath) const
{
    auto assetPath = findAssetPath(filePath);
    if (assetPath) {
        return loadAssetModelFromAssetPath(*assetPath);
    }
    else {
        LN_WARNING(_TT("Asset not found: ") + String(filePath));    // TODO: operator
        return nullptr;
    }
}

Ref<AssetModel> AssetManager::loadAssetModelFromAssetPath(const AssetPath& assetPath) const
{
    auto stream = openStreamFromAssetPath(assetPath);
    auto text = FileSystem::readAllText(stream);
    auto asset = Serializer2_deprecated::deserialize(text.unwrap(), assetPath.getParentAssetPath());
    asset->target()->setAssetPath(assetPath);
    return asset;
}

Optional_deprecated<AssetPath> AssetManager::findAssetPath(const StringView& filePath) const
{
    const Char* ext = AssetModel::AssetFileExtension.c_str();
    return findAssetPath(filePath, &ext, 1);
}

bool AssetManager::loadAssetModelFromAssetPathToInstance(Object* obj, const AssetPath& assetPath) const
{
    // TODO: ネットワークからのダウンロードとか。失敗したら return false

    auto stream = openStreamFromAssetPath(assetPath);
    auto text = FileSystem::readAllText(stream);

    auto asset = makeObject_deprecated<AssetModel>(obj);
    Serializer2_deprecated::deserializeInstance(asset, text.unwrap(), assetPath.getParentAssetPath());

    obj->setAssetPath(assetPath);

    return true;
}

void AssetManager::saveAssetModelToLocalFile(AssetModel* asset, const String& filePath) const
{
    if (LN_REQUIRE(asset)) return;
    if (LN_REQUIRE(!m_fileSystemArchives.isEmpty())) return;

    AssetPath assetPath;
    Path localPath;
    if (!filePath.isEmpty()) {
        auto primaryArchive = primaryAssetDirectoryArchive();
        auto primaryLocalAssetDirectory = primaryArchive->rootPath();
        auto fullPath = Path(primaryLocalAssetDirectory, filePath);
        if (primaryLocalAssetDirectory.contains(fullPath)) {
            assetPath = AssetPath(primaryArchive->scheme(), primaryArchive->name(), primaryLocalAssetDirectory.makeRelative(fullPath));
        }
        else {
            assetPath = AssetPath::makeFromLocalFilePath(fullPath);
        }
        localPath = fullPath;
    }
    else if (!asset->target()->assetPath().isNull()) {
        assetPath = asset->target()->assetPath();
        localPath = assetPathToLocalFullPath(assetPath);
    }
    else {
        LN_UNREACHABLE();
        return;
    }

    if (!localPath.hasExtension()) {
        localPath = localPath.replaceExtension(AssetModel::AssetFileExtension);
    }

    String text = Serializer2_deprecated::serialize(asset, assetPath.getParentAssetPath());
    FileSystem::writeAllText(localPath, text);

    ////auto json = JsonSerializer::serialize(*asset, assetPath.getParentAssetPath().toString(), JsonFormatting::Indented);
    //auto serializer = makeObject_deprecated<Serializer>(); // TODO: Pool
    //JsonTextOutputArchive ar;
    //ar.m_serializer = serializer;
    //ar.setBasePath(assetPath.getParentAssetPath().toString());
    //ar.save(*asset);
    //auto json = ar.toString(JsonFormatting::Indented);

    //FileSystem::writeAllText(localPath, json);
}

String AssetManager::assetPathToLocalFullPath(const AssetPath& assetPath) const
{
    //LN_CHECK(!m_primaryLocalAssetDirectory.isEmpty());
    LN_CHECK(!assetPath.isNull());
    // TODO: assetPath の持ってる host に応じて 親フォルダを変えるべきか。

    auto primaryArchive = primaryAssetDirectoryArchive();
    auto primaryLocalAssetDirectory = primaryArchive->rootPath();

    return Path(primaryLocalAssetDirectory, assetPath.path());
}

//String AssetManager::localFullPathToAssetPath(const String& localFullPath) const
//{
//    // TODO: ちょっと手抜き
//    return AssetPathPrefix + _TT("/" + localFullPath;
//}

//String AssetManager::getParentAssetPath(const String& assetPath)
//{
//    String archiveName;
//    Path localPath;
//    tryParseAssetPath(assetPath, &archiveName, &localPath);
//
//    if (localPath.str().contains('/'))
//        localPath = localPath.parent();
//    else
//        localPath = _TT("";
//
//    return AssetPathPrefix + archiveName + _TT("/" + localPath.str();
//}

//String AssetManager::combineAssetPath(const String& assetFullBasePath, const String& localAssetPath)
//{
//    if (localAssetPath.indexOf(AssetPathPrefix) == 0) {
//        return localAssetPath;
//    }
//    else {
//        String archiveName;
//        Path localPath;
//        tryParseAssetPath(assetFullBasePath, &archiveName, &localPath);
//
//
//        return AssetPathPrefix + archiveName + canonicalizeAssetPath(_TT("/" + localPath.str() + _TT("/" + localAssetPath);
//    }
//}

//String AssetManager::makeRelativeAssetPath(const String& assetFullBasePath, const String& assetFullPath)
//{
//    LN_CHECK(assetFullBasePath.indexOf(AssetPathPrefix) == 0);
//    LN_CHECK(assetFullPath.indexOf(AssetPathPrefix) == 0);
//
//    String archiveName1, archiveName2;
//    Path localPath1, localPath2;
//    tryParseAssetPath(assetFullBasePath, &archiveName1, &localPath1);
//    tryParseAssetPath(assetFullPath, &archiveName2, &localPath2);
//
//    localPath1 = Path(_TT("/", localPath1);
//    localPath2 = Path(_TT("/", localPath2);
//    return localPath1.makeRelative(localPath2);
//}

String AssetManager::canonicalizeAssetPath(const String& assetPath)
{
    std::vector<Char> tmpPath(assetPath.length() + 1);
    int len = detail::PathTraits::canonicalizePath(assetPath.c_str(), assetPath.length(), tmpPath.data(), tmpPath.size());
    return Path(StringView(tmpPath.data(), len)).unify();
}

bool AssetManager::existsFile(const StringView& filePath) const
{
    auto unifiedFilePath = Path(filePath).unify();
    for (auto& archive : m_actualArchives) {
        if (archive->existsFile(unifiedFilePath)) {
            return true;
        }
    }

    // TODO: dummy archive

    return FileSystem::existsFile(filePath);
}

Ref<Stream> AssetManager::openFileStream(const StringView& filePath)
{
	auto unifiedFilePath = Path(filePath).unify();
	for (auto& archive : m_actualArchives) {
		auto stream = archive->openFileStream(unifiedFilePath);
		if (stream) {
			return stream;
		}
	}

	// TODO: dummy archive

	return FileStream::create(filePath, FileOpenMode::Read);
}

Ref<ByteBuffer> AssetManager::readAllBytes(const StringView& filePath)
{
	auto stream = openFileStream(filePath);
	auto buffer = makeRef<ByteBuffer>(stream->length());
	stream->read(buffer->data(), buffer->size());
	return buffer;
}

void AssetManager::buildAssetIndexFromLocalFiles(const ln::Path& assetDir)
{
    m_assetIndex.clear();

    // TODO: 量が多くなると重くなるのでインデックス作成スレッド建てたい
    auto assetFiles = ln::FileSystem::getFiles(assetDir, _TT("*.lnasset"), ln::SearchOption::Recursive);
    for (auto asset : assetFiles) {
        auto id = AssetModel::readAssetId(asset);
        if (!id.isEmpty()) {
            m_assetIndex[id] = asset;
        }
    }
}

void AssetManager::refreshActualArchives()
{
	m_actualArchives.clear();

	switch (m_storageAccessPriority)
	{
	case AssetStorageAccessPriority::DirectoryFirst:
		for (auto& ac : m_requestedArchives) {
			if (ac->storageKind() == AssetArchiveStorageKind::AssetDirectory) {
				m_actualArchives.add(ac);
			}
		}
        for (auto& ac : m_requestedArchives) {
            if (ac->storageKind() == AssetArchiveStorageKind::LocalDirectory) {
                m_actualArchives.add(ac);
            }
        }
		for (auto& ac : m_requestedArchives) {
			if (ac->storageKind() == AssetArchiveStorageKind::ArchiveFile) {
				m_actualArchives.add(ac);
			}
		}
		break;
	case AssetStorageAccessPriority::ArchiveFirst:
		for (auto& ac : m_requestedArchives) {
			if (ac->storageKind() == AssetArchiveStorageKind::ArchiveFile) {
				m_actualArchives.add(ac);
			}
		}
		for (auto& ac : m_requestedArchives) {
			if (ac->storageKind() == AssetArchiveStorageKind::AssetDirectory) {
				m_actualArchives.add(ac);
			}
		}
        for (auto& ac : m_requestedArchives) {
            if (ac->storageKind() == AssetArchiveStorageKind::LocalDirectory) {
                m_actualArchives.add(ac);
            }
        }
		break;
	case AssetStorageAccessPriority::ArchiveOnly:
		for (auto& ac : m_requestedArchives) {
			if (ac->storageKind() == AssetArchiveStorageKind::ArchiveFile) {
				m_actualArchives.add(ac);
			}
		}
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

bool AssetManager::existsFileInternal(const StringView& filePath, const Char** exts, int extsCount) const
{
	List<Path> paths;
	paths.reserve(extsCount);	// TODO: 毎回メモリ確保したくない気がするので、template と lambda なコールバックで処理してみたい
	makeFindPaths(filePath, exts, extsCount, &paths);

	auto unifiedFilePath = Path(filePath).unify();
	for (auto& path : paths) {
		for (auto& archive : m_actualArchives) {
			if (archive->existsFile(path)) {
				return true;
			}
		}
	}

	return false;
}

Ref<Stream> AssetManager::openFileStreamInternal(const StringView& filePath, const Char** exts, int extsCount, Path* outPath)
{
	List<Path> paths;
	paths.reserve(extsCount);
	makeFindPaths(filePath, exts, extsCount, &paths);

	auto unifiedFilePath = Path(filePath).unify();
	for (auto& path : paths) {
        //if (m_storageAccessPriority == AssetStorageAccessPriority::AllowLocalDirectory) {
        //    path = path.canonicalize();
        //    if (FileSystem::existsFile(path)) {
        //        *outPath = path;
        //        return FileStream::create(path, FileOpenMode::Read);
        //    }
        //}
        //else
        {
            for (auto& archive : m_actualArchives) {
                auto stream = archive->openFileStream(path);
                if (stream) {
                    *outPath = path;
                    return stream;
                }
            }
        }
	}

	return nullptr;
}

void AssetManager::makeFindPaths(const StringView& filePath, const Char* const* exts, int extsCount, List<Path>* paths) const
{
	const Char* begin = filePath.data();
	const Char* end = filePath.data() + filePath.length();
	if (detail::PathTraits::getExtensionBegin(begin, end, false) != end) {
        // has extension
		paths->add(Path(filePath).unify());
	}
	else {
		for (int i = 0; i < extsCount; i++) {
			paths->add(Path(String(filePath) + exts[i]).unify());   // TODO: operator StringView + Char*
		}
	}
}

bool AssetManager::tryParseAssetPath(const String& assetPath, String* outArchiveName, Path* outLocalPath)
{
    if (assetPath.indexOf(AssetPathPrefix) != 0) return false;
    int separate1 = assetPath.indexOf('/', AssetPathPrefix.length(), CaseSensitivity::CaseInsensitive);
    if (separate1 < 0) return false;

    *outArchiveName = assetPath.substr(AssetPathPrefix.length(), separate1 - AssetPathPrefix.length());
    *outLocalPath = Path(assetPath.substr(separate1 + 1));
    return true;
}

} // namespace detail
} // namespace ln

