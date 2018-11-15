
#include "Internal.hpp"
#include <zip.h>
#include <unzip.h>
#include <crypt.h>
#include <zlib.h>
#include "AssetArchive.hpp"

namespace ln {
namespace detail {


//=============================================================================
// CryptedArchiveHelper

const char CryptedArchiveHelper::FileSignature[4] = { 'l', 'c', 'a', 'c' };
const uint16_t CryptedArchiveHelper::FileVersion = 1;
const char CryptedArchiveHelper::FileEntrySignature[4] = { 'l', 'c', '3', '4' };
const char CryptedArchiveHelper::CentralDirectorySignature[4] = { 'l', 'c', '1', '2' };
static const z_crc_t* g_crcTable = nullptr;

void CryptedArchiveHelper::initKeys(const char* password, uint32_t* keys)
{
	g_crcTable = get_crc_table();

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
		register int keyshift = (int)((*(keys + 1)) >> 24);
		(*(keys + 2)) = crc((*(keys + 2)), keyshift);
	}
}

uint32_t CryptedArchiveHelper::crc(uint32_t key, int c)
{
	return ((*(g_crcTable + (((int)(c) ^ (key)) & 0xff))) ^ ((c) >> 8));
}

//=============================================================================
// CryptedAssetArchiveWriter

CryptedAssetArchiveWriter::CryptedAssetArchiveWriter()
{
}

void CryptedAssetArchiveWriter::open(const StringRef& filePath, const StringRef& password)
{
	CryptedArchiveHelper::initKeys(password.toStdString().c_str(), m_keys);

	m_file = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
	m_writer = makeRef<BinaryWriter>(m_file);

	m_writer->write(CryptedArchiveHelper::FileSignature, 4);
	m_writer->writeUInt16(CryptedArchiveHelper::FileVersion);
	m_writer->writeUInt32(CRCHash::compute(password.data(), password.length()));
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

bool CryptedAssetArchiveReader::open(const StringRef& filePath, const StringRef& password, bool pathAsRawRelative)
{
	CryptedArchiveHelper::initKeys(password.toStdString().c_str(), m_keys);

	ln::Path virtualDirFullPath = ln::Path(filePath).canonicalize().replaceExtension(u"");

	m_file = FileStream::create(filePath, FileOpenMode::Read);
	m_reader = makeRef<BinaryReader>(m_file);

	char sig[4];
	if (m_reader->read(sig, 4) != 4 || strncmp(sig, CryptedArchiveHelper::FileSignature, 4) != 0) {
		return false;
	}
	uint16_t fileVersion = m_reader->readUInt16();
	uint32_t passwordHash = m_reader->readUInt32();

	// check password
	if (passwordHash != CRCHash::compute(password.data(), password.length())) {
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
            }
            else {
                ln::Path virtualFullPath = ln::Path(virtualDirFullPath, path).canonicalize().unify();
                m_fileEntries.insert({ virtualFullPath, fe });
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




#define WRITEBUFFERSIZE (16384)

//=============================================================================
// ZipAssetArchiveMaker

ZipAssetArchiveMaker::ZipAssetArchiveMaker()
	: m_zipFile(nullptr)
{
	m_crcBuffer.resize(WRITEBUFFERSIZE);
}

ZipAssetArchiveMaker::~ZipAssetArchiveMaker()
{
	close();
}

bool ZipAssetArchiveMaker::open(const StringRef& filePath, const StringRef& password)
{
	m_zipFile = zipOpen64(filePath.toStdString().c_str(), APPEND_STATUS_CREATE);
	if (!m_zipFile) {
		return false;
	}

	m_password = password.toStdString();

	return true;
}

void ZipAssetArchiveMaker::close()
{
	if (m_zipFile)
	{
		zipClose(m_zipFile, nullptr);
		m_zipFile = nullptr;
	}
}

bool ZipAssetArchiveMaker::addFile(const StringRef& filePath, const StringRef& localPath)
{
	unsigned long fileCRC = 0;

	//getFileCrc(filePath.toStdString().c_str(), m_crcBuffer.data(), m_crcBuffer.size(), &crc);

	zip_fileinfo info = { 0 };
	int result = zipOpenNewFileInZip3(
		m_zipFile, localPath.toStdString().c_str(), &info,
		NULL, 0, NULL, 0, NULL,
		Z_DEFLATED, Z_DEFAULT_COMPRESSION,
		0, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
		//NULL, 0);
		m_password.c_str(), fileCRC);
	if (result != ZIP_OK) {
		return false;
	}

	auto data = FileSystem::readAllBytes(filePath);
	result = zipWriteInFileInZip(m_zipFile, data.data(), data.size());
	if (result != ZIP_OK) {
		zipCloseFileInZip(m_zipFile);
		return false;
	}

	zipCloseFileInZip(m_zipFile);
	return true;
}

int ZipAssetArchiveMaker::getFileCrc(const char* filenameinzip, void* buf, unsigned long size_buf, unsigned long* result_crc)
{
	unsigned long calculate_crc = 0;
	int err = ZIP_OK;
	FILE * fin = fopen64(filenameinzip, "rb");
	unsigned long size_read = 0;
	unsigned long total_read = 0;
	if (fin == NULL)
	{
		err = ZIP_ERRNO;
	}

	if (err == ZIP_OK)
		do
		{
			err = ZIP_OK;
			size_read = (int)fread(buf, 1, size_buf, fin);
			if (size_read < size_buf)
				if (feof(fin) == 0)
				{
					printf("error in reading %s\n", filenameinzip);
					err = ZIP_ERRNO;
				}

			if (size_read > 0)
				calculate_crc = crc32(calculate_crc, (const Bytef*)buf, size_read);
			total_read += size_read;

		} while ((err == ZIP_OK) && (size_read > 0));

		if (fin)
			fclose(fin);

		*result_crc = calculate_crc;
		printf("file %s crc %lx\n", filenameinzip, calculate_crc);
		return err;
}

//=============================================================================
// AssetArchive


//=============================================================================
// ZipAssetArchive

void ZipAssetArchive::open(const StringRef& filePath, const StringRef& password)
{
	m_password = password.toStdString();


	const z_crc_t* pcrc_32_tab;
	unsigned long keys[3];     /* keys defining the pseudo-random sequence */

	if (!m_password.empty())
	{
		int i;
		//pcrc_32_tab = get_crc_table();
		//init_keys(m_password.c_str(), keys, pcrc_32_tab);
		//if (ZSEEK64(s->z_filefunc, s->filestream,
		//	s->pfile_in_zip_read->pos_in_zipfile +
		//	s->pfile_in_zip_read->byte_before_the_zipfile,
		//	SEEK_SET) != 0)
		//	return UNZ_INTERNALERROR;
		//if (ZREAD64(s->z_filefunc, s->filestream, source, 12) < 12)
		//	return UNZ_INTERNALERROR;

		//for (i = 0; i < 12; i++)
		//	zdecode(s->keys, s->pcrc_32_tab, source[i]);

		//s->pfile_in_zip_read->pos_in_zipfile += 12;
		//s->encrypted = 1;
	}




	m_zipFile = unzOpen(filePath.toStdString().c_str());

	int err = unzGoToFirstFile(m_zipFile);
	if (err == UNZ_OK)
	{
		unz_file_info64 fileInfo = { 0 };
		char filePathInZip[256] = { 0 };
		int err = unzGetCurrentFileInfo64(m_zipFile, &fileInfo, filePathInZip, sizeof(filePathInZip), nullptr, 0, nullptr, 0);
		if (LN_ENSURE(err == UNZ_OK)) return;

		//unzOpenCurrentFilePassword(m_zipFile, m_password.c_str());
		unzOpenCurrentFile(m_zipFile);

		auto ofs1 = unzGetOffset64(m_zipFile);
		auto ifs2 = unztell(m_zipFile);
		auto ofs3 = unzGetCurrentFileZStreamPos64(m_zipFile);


		auto fs = FileStream::create(filePath);

		// unzOpenCurrentFile で Entry を開くと、unzGetCurrentFileZStreamPos64() はデータ先頭にある 12 byte の crypt 情報を指している
		auto keyofs = ofs3;// -12;
		fs->seek(keyofs, SeekOrigin::Begin);
		byte_t src[12];
		size_t size2 = fs->read(src, 12);

		// 12 byte の後ろからデータ本体
		ofs3 += 12;


		pcrc_32_tab = get_crc_table();
		init_keys(m_password.c_str(), keys, pcrc_32_tab);
		for (int i = 0; i < 12; i++)
			zdecode(keys, pcrc_32_tab, src[i]);


		fs->seek(ofs3, SeekOrigin::Begin);
		byte_t buf[2048] = { 0 };
		size_t size  = fs->read(buf, fileInfo.compressed_size);

		z_stream zstream = {0};
		int r1 = inflateInit2(&zstream, -MAX_WBITS);
		



		if (1)//(s->encrypted)
		{
			uInt i;
			for (i = 0; i < size; i++)
			{
				buf[i] =
					zdecode(keys, pcrc_32_tab,
						buf[i]);
			}
		}



		byte_t outData[2048] = { 0 };
		/*
		z.next_in = 入力ポインタ;
    z.avail_in = 入力データの残量;
    z.next_out = 出力ポインタ;
    z.avail_out = 出力バッファの残量;
	*/
		zstream.next_in = buf;  /* 入力ポインタを元に戻す */
		zstream.avail_in = fileInfo.compressed_size;
		zstream.next_out = outData;
		zstream.avail_out = 2048;
		//Z_STREAM_END;
		int r2 = inflate(&zstream, Z_NO_FLUSH);
		if (r2 == Z_OK) {

		}
		else if (r2 == Z_DATA_ERROR) {
			// パスワード不一致
		}

		inflateEnd(&zstream);

		printf("");
	}
}

} // namespace detail
} // namespace ln

