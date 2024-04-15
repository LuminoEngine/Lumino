
#pragma once
#include <map>

namespace ln {
namespace detail {

enum class AssetArchiveStorageKind
{
	ArchiveFile,
    AssetDirectory,
    LocalDirectory,
};

class AssetArchive
    : public RefObject
{
public:
    virtual const String& scheme() const = 0;
    virtual const String& name() const = 0;
    virtual void close() = 0;
    virtual bool existsFile(const Path& unifiedFilePath) const = 0;
	virtual Ref<Stream> openFileStream(const Path& unifiedFilePath) = 0;
	virtual AssetArchiveStorageKind storageKind() const = 0;
};


class CryptedArchiveHelper
{
public:
    static const String DefaultPassword;

	static const char FileSignature[4];
	static const uint16_t FileVersion;
	static const char FileEntrySignature[4];
	static const char CentralDirectorySignature[4];

	static void initKeys(const char* password, uint32_t* keys);
	static void encrypt128(const uint32_t* keys, byte_t* data);
	static void decrypt128(const uint32_t* keys, byte_t* data);
	static byte_t ccbyte(const uint32_t* keys);
	static void shiftKey(uint32_t* keys, int c);
	static uint32_t crc(uint32_t key, int c);
};

class CryptedAssetArchiveWriter
{
public:
	CryptedAssetArchiveWriter();
	void open(const StringView& filePath, const StringView& password);
	void close();
	void addFile(const StringView& filePath, const StringView& localPath);

private:
	struct FileEntry
	{
		uint32_t dataOffset;
		uint32_t dataSize;
		std::string filePath;
	};

	uint32_t m_keys[3];
	Ref<FileStream> m_file;
	Ref<BinaryWriter> m_writer;
	List<FileEntry> m_fileEntries;
};

class CryptedAssetArchiveReader
	: public AssetArchive
{
public:
	CryptedAssetArchiveReader();
    ~CryptedAssetArchiveReader();
	bool open(const StringView& filePath, const StringView& password, bool pathAsRawRelative);
    virtual const String& scheme() const;
    virtual const String& name() const;
    virtual void close() override;
	size_t read(byte_t* data, size_t count, size_t dataOffset, size_t dataSize, size_t seekPoint);
    virtual bool existsFile(const Path& unifiedFilePath) const override;
	virtual Ref<Stream> openFileStream(const Path& unifiedFilePath) override;
	virtual AssetArchiveStorageKind storageKind() const override { return AssetArchiveStorageKind::ArchiveFile; }

private:
	struct FileEntry
	{
		uint32_t dataOffset;
		uint32_t dataSize;
	};

	struct ComparePath {
		bool operator()(const Path& a, const Path& b) const {
			return Path::compare(a, b) < 0;
		}
	};

	bool checkSignature(BinaryReader* r, const char* sig);
	std::string readString(BinaryReader* r);

    String m_name;
	uint32_t m_keys[3];
	Ref<FileStream> m_file;
	Ref<BinaryReader> m_reader;
	std::map<Path, FileEntry, ComparePath> m_fileEntries;
	std::mutex m_mutex;
};

class CryptedArchiveFileStream
	: public Stream
{
public:
	CryptedArchiveFileStream(CryptedAssetArchiveReader* archive, size_t dataOffset, size_t dataSize);

	virtual bool canRead() const override;
	virtual bool canWrite() const override;
	virtual int64_t length() const override;
	virtual int64_t position() const override;
	virtual size_t read(void* buffer, size_t byteCount) override;
	virtual void write(const void* data, size_t byteCount) override;
	virtual void seek(int64_t offset, SeekOrigin origin) override;
	virtual void flush() override;

private:
	CryptedAssetArchiveReader* m_archive;
	size_t m_dataOffset;
	size_t m_dataSize;
	size_t m_position;
};

class FileSystemReader
	: public AssetArchive
{
public:
	FileSystemReader();
	void setRootPath(const StringView& path);
	const Path& rootPath() const { return m_rootPath; }
    virtual const String& scheme() const;
    virtual const String& name() const;
	virtual void close() override;
	virtual bool existsFile(const Path& unifiedFilePath) const override;
	virtual Ref<Stream> openFileStream(const Path& unifiedFilePath) override;
    virtual AssetArchiveStorageKind storageKind() const override;

private:
	Path m_rootPath;
    String m_scheme;
    String m_name;
};

} // namespace detail
} // namespace ln
