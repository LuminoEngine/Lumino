
#include <sys/types.h>
#include <sys/stat.h>
#include "../Internal.h"
#include "../../include/Lumino/Base/ByteBuffer.h"
#include "../../include/Lumino/Text/Encoding.h"
#include "../../include/Lumino/IO/FileStream.h"
#include "../../include/Lumino/IO/FileSystem.h"
#include "../../include/Lumino/IO/PathTraits.h"

LN_NAMESPACE_BEGIN

#define MBCS_FILEPATH(mbcsPath, srcWPath) \
	char mbcsPath[LN_MAX_PATH + 1]; \
	if (wcstombs(mbcsPath, srcWPath, LN_MAX_PATH) < 0) { \
		LN_THROW(0, IOException); \
	}

//------------------------------------------------------------------------------
static bool is_stat_writable(struct stat *st, const char *path)
{
	// 制限なしに書き込み可であるか
	if (st->st_mode & S_IWOTH)
		return 1;
	// 現在のユーザーIDに許可されているか
	if ((st->st_uid == geteuid()) && (st->st_mode & S_IWUSR))
		return 1;
	// 現在のグループIDに許可されているか
	if ((st->st_gid == getegid()) && (st->st_mode & S_IWGRP))
		return 1;
	// もうここに来ることはほとんどないはずだが念のため
	return access(path, W_OK) == 0;
}

//------------------------------------------------------------------------------
bool FileSystem::ExistsFile(const StringRefA& filePath)
{
	//if (filePath == NULL) {
	//	return false;
	//}
	PathNameA path(filePath);

	// ※fopen によるチェックはNG。ファイルが排他ロックで開かれていた時に失敗する。
	// ※access によるチェックもNG。ディレクトリも考慮してしまう。
	struct stat st;
	int ret = ::stat(path.c_str(), &st);
	if (ret == 0)
	{
		if (S_ISDIR(st.st_mode)) {
			return false;		// ディレクトリだった。
		}
		return true;
	}
	return false;

#if 0
	// http://www.ie.u-ryukyu.ac.jp/?kono/lecture/1999/os/info1/file-2.html
	//struct stat st;
	//int ret = ::stat(file, &st);
	//
	int ret = access(filePath, F_OK);
	if (ret == -1) {
		if (errno == ENOENT) {
			return false;
		}
		else {
			// パスが長い、メモリが足りない等理由は様々。
			// http://linuxjm.sourceforge.jp/html/LDP_man-pages/man2/faccessat.2.html
			LN_THROW(0, IOException, strerror(errno));
		}
	}
	return true;
#endif
}
bool FileSystem::ExistsFile(const StringRefW& filePath)
{
	if (filePath.IsNullOrEmpty()) {
		return false;
	}
	MBCS_FILEPATH(mbcsFilePath, filePath.GetBegin());	// TODO: GetBegin
	return ExistsFile(mbcsFilePath);
}

//------------------------------------------------------------------------------
void FileSystem::SetAttribute(const char* filePath, uint32_t attrs)
{
	struct stat st;
	int ret = ::stat(filePath, &st);
	LN_THROW(ret != -1, IOException);

	// 変更できるのは読み取り属性だけ。
	// 隠し属性は Unix ではファイル名で表現する。
	if (attrs & FileAttribute::ReadOnly) {
		ret = ::chmod(filePath, st.st_mode & ~(S_IWUSR | S_IWOTH | S_IWGRP));
	}
	else {
		ret = ::chmod(filePath, st.st_mode | S_IWUSR);
	}
	LN_THROW(ret != -1, IOException);
}
void FileSystem::SetAttribute(const wchar_t* filePath, uint32_t attrs)
{
	MBCS_FILEPATH(mbcsFilePath, filePath);
	return SetAttribute(mbcsFilePath, attrs);
}

//------------------------------------------------------------------------------
void FileSystem::Copy(const char* sourceFileName, const char* destFileName, bool overwrite)
{
	// コピー先ファイルの存在確認
	if (!overwrite && ExistsFile(destFileName)) {
		LN_THROW(0, IOException);
	}

	// http://ppp-lab.sakura.ne.jp/ProgrammingPlacePlus/c/044.html
	FILE* fpSrc = fopen(sourceFileName, "rb");
	if (fpSrc == NULL) {
		LN_THROW(0, IOException);
	}

	FILE* fpDest = fopen(destFileName, "wb");
	if (fpDest == NULL) {
		LN_THROW(0, IOException);
	}

	// バイナリデータとして 1byte ずつコピー
	// (windows ではバッファリングが効くけど、それ以外はわからない。
	//  Linux とかで極端に遅くなるようならここでバッファリングも考える)
	while (1)
	{
		byte_t b;
		fread(&b, 1, 1, fpSrc);
		if (feof(fpSrc)) {
			break;
		}
		if (ferror(fpSrc)) {
			LN_THROW(0, IOException);
		}

		fwrite(&b, sizeof(b), 1, fpDest);
	}

	fclose(fpDest);
	fclose(fpSrc);
}

void FileSystem::Copy(const wchar_t* sourceFileName, const wchar_t* destFileName, bool overwrite)
{
	MBCS_FILEPATH(mbcsSrc, sourceFileName);
	MBCS_FILEPATH(mbcsDest, destFileName);
	Copy(mbcsSrc, mbcsDest, overwrite);
}

//------------------------------------------------------------------------------
void FileSystem::Delete(const char* filePath)
{
	int ret = remove(filePath);
	if (ret == -1) LN_THROW(0, IOException, strerror(errno));
}
void FileSystem::Delete(const wchar_t* filePath)
{
	MBCS_FILEPATH(mbcsFilePath, filePath);
	Delete(mbcsFilePath);
}

//------------------------------------------------------------------------------
uint64_t FileSystem::GetFileSize(const TCHAR* filePath)
{
	LN_CHECK_ARG(filePath != nullptr);
	detail::GenericStaticallyLocalPath<char> localPath(filePath);
	struct stat stat_buf;
	int r = stat(localPath.c_str(), &stat_buf);
	LN_THROW(r == 0, FileNotFoundException);
	return stat_buf.st_size;
}

//------------------------------------------------------------------------------
uint64_t FileSystem::GetFileSize(FILE* stream)
{
	struct stat stbuf;
	int handle = fileno(stream);
	if (handle == 0) {
		return 0;
	}
	if (fstat(handle, &stbuf) == -1) {
		return 0;
	}
	return stbuf.st_size;
}
//------------------------------------------------------------------------------
bool FileSystem::mkdir(const char* path)
{
	int ret = ::mkdir(path, 0755);
	return ret != -1;
}
bool FileSystem::mkdir(const wchar_t* path)
{
	MBCS_FILEPATH(mbcsFilePath, path);
	return mkdir(mbcsFilePath);
}

//------------------------------------------------------------------------------
bool FileSystem::GetAttributeInternal(const char* path, FileAttribute* outAttr)
{
	// Unix 系の場合、ファイルの先頭が . であれば隠しファイルである。
	// mono-master/mono/io-layer/io.c の、_wapi_stat_to_file_attributes が参考になる。
	struct stat st;
	int ret = ::stat(path, &st);
	if (ret == -1) {
		return false;
	}

	const char* fileName = PathTraits::GetFileNameSub(path);
	FileAttribute attrs;
	if (S_ISDIR(st.st_mode))
	{
		attrs |= FileAttribute::Directory;
		if (!is_stat_writable(&st, path)) {
			attrs |= FileAttribute::ReadOnly;
		}
		if (fileName[0] == '.') {
			attrs |= FileAttribute::Hidden;
		}
	}
	else
	{
		if (!is_stat_writable(&st, path)) {
			attrs |= FileAttribute::ReadOnly;
		}
		if (fileName[0] == '.') {
			attrs |= FileAttribute::Hidden;
		}
	}
	*outAttr = attrs;
	return true;
}
bool FileSystem::GetAttributeInternal(const wchar_t* path, FileAttribute* outAttr)
{
	MBCS_FILEPATH(mbcsFilePath, path);
	return GetAttributeInternal(mbcsFilePath, outAttr);
}

//------------------------------------------------------------------------------
static void RemoveDirectoryImpl(const char* path)
{
	int r = rmdir(path);
	LN_THROW(r == 0, IOException);
}
static void RemoveDirectoryImpl(const wchar_t* path)
{
	detail::GenericStaticallyLocalPath<char> localPath(path);
	RemoveDirectoryImpl(localPath.c_str());
}

LN_NAMESPACE_END
