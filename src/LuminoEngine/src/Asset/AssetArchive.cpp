
#include "Internal.hpp"
#include <zip.h>
#include <unzip.h>
#include <crypt.h>
#include <zlib.h>
#include "AssetArchive.hpp"

namespace ln {
namespace detail {

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

