
#include "Internal.hpp"
#include "../../LuminoCore/src/IO/PathHelper.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include "AssetArchive.hpp"
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
    for (auto& archive : m_archives) {
        archive->close();
    }
    m_archives.clear();
}

void AssetManager::addAssetArchive(const StringRef& filePath, const StringRef& password)
{
    auto archive = makeRef<CryptedAssetArchiveReader>();
    bool result = archive->open(filePath, password, true);
    if (LN_ENSURE(result)) return;
    m_archives.add(archive);
}

bool AssetManager::existsFile(const StringRef& filePath) const
{
    auto unifiedFilePath = Path(filePath).unify();
    for (auto& archive : m_archives) {
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
	for (auto& archive : m_archives) {
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

Ref<Texture2D> AssetManager::loadTexture(const StringRef& filePath)
{
	// TODO: cache

	auto ref = newObject<Texture2D>(filePath, TextureFormat::RGBA32, true);
	return ref;
}

bool AssetManager::existsFileInternal(const StringRef& filePath, const Char** exts, int extsCount) const
{
	List<Path> paths;
	paths.reserve(extsCount);	// TODO: 毎回メモリ確保したくない気がするので、template と lambda なコールバックで処理してみたい
	makeFindPaths(filePath, exts, extsCount, &paths);

	auto unifiedFilePath = Path(filePath).unify();
	for (auto& archive : m_archives) {
		for (auto& path : paths) {
			if (archive->existsFile(path)) {
				return true;
			}
		}
	}

	// TODO: dummy archive

	return FileSystem::existsFile(filePath);
}

Ref<Stream> AssetManager::openFileStreamInternal(const StringRef& filePath, const Char** exts, int extsCount)
{
	List<Path> paths;
	paths.reserve(extsCount);
	makeFindPaths(filePath, exts, extsCount, &paths);

	auto unifiedFilePath = Path(filePath).unify();
	for (auto& archive : m_archives) {
		for (auto& path : paths) {
			auto stream = archive->openFileStream(path);
			if (stream) {
				return stream;
			}
		}
	}

	// TODO: dummy archive

	return FileStream::create(filePath, FileOpenMode::Read);
}

void AssetManager::makeFindPaths(const StringRef& filePath, const Char** exts, int extsCount, List<Path>* paths) const
{
	const Char* begin = filePath.data();
	const Char* end = filePath.data() + filePath.length();
	if (detail::PathTraits::getExtensionBegin(begin, end, false) != end) {
		paths->add(filePath);
	}
	else {
		for (int i = 0; i < extsCount; i++) {
			auto unifiedFilePath = Path(String(filePath) + exts[i]).unify();	// TODO: StringRef + Char*
			paths->add(unifiedFilePath);
		}
	}
}

} // namespace detail
} // namespace ln

