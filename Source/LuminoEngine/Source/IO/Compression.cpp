
#pragma once
#include <fstream>
#include "../Internal.h"
#include <zlib-1.2.7/contrib/minizip/unzip.h>
#include <Lumino/IO/Compression.h>

LN_NAMESPACE_BEGIN

void ZipFile::ExtractToDirectory(const Path& sourceArchiveFilePath, const Path& destinationDirectoryPath)
{
	// https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT
	// https://hg.python.org/cpython/file/2.7/Lib/zipfile.py#l1046

	std::string srcPath = sourceArchiveFilePath.getString().toStdString();
	std::string dstPath = destinationDirectoryPath.getString().toStdString() + "/";
	std::string m_password;

	static const size_t WRITEBUFFERSIZE = 8192;
	std::vector<char> buffer;
	buffer.resize(WRITEBUFFERSIZE);


	unzFile m_zf = unzOpen(srcPath.c_str());
	if (LN_ENSURE(m_zf)) return;

	int err = unzGoToFirstFile(m_zf);
	if (err == UNZ_OK)
	{
		do
		{
			unz_file_info64 file_info = { 0 };
			char filename_inzip[256] = { 0 };

			int err = unzGetCurrentFileInfo64(m_zf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
			//if (UNZ_OK != err)
			//	throw EXCEPTION_CLASS("Error, couln't get the current entry info");

			//if ( == '')




			if (filename_inzip[file_info.size_filename - 1] == '/')
			{
				std::string path = dstPath + filename_inzip;
				detail::FileSystemInternal::createDirectory(path.c_str(), path.length());
			}
			else
			{
				std::string path = dstPath + filename_inzip;
				std::ofstream output_file(path, std::ofstream::binary);
				//FileStream::create(Path())
				if (output_file.good())
				{
					//err = unzOpenCurrentFilePassword(m_zf, m_password.c_str());
					err = unzOpenCurrentFile(m_zf);

					do
					{
						err = unzReadCurrentFile(m_zf, buffer.data(), (unsigned int)buffer.size());
						if (err < 0 || err == 0)
							break;

						output_file.write(buffer.data(), err);
						if (!output_file.good())
						{
							err = UNZ_ERRNO;
							break;
						}

					} while (err > 0);


					output_file.flush();

					err = unzCloseCurrentFile(m_zf);
				}
			}

			printf("%s\n", filename_inzip);

		} while (unzGoToNextFile(m_zf) != UNZ_END_OF_LIST_OF_FILE);


	}

	unzClose(m_zf);

}

LN_NAMESPACE_END
