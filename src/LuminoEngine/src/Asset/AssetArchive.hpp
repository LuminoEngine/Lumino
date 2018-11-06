
#pragma once

namespace ln {
namespace detail {

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

class AssetArchive
{
public:
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
