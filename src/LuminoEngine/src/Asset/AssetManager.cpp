
#include "Internal.hpp"
#include "../../LuminoCore/src/IO/PathHelper.hpp"
//#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
#include "AssetArchive.hpp"
#include "AssetManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AssetManager

const String AssetManager::AssetPathPrefix = u"asset://";
const String AssetManager::LocalhostPrefix = u"local";

AssetManager::AssetManager()
	: m_storageAccessPriority(AssetStorageAccessPriority::DirectoryFirst)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "AssetManager Initialization started.";

    m_storageAccessPriority = settings.assetStorageAccessPriority;
    if (m_storageAccessPriority != AssetStorageAccessPriority::ArchiveOnly) {
        auto archive = makeRef<FileSystemReader>();
        m_requestedArchives.add(archive);
        refreshActualArchives();
        //addAssetDirectory(Environment::currentDirectory());
    }

    LN_LOG_DEBUG << "AssetManager Initialization ended.";
}

void AssetManager::dispose()
{
    for (auto& archive : m_requestedArchives) {
        archive->close();
    }
	m_requestedArchives.clear();
}

void AssetManager::addAssetDirectory(const StringRef& path)
{
	auto archive = makeRef<FileSystemReader>();
	archive->setRootPath(path);
	m_requestedArchives.add(archive);
	refreshActualArchives();

    if (m_primaryLocalAssetDirectory.isEmpty()) {
        m_primaryLocalAssetDirectory = Path(path);
    }

    LN_LOG_INFO << "Asset directory added: " << path;
}

void AssetManager::addAssetArchive(const StringRef& filePath, const StringRef& password)
{
    auto archive = makeRef<CryptedAssetArchiveReader>();
    bool result = archive->open(filePath, password, true);
    if (LN_ENSURE(result)) return;
	m_requestedArchives.add(archive);
	refreshActualArchives();

    LN_LOG_INFO << "Asset archive added: " << filePath;
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
Optional<AssetPath> AssetManager::findAssetPath(const StringRef& filePath, const Char** exts, int extsCount) const
{
    List<Path> paths;
    paths.reserve(extsCount);
    makeFindPaths(filePath, exts, extsCount, &paths);

    AssetPath result;
    for (auto& path : paths) {
        //if (m_storageAccessPriority == AssetStorageAccessPriority::AllowLocalDirectory) {
        //    auto localPath = path.canonicalize();
        //    if (FileSystem::existsFile(localPath)) {
        //        result = localPath;// String::concat(LocalhostPrefix, u"/", localPath.unify().str());
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
        return nullptr;
}

bool AssetManager::existsAsset(const AssetPath& assetPath) const
{
	//String archiveName;
	//Path path;
	//if (tryParseAssetPath(assetPath, &archiveName, &path)) {
		//if (String::compare(assetPath.scheme(), u"file", CaseSensitivity::CaseInsensitive) == 0) {
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
		//if (String::compare(assetPath.scheme(), u"file", CaseSensitivity::CaseInsensitive) == 0) {
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

Ref<AssetModel> AssetManager::loadAssetModelFromLocalFile(const String& filePath) const
{
    auto assetPath = findAssetPath(filePath);
    if (assetPath) {
        return loadAssetModelFromAssetPath(*assetPath);
    }
    else {
        LN_WARNING(u"Asset not found: " + String(filePath));    // TODO: operator
        return nullptr;
    }
}

Optional<AssetPath> AssetManager::findAssetPath(const StringRef& filePath) const
{
    const Char* ext = AssetModel::AssetFileExtension.c_str();
    return findAssetPath(filePath, &ext, 1);
}

Ref<AssetModel> AssetManager::loadAssetModelFromAssetPath(const AssetPath& assetPath) const
{
    auto stream = openStreamFromAssetPath(assetPath);
    auto json = FileSystem::readAllText(stream);
    auto asset = makeObject<AssetModel>();

    //JsonSerializer::deserialize(json, assetPath.getParentAssetPath().toString(), *asset);
    auto serializer = makeObject<Serializer>(); // TODO: Pool
    JsonTextInputArchive ar(json);
    ar.m_serializer = serializer;
    ar.setBasePath(assetPath.getParentAssetPath().toString());
    ar.load(*asset);


    printf("[Engine] end load\n");

    asset->target()->setAssetPath(assetPath);
    printf("[Engine] end loadAssetModelFromAssetPath\n");

    return asset;
}

void AssetManager::loadAssetModelFromAssetPathToInstance(Object* obj, const AssetPath& assetPath) const
{
    auto stream = openStreamFromAssetPath(assetPath);
    auto text = FileSystem::readAllText(stream);

    auto asset = makeObject<AssetModel>(obj);
    Serializer2::deserializeInstance(asset, text, assetPath.getParentAssetPath().toString());
}

void AssetManager::saveAssetModelToLocalFile(AssetModel* asset, const String& filePath) const
{
    if (LN_REQUIRE(asset)) return;
    if (LN_REQUIRE(!m_primaryLocalAssetDirectory.isEmpty())) return;

    AssetPath assetPath;
    Path localPath;
    if (!filePath.isEmpty()) {
        auto fullPath = Path(m_primaryLocalAssetDirectory, filePath);
        assetPath = AssetPath::makeFromLocalFilePath(fullPath);
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

    String text = Serializer2::serialize(asset, assetPath.getParentAssetPath().toString());
    FileSystem::writeAllText(localPath, text);

    ////auto json = JsonSerializer::serialize(*asset, assetPath.getParentAssetPath().toString(), JsonFormatting::Indented);
    //auto serializer = makeObject<Serializer>(); // TODO: Pool
    //JsonTextOutputArchive ar;
    //ar.m_serializer = serializer;
    //ar.setBasePath(assetPath.getParentAssetPath().toString());
    //ar.save(*asset);
    //auto json = ar.toString(JsonFormatting::Indented);

    //FileSystem::writeAllText(localPath, json);
}

String AssetManager::assetPathToLocalFullPath(const AssetPath& assetPath) const
{
    LN_CHECK(!m_primaryLocalAssetDirectory.isEmpty());
    LN_CHECK(!assetPath.isNull());
    // TODO: assetPath の持ってる host に応じて 親フォルダを変えるべきか。
    return Path(m_primaryLocalAssetDirectory, assetPath.path());
}

//String AssetManager::localFullPathToAssetPath(const String& localFullPath) const
//{
//    // TODO: ちょっと手抜き
//    return AssetPathPrefix + u"/" + localFullPath;
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
//        localPath = u"";
//
//    return AssetPathPrefix + archiveName + u"/" + localPath.str();
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
//        return AssetPathPrefix + archiveName + canonicalizeAssetPath(u"/" + localPath.str() + u"/" + localAssetPath);
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
//    localPath1 = Path(u"/", localPath1);
//    localPath2 = Path(u"/", localPath2);
//    return localPath1.makeRelative(localPath2);
//}

String AssetManager::canonicalizeAssetPath(const String& assetPath)
{
    std::vector<Char> tmpPath(assetPath.length() + 1);
    int len = detail::PathTraits::canonicalizePath(assetPath.c_str(), assetPath.length(), tmpPath.data(), tmpPath.size());
    return Path(StringRef(tmpPath.data(), len)).unify();
}

bool AssetManager::existsFile(const StringRef& filePath) const
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

Ref<Stream> AssetManager::openFileStream(const StringRef& filePath)
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

Ref<ByteBuffer> AssetManager::readAllBytes(const StringRef& filePath)
{
	auto stream = openFileStream(filePath);
	auto buffer = makeRef<ByteBuffer>(stream->length());
	stream->read(buffer->data(), buffer->size());
	return buffer;
}

//Ref<Texture2D> AssetManager::loadTexture(const StringRef& filePath)
//{
//    static const Char* exts[] = {
//        u".png",
//    };
//
//    auto path = findFilePathFromIndex(filePath);
//    if (!path.isEmpty()) {
//       auto asset = makeObject<AssetModel>();
//       JsonSerializer::deserialize(ln::FileSystem::readAllText(path), path.parent(), *asset);
//       return static_cast<Texture2D*>(asset->target());
//    }
//
//	Path sourceFile;
//    Ref<Stream> stream = openFileStreamInternal(filePath, exts, LN_ARRAY_SIZE_OF(exts), &sourceFile);
//
//	// TODO: cache
//
//    // TODO: mipmap
//	auto ref = makeObject<Texture2D>(stream, TextureFormat::RGBA8);
//	ref->setAssetSource(sourceFile);
//	return ref;
//}

Ref<Shader> AssetManager::loadShader(const StringRef& filePath)
{
    static const Char* exts[] = {
        u".lcfx",
        u".fx",
    };

	Path sourceFile;
    auto stream = openFileStreamInternal(filePath, exts, LN_ARRAY_SIZE_OF(exts), &sourceFile);
    if (LN_ENSURE_IO(stream, filePath)) return nullptr;

    // TODO: cache

    auto ref = makeObject<Shader>(Path(filePath).fileNameWithoutExtension(), stream);
    return ref;
}

Ref<Object> AssetManager::loadAsset(const StringRef& filePath)
{
    static const Char* exts[] = {
        u".lnasset",
    };

    Path sourceFile;
    auto stream = openFileStreamInternal(filePath, exts, LN_ARRAY_SIZE_OF(exts), &sourceFile);
    if (LN_ENSURE_IO(stream, filePath)) return nullptr;
    auto text = FileSystem::readAllText(stream);

    auto asset = ln::makeObject<ln::AssetModel>();
    JsonSerializer::deserialize(text, sourceFile.parent(), *asset);
    //asset->onSetAssetFilePath(sourceFile);
    return asset->target();
}

void AssetManager::buildAssetIndexFromLocalFiles(const ln::Path& assetDir)
{
    m_assetIndex.clear();

    // TODO: 量が多くなると重くなるのでインデックス作成スレッド建てたい
    auto assetFiles = ln::FileSystem::getFiles(assetDir, u"*.lnasset", ln::SearchOption::Recursive);
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

bool AssetManager::existsFileInternal(const StringRef& filePath, const Char** exts, int extsCount) const
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

const Path& AssetManager::findFilePathFromIndex(const StringRef& id) const
{
    auto itr = m_assetIndex.find(Uuid(id));
    if (itr != m_assetIndex.end()) {
        return itr->second;
    }
    else {
        return Path::Empty;
    }
}

Ref<Stream> AssetManager::openFileStreamInternal(const StringRef& filePath, const Char** exts, int extsCount, Path* outPath)
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

void AssetManager::makeFindPaths(const StringRef& filePath, const Char** exts, int extsCount, List<Path>* paths) const
{
	const Char* begin = filePath.data();
	const Char* end = filePath.data() + filePath.length();
	if (detail::PathTraits::getExtensionBegin(begin, end, false) != end) {
        // has extension
		paths->add(Path(filePath).unify());
	}
	else {
		for (int i = 0; i < extsCount; i++) {
			paths->add(Path(String(filePath) + exts[i]).unify());   // TODO: operator StringRef + Char*
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

