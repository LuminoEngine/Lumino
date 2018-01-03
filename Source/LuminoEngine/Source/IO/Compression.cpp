
#pragma once
#include <fstream>
#include "../Internal.h"
#include <zlib-1.2.7/contrib/minizip/unzip.h>
#include <Lumino/Text/Encoding.h>
#include <Lumino/IO/Compression.h>

LN_NAMESPACE_BEGIN
namespace detail {

class ExtractZip
{
public:
	ExtractZip()
		: m_zipFile(nullptr)
	{
	}

	~ExtractZip()
	{
		if (m_zipFile)
		{
			unzClose(m_zipFile);
		}
	}

	void init(
		const Path& sourceArchiveFilePath,
		const Path& destinationDirectoryPath)
	{
		m_destinationDirectoryPath = destinationDirectoryPath;
		std::string srcPath = sourceArchiveFilePath.getString().toStdString();
		m_zipFile = unzOpen(srcPath.c_str());
		if (LN_ENSURE(m_zipFile)) return;
	}

	void extract()
	{
		// https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT
		// https://hg.python.org/cpython/file/2.7/Lib/zipfile.py#l1046

		static const size_t TempBufferSize = 8192;
		std::vector<char> buffer;
		buffer.resize(TempBufferSize);

		FileSystem::createDirectory(m_destinationDirectoryPath);

		int err = unzGoToFirstFile(m_zipFile);
		if (err == UNZ_OK)
		{
			do
			{
				unz_file_info64 fileInfo = { 0 };
				char filePathInZip[256] = { 0 };

				int err = unzGetCurrentFileInfo64(m_zipFile, &fileInfo, filePathInZip, sizeof(filePathInZip), nullptr, 0, nullptr, 0);
				if (LN_ENSURE(err == UNZ_OK)) return;

				Encoding* pathEncoding = Encoding::detectEncodingSimple(filePathInZip, fileInfo.size_filename, false);
				String filePath = String::fromCString(filePathInZip, fileInfo.size_filename, pathEncoding);

				// Directory?
				if (filePathInZip[fileInfo.size_filename - 1] == '/' ||
					filePathInZip[fileInfo.size_filename - 1] == '\\')
				{
					Path path = Path(m_destinationDirectoryPath, filePath);
					FileSystem::createDirectory(path);
				}
				// File?
				else
				{
					//err = unzOpenCurrentFilePassword(m_zf, m_password.c_str());
					err = unzOpenCurrentFile(m_zipFile);
					if (LN_ENSURE(err == UNZ_OK, "unzOpenCurrentFile (%d)", err)) return;

					Path path = Path(m_destinationDirectoryPath, filePath);

					// ディレクトリのエンティティが含まれていないこともあるのでフォルダ作成を試行する
					FileSystem::createDirectory(path.getParent());

					auto outputFile = FileStream::create(path.c_str(), FileOpenMode::write | FileOpenMode::Truncate);

					do
					{
						err = unzReadCurrentFile(m_zipFile, buffer.data(), (unsigned int)buffer.size());
						if (err < 0 || err == 0)
						{
							break;
						}

						outputFile->write(buffer.data(), err);

					} while (err > 0);

					outputFile->flush();

					err = unzCloseCurrentFile(m_zipFile);
				}

			} while (unzGoToNextFile(m_zipFile) != UNZ_END_OF_LIST_OF_FILE);
		}
	}

private:
	unzFile m_zipFile;
	Path m_destinationDirectoryPath;
};

} // namespace detail

void ZipFile::extractToDirectory(const Path& sourceArchiveFilePath, const Path& destinationDirectoryPath)
{
	detail::ExtractZip ez;
	ez.init(sourceArchiveFilePath, destinationDirectoryPath);
	ez.extract();
}

LN_NAMESPACE_END
