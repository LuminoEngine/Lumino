
#pragma once
#include <map>

namespace ln {
namespace detail {


class AssetArchive
{
public:
	virtual Ref<Stream> openFileStream(const StringRef& filePath) = 0;
};


class CryptedArchiveHelper
{
public:
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
	void open(const StringRef& filePath, const StringRef& password);
	void close();
	void addFile(const StringRef& filePath, const StringRef& localPath);

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
	bool open(const StringRef& filePath, const StringRef& password);
	void close();
	size_t read(byte_t* data, size_t count, size_t dataOffset, size_t dataSize, size_t seekPoint);
	virtual Ref<Stream> openFileStream(const StringRef& filePath) override;

private:
	struct FileEntry
	{
		uint32_t dataOffset;
		uint32_t dataSize;
		//String relativePath;
	};

	struct ComparePath {
		bool operator()(const String& a, const String& b) const {
			return Path::compare(a, b) < 0;
		}
	};

	bool checkSignature(BinaryReader* r, const char* sig);
	std::string readString(BinaryReader* r);

	uint32_t m_keys[3];
	Ref<FileStream> m_file;
	Ref<BinaryReader> m_reader;
	std::map<String, FileEntry, ComparePath> m_fileEntries;
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






class ZipAssetArchiveMaker
{
public:
	

	ZipAssetArchiveMaker();
	~ZipAssetArchiveMaker();
	bool open(const StringRef& filePath, const StringRef& password);
	void close();
	bool addFile(const StringRef& filePath, const StringRef& localPath);

private:
	static int getFileCrc(const char* filenameinzip, void* buf, unsigned long size_buf, unsigned long* result_crc);

	void* m_zipFile;	// zipFile pointer
	std::string m_password;
	std::vector<byte_t> m_crcBuffer;
};


class ZipAssetArchive
{
public:
	void open(const StringRef& filePath, const StringRef& password);

private:
	void* m_zipFile;	// zipFile pointer
	std::string m_password;
};

} // namespace detail
} // namespace ln
