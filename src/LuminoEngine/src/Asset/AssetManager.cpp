
#include "Internal.hpp"
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

Ref<Stream> AssetManager::openFileStream(const StringRef& filePath)
{
	auto unifiedFilePath = Path(filePath).unify();
	for (auto& archive : m_archives) {
		auto stream = archive->openFileStream(unifiedFilePath);
		if (stream) {
			return stream;
		}
	}

    // TODO: archive

    return FileStream::create(filePath, FileOpenMode::Read);
}

} // namespace detail
} // namespace ln

