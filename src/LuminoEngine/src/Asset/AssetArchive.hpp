
#pragma once
#include <map>

namespace ln {
namespace detail {


class AssetArchive
{
public:
};

class CryptedArchiveHelper
{
public:
	static const char FileSignature[4];
	static const uint16_t FileVersion;
	static const char FileEntrySignature[4];
	static const char CentralDirectorySignature[4];

	
	static void initKeys128(const char* password, byte_t* keys);
	static void encrypt128(const byte_t* keys, byte_t* data);
	static void decrypt128(const byte_t* keys, byte_t* data);
	static byte_t ccbyte(const byte_t* keys, int index);
	static void shiftKey128(byte_t* keys, int c);
	static uint32_t crc(byte_t b);
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

	byte_t m_keys[128];
	Ref<FileStream> m_file;
	Ref<BinaryWriter> m_writer;
	List<FileEntry> m_fileEntries;
};

class CryptedAssetArchiveReader
{
public:
	CryptedAssetArchiveReader();
	bool open(const StringRef& filePath, const StringRef& password);
	void close();

private:
	struct FileEntry
	{
		uint32_t dataOffset;
		uint32_t dataSize;
		//String relativePath;
	};

	struct ComparePath {
		bool operator()(const String& a, const String& b) const {
			return Path::compare(a, b) == 0;
		}
	};

	bool checkSignature(BinaryReader* r, const char* sig);
	std::string readString(BinaryReader* r);

	std::string m_password;
	Ref<FileStream> m_file;
	Ref<BinaryReader> m_reader;
	std::map<String, FileEntry, ComparePath> m_fileEntries;
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
