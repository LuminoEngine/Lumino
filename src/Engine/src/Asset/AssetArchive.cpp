
#include "Internal.hpp"
#include "AssetArchive.hpp"
#include "AssetManager.hpp"

namespace ln {
namespace detail {

//=============================================================================
// CryptedArchiveHelper

const String CryptedArchiveHelper::DefaultPassword = u"n7OeL8Hh";
const char CryptedArchiveHelper::FileSignature[4] = { 'l', 'c', 'a', 'c' };
const uint16_t CryptedArchiveHelper::FileVersion = 1;
const char CryptedArchiveHelper::FileEntrySignature[4] = { 'l', 'c', '3', '4' };
const char CryptedArchiveHelper::CentralDirectorySignature[4] = { 'l', 'c', '1', '2' };
extern const unsigned* g_crctab = nullptr;

void CryptedArchiveHelper::initKeys(const char* password, uint32_t* keys)
{
    g_crctab = CRCHashInternal::getCRCTable();

	*(keys + 0) = 305419896L;
	*(keys + 1) = 591751049L;
	*(keys + 2) = 878082192L;
	while (*password != '\0') {
		shiftKey(keys, (int)*password);
		password++;
	}
}

void CryptedArchiveHelper::encrypt128(const uint32_t* keys, byte_t* data)
{
	uint32_t localKeys[3] = { keys[0], keys[1], keys[2] };
	for (int i = 0; i < 128; i++) {
		byte_t t = ccbyte(localKeys);
		shiftKey(localKeys, data[i]);
		data[i] = t ^ data[i];
	}
}

void CryptedArchiveHelper::decrypt128(const uint32_t* keys, byte_t* data)
{
	uint32_t localKeys[3] = { keys[0], keys[1], keys[2] };
	for (int i = 0; i < 128; i++) {
		data[i] ^= ccbyte(localKeys);
		shiftKey(localKeys, data[i]);
	}
}

byte_t CryptedArchiveHelper::ccbyte(const uint32_t* keys)
{
	unsigned t = ((unsigned)(*(keys + 2)) & 0xffff) | 2;
	return (int)(((t * (t ^ 1)) >> 8) & 0xff);
}

void CryptedArchiveHelper::shiftKey(uint32_t* keys, int c)
{
	(*(keys + 0)) = crc((*(keys + 0)), c);
	(*(keys + 1)) += (*(keys + 0)) & 0xff;
	(*(keys + 1)) = (*(keys + 1)) * 134775813L + 1;
	{
		int keyshift = (int)((*(keys + 1)) >> 24);
		(*(keys + 2)) = crc((*(keys + 2)), keyshift);
	}
}

uint32_t CryptedArchiveHelper::crc(uint32_t key, int c)
{
	return ((*(g_crctab + (((int)(c) ^ (key)) & 0xff))) ^ ((c) >> 8));
}

//=============================================================================
// CryptedAssetArchiveWriter

CryptedAssetArchiveWriter::CryptedAssetArchiveWriter()
{
}

void CryptedAssetArchiveWriter::open(const StringRef& filePath, const StringRef& password)
{
    ln::String actualPassword;
    if (password.isEmpty()) {
        actualPassword = CryptedArchiveHelper::DefaultPassword;
    }
    else {
        actualPassword = password;
    }

	CryptedArchiveHelper::initKeys(actualPassword.toStdString().c_str(), m_keys);

	m_file = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
	m_writer = makeRef<BinaryWriter>(m_file);

	m_writer->write(CryptedArchiveHelper::FileSignature, 4);
	m_writer->writeUInt16(CryptedArchiveHelper::FileVersion);
	m_writer->writeUInt32(CRCHash::compute(actualPassword.c_str(), actualPassword.length()));
}

void CryptedAssetArchiveWriter::close()
{
	size_t fileEntriesPos = m_file->position();

	// file entries
	{
		for (int i = 0; i < m_fileEntries.size(); i++)
		{
			const FileEntry& fe = m_fileEntries[i];
			m_writer->write(CryptedArchiveHelper::FileEntrySignature, 4);
			m_writer->writeUInt32(fe.dataOffset);
			m_writer->writeUInt32(fe.dataSize);
			m_writer->writeUInt32(fe.filePath.size());
			m_writer->write(fe.filePath.c_str(), fe.filePath.size());
		}
	}

	// central directory
	{
		m_writer->write(CryptedArchiveHelper::CentralDirectorySignature, 4);
		m_writer->writeUInt32(fileEntriesPos);
	}

	m_writer = nullptr;

	if (m_file) {
		m_file->close();
		m_file = nullptr;
	}
}

void CryptedAssetArchiveWriter::addFile(const StringRef& filePath, const StringRef& localPath)
{
	auto file = FileStream::create(filePath, FileOpenMode::Read);

	byte_t data[128];
	uint32_t offset = m_file->position();
	while (file->position() < file->length())
	{
		size_t size = file->read(data, 128);
		if (size < 128) {
			memset(data + size, 0, 128 - size);	// fill 0.
		}

		CryptedArchiveHelper::encrypt128(m_keys, data);
		m_writer->write(data, 128);
	}

	// TODO: UTF8
	m_fileEntries.add(FileEntry{ offset, (uint32_t)FileSystem::getFileSize(filePath), localPath.toStdString() });
}


//=============================================================================
// CryptedAssetArchiveReader

CryptedAssetArchiveReader::CryptedAssetArchiveReader()
{
}

CryptedAssetArchiveReader::~CryptedAssetArchiveReader()
{
    close();
}

const String& CryptedAssetArchiveReader::scheme() const
{
    return AssetPath::AssetSchemeName;
}

const String& CryptedAssetArchiveReader::name() const
{
    return m_name;
}

bool CryptedAssetArchiveReader::open(const StringRef& filePath, const StringRef& password, bool pathAsRawRelative)
{
    LN_LOG_DEBUG << "Archive: " << filePath;

    ln::String actualPassword;
    if (password.isEmpty()) {
        actualPassword = CryptedArchiveHelper::DefaultPassword;
    }
    else {
        actualPassword = password;
    }

	CryptedArchiveHelper::initKeys(actualPassword.toStdString().c_str(), m_keys);

	ln::Path virtualDirFullPath = ln::Path(filePath).canonicalize().replaceExtension(u"");

    m_name = virtualDirFullPath.fileName();
	m_file = FileStream::create(filePath, FileOpenMode::Read);
	m_reader = makeRef<BinaryReader>(m_file);

	char sig[4];
	if (m_reader->read(sig, 4) != 4 || strncmp(sig, CryptedArchiveHelper::FileSignature, 4) != 0) {
		return false;
	}
	uint16_t fileVersion = m_reader->readUInt16();
	uint32_t passwordHash = m_reader->readUInt32();

	// check password
	if (passwordHash != CRCHash::compute(actualPassword.c_str(), actualPassword.length())) {
		return false;
	}

	// central directory
	size_t fileEntriesPos = 0;
	{
		m_file->seek(m_file->length() - 8, SeekOrigin::Begin);
		if (!checkSignature(m_reader, CryptedArchiveHelper::CentralDirectorySignature)) {
			return false;
		}

		fileEntriesPos = m_reader->readUInt32();
	}

	// file entries
	{
		m_file->seek(fileEntriesPos, SeekOrigin::Begin);
		while (m_file->position() < m_file->length())	// fail safe
		{
			// check signeture
			char sig[4];
			size_t size = m_reader->read(sig, 4);
			if (size != 4) {
				return false;
			}
			if (strncmp(sig, CryptedArchiveHelper::CentralDirectorySignature, 4) == 0) {
				break;
			}
			if (strncmp(sig, CryptedArchiveHelper::FileEntrySignature, 4) != 0) {
				return false;
			}

			FileEntry fe;
			fe.dataOffset = m_reader->readUInt32();
			fe.dataSize = m_reader->readUInt32();

			ln::Path path = ln::Path(String::fromStdString(readString(m_reader)));

            if (pathAsRawRelative) {
                m_fileEntries.insert({ path.unify(), fe });
                LN_LOG_DEBUG << "File: " << path.unify();
            }
            else {
                ln::Path virtualFullPath = ln::Path(virtualDirFullPath, path).canonicalize().unify();
                m_fileEntries.insert({ virtualFullPath, fe });
                LN_LOG_DEBUG << "File: " << virtualFullPath;
            }

		}
	}

	return true;
}

void CryptedAssetArchiveReader::close()
{
	m_reader = nullptr;

	if (m_file) {
		m_file->close();
		m_file = nullptr;
	}
}

size_t CryptedAssetArchiveReader::read(byte_t* data, size_t count, size_t dataOffset, size_t dataSize, size_t seekPoint)
{
	if (seekPoint >= dataSize) {
		return 0;
	}

	std::lock_guard<std::mutex> lock(m_mutex);

	size_t seekHeadOffset = seekPoint % 128;
	size_t seekHead = seekPoint - seekHeadOffset;

	m_file->seek(dataOffset + seekHead, SeekOrigin::Begin);

	byte_t buf[128];
	size_t totalSize = 0;
	while (m_file->position() < m_file->length() && totalSize < count && seekPoint + totalSize < dataSize)
	{
		size_t size = m_file->read(buf, 128);
		if (size < 128) {
			memset(data + size, 0, 128 - size);	// fill 0.
		}

		CryptedArchiveHelper::decrypt128(m_keys, buf);

		size_t remain = dataSize - (seekPoint + totalSize);
		size_t writeSize = std::min(size - seekHeadOffset, count - totalSize);
		writeSize = std::min(writeSize, remain);


		memcpy(data, buf + seekHeadOffset, writeSize);
		data += writeSize;
		totalSize += writeSize;
		seekHeadOffset = 0;
	}

	return totalSize;
}

bool CryptedAssetArchiveReader::existsFile(const Path& unifiedFilePath) const
{
    return m_fileEntries.find(unifiedFilePath) != m_fileEntries.end();
}

Ref<Stream> CryptedAssetArchiveReader::openFileStream(const Path& unifiedFilePath)
{
	auto itr = m_fileEntries.find(unifiedFilePath);
	if (itr != m_fileEntries.end()) {
		return makeRef<CryptedArchiveFileStream>(this, itr->second.dataOffset, itr->second.dataSize);
	}
	else {
		return nullptr;
	}
}

bool CryptedAssetArchiveReader::checkSignature(BinaryReader* r, const char* sig)
{
	char buf[8];
	size_t size = r->read(buf, 4);
	if (size != 4 || strncmp(buf, sig, 4) != 0) {
		return false;
	}
	return true;
}

std::string CryptedAssetArchiveReader::readString(BinaryReader* r)
{
	uint32_t len = r->readUInt32();
	if (len == 0) {
		return std::string();
	}
	else if (len <= 255) {	// min str optimaize
		char buf[255] = { 0 };
		r->read(buf, len);
		return std::string(buf, len);
	}
	else {
		std::vector<char> buf;
		buf.resize(len);
		r->read(buf.data(), len);
		return std::string(buf.begin(), buf.end());
	}
}


//=============================================================================
// CryptedArchiveFileStream

CryptedArchiveFileStream::CryptedArchiveFileStream(CryptedAssetArchiveReader* archive, size_t dataOffset, size_t dataSize)
	: m_archive(archive)
	, m_dataOffset(dataOffset)
	, m_dataSize(dataSize)
	, m_position(0)
{
}

bool CryptedArchiveFileStream::canRead() const
{
	return true;
}

bool CryptedArchiveFileStream::canWrite() const
{
	return false;
}

int64_t CryptedArchiveFileStream::length() const
{
	return m_dataSize;
}

int64_t CryptedArchiveFileStream::position() const
{
	return m_position;
}

size_t CryptedArchiveFileStream::read(void* buffer, size_t byteCount)
{
	size_t size = m_archive->read((byte_t*)buffer, byteCount, m_dataOffset, m_dataSize, m_position);
	m_position += size;
	return size;
}

void CryptedArchiveFileStream::write(const void* data, size_t byteCount)
{
	LN_UNREACHABLE();
}

void CryptedArchiveFileStream::seek(int64_t offset, SeekOrigin origin)
{
	m_position = detail::FileSystemInternal::calcSeekPoint(m_position, m_dataSize, offset, (int)origin);
}

void CryptedArchiveFileStream::flush()
{
}


//=============================================================================
// FileSystemReader

FileSystemReader::FileSystemReader()
{
    m_scheme = AssetPath::FileSchemeName;
}

void FileSystemReader::setRootPath(const StringRef& path)
{
	m_rootPath = Path(path);
    m_name = m_rootPath.fileName();
    m_scheme = AssetPath::AssetSchemeName;
}

const String& FileSystemReader::scheme() const
{
    return m_scheme;
}

const String& FileSystemReader::name() const
{
    return m_name;
}

void FileSystemReader::close()
{
}

bool FileSystemReader::existsFile(const Path& unifiedFilePath) const
{
	return FileSystem::existsFile(Path(m_rootPath, unifiedFilePath));
}

Ref<Stream> FileSystemReader::openFileStream(const Path& unifiedFilePath)
{
	if (FileSystem::existsFile(Path(m_rootPath, unifiedFilePath))) {
		return FileStream::create(Path(m_rootPath, unifiedFilePath), FileOpenMode::Read);
	}
	else {
		return nullptr;
	}
}

AssetArchiveStorageKind FileSystemReader::storageKind() const
{
    if (m_name.isEmpty())
        return AssetArchiveStorageKind::LocalDirectory;
    else
        return AssetArchiveStorageKind::AssetDirectory;
}

} // namespace detail
} // namespace ln

