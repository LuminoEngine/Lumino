
#include <sys/types.h>
#include <sys/stat.h>
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/Text/Encoding.h>
#include <Lumino/IO/FileStream.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/PathName.h>
#if defined(LN_OS_WIN32)
#include "FileSystem_Win32.h"
#else
#include "FileSystem_Unix.h"
#endif

LN_NAMESPACE_BEGIN
#if 0


#ifdef _WIN32
#else
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
#endif

//------------------------------------------------------------------------------
bool FileSystem::Exists(const char* filePath)
{
	// ※fopen によるチェックはNG。ファイルが排他ロックで開かれていた時に失敗する。
#ifdef _WIN32
	DWORD attr = ::GetFileAttributesA(filePath);
	return ((attr != -1) &&
			(attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
	// http://www.ie.u-ryukyu.ac.jp/~kono/lecture/1999/os/info1/file-2.html
	//struct stat st;
	//int ret = ::stat(file, &st);
	//st.st_mode
	int ret = access(path, F_OK);
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
bool FileSystem::Exists(const wchar_t* filePath)
{
#ifdef _WIN32
	DWORD attr = ::GetFileAttributesW(filePath);
	return ((attr != -1) &&
			(attr & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
	LN_LOCAL_MBCS_FILEPATH(mbcsFilePath, filePath);
	return Exists(mbcsFilePath);
#endif
}

//------------------------------------------------------------------------------
uint32_t FileSystem::GetAttribute(const char* filePath)
{
#ifdef _WIN32
	DWORD attr = ::GetFileAttributesA(filePath);
	LN_THROW(attr != -1, Win32Exception, ::GetLastError());

	uint32_t flags = FileAttribute_Normal;
	if (attr & FILE_ATTRIBUTE_DIRECTORY) flags |= FileAttribute_Directory;
	if (attr & FILE_ATTRIBUTE_READONLY)  flags |= FileAttribute_ReadOnly;
	if (attr & FILE_ATTRIBUTE_HIDDEN)    flags |= FileAttribute_Hidden;
	return flags;
#else
	// Unix 系の場合、ファイルの先頭が . であれば隠しファイルである。
	// mono-master/mono/io-layer/io.c の、_wapi_stat_to_file_attributes が参考になる。
	struct stat st;
	int ret = ::stat(file, &st);
	if (ret == -1) {
		LN_THROW(0, IOException);
	}

	uint32_t attrs = 0;
	if (S_ISDIR(buf->st_mode))
	{
		attrs |= FileAttribute_Directory;
		if (!is_stat_writable()) {
			attrs |= FileAttribute_ReadOnly;
		}
		if (filename[0] == '.') {
			attrs |= FileAttribute_Hidden;
		}
	}
	else
	{
		if (!is_stat_writable()) {
			attrs |= FileAttribute_ReadOnly;
		}
		if (filename[0] == '.') {
			attrs |= FileAttribute_Hidden;
		}
	}
	return attrs;
#endif
}

uint32_t FileSystem::GetAttribute(const wchar_t* filePath)
{
#ifdef _WIN32
	DWORD attr = ::GetFileAttributesW(filePath);
	LN_THROW(attr != -1, Win32Exception, ::GetLastError());

	uint32_t flags = FileAttribute_Normal;
	if (attr & FILE_ATTRIBUTE_DIRECTORY) flags |= FileAttribute_Directory;
	if (attr & FILE_ATTRIBUTE_READONLY)  flags |= FileAttribute_ReadOnly;
	if (attr & FILE_ATTRIBUTE_HIDDEN)    flags |= FileAttribute_Hidden;
	return flags;
#else
	LN_LOCAL_MBCS_FILEPATH(mbcsFilePath, filePath);
	return GetAttribute(mbcsFilePath);
#endif
}

//------------------------------------------------------------------------------
void FileSystem::Copy(const char* sourceFileName, const char* destFileName, bool overwrite)
{
#ifdef _WIN32
	BOOL bRes = ::CopyFileA(sourceFileName, destFileName, (overwrite) ? FALSE : TRUE);
	LN_THROW(bRes, Win32Exception, ::GetLastError());
#else
	// http://ppp-lab.sakura.ne.jp/ProgrammingPlacePlus/c/044.html
	FILE* fpSrc = fopen(sourceFileName, "rb");
	if (fpSrc == NULL){
		LN_THROW(0, IOException);
	}

	FILE* fpDest = fopen(destFileName, "wb");
	if (fpDest == NULL){
		LN_THROW(0, IOException);
	}

	// バイナリデータとして 1byte ずつコピー
	// (windows ではバッファリングが効くけど、それ以外はわからない。
	//  Linux とかで極端に遅くなるようならここでバッファリングも考える)
	while (1)
	{
		byte_t b;
		fread(&b, 1, 1, fpSrc);
		if (feof(fpSrc)){
			break;
		}
		if (ferror(fpSrc)){
			LN_THROW(0, IOException);
		}

		fwrite(&c, sizeof(c), 1, fpDest);
	}

	fclose(fpDest);
	fclose(fpSrc);
	return 0;
#endif
}

void FileSystem::Copy(const wchar_t* sourceFileName, const wchar_t* destFileName, bool overwrite)
{
#ifdef _WIN32
	BOOL bRes = ::CopyFileW(sourceFileName, destFileName, (overwrite) ? FALSE : TRUE);
	LN_THROW(bRes, Win32Exception, ::GetLastError());
#else
	LN_LOCAL_MBCS_FILEPATH(mbcsSrc, sourceFileName);
	LN_LOCAL_MBCS_FILEPATH(mbcsDest, destFileName);
	Copy(mbcsSrc, mbcsDest, overwrite);
#endif
}

//------------------------------------------------------------------------------
void FileSystem::Delete(const char* filePath)
{
#ifdef _WIN32
	BOOL r = ::DeleteFileA(filePath);
	LN_THROW(r != FALSE, Win32Exception, ::GetLastError());
#else
	int ret = remove(path);
	if (ret == -1) LN_THROW(IOException, strerror(errno));
#endif
}
void FileSystem::Delete(const wchar_t* filePath)
{
#ifdef _WIN32
	BOOL r = ::DeleteFileW(filePath);
	LN_THROW(r != FALSE, Win32Exception, ::GetLastError());
#else
	LN_LOCAL_MBCS_FILEPATH(mbcsFilePath, filePath);
	Delete(mbcsFilePath);
#endif
}

//------------------------------------------------------------------------------
size_t FileSystem::GetFileSize(const TCHAR* filePath)
{
	LN_THROW( filePath != NULL, ArgumentException );

	FILE* fp;
	errno_t r = _tfopen_s( &fp, filePath, _T("r") );
	LN_THROW( r == 0, FileNotFoundException );

	size_t size = 0;
	try
	{
		size = GetFileSize(fp);
	}
	catch (...)
	{
		fclose( fp );
		throw;
	}
	fclose( fp );
	return size;
}

//------------------------------------------------------------------------------
size_t FileSystem::GetFileSize( FILE* stream )
{
#if defined(LN_OS_WIN32)
	struct _stat stbuf;
	int handle = _fileno( stream );
	if ( handle == 0 ) {
		return 0;
	}
	if ( _fstat( handle, &stbuf ) == -1 ) {
		return 0;
	}
	return stbuf.st_size;
#else
	struct stat stbuf;
	int handle = fileno( stream );
	if ( handle == 0 ) {
		return 0;
	}
	if (fstat(handle, &stbuf) == -1) {
		return 0;
	}
	return stbuf.st_size;
#endif
}

#endif



template<typename TChar> static bool Exists2(const TChar* filePath);
template<typename TString> static bool Exists2(const TString& filePath);





//------------------------------------------------------------------------------
FileAttribute FileSystem::GetAttribute(const char* filePath)
{
	FileAttribute attr;
	bool r = GetAttributeInternal(filePath, &attr);
	LN_THROW(r, FileNotFoundException, filePath);
	return attr;
}
FileAttribute FileSystem::GetAttribute(const wchar_t* filePath)
{
	FileAttribute attr;
	bool r = GetAttributeInternal(filePath, &attr);
	LN_THROW(r, FileNotFoundException, filePath);
	return attr;
}



//------------------------------------------------------------------------------
template<typename TChar>
void FileSystem::DeleteDirectoryInternal(const GenericStringRef<TChar>& path, bool recursive)
{
	detail::GenericStaticallyLocalPath<TChar> localPath(path);
	if (recursive)
	{
		GenericFileFinder<TChar> finder(localPath.c_str());
		while (!finder.GetCurrent().IsEmpty())
		{
			if (GetAttribute(finder.GetCurrent().c_str()) == FileAttribute::Directory)
			{
				DeleteDirectoryInternal<TChar>(finder.GetCurrent(), recursive);	// recursive
			}
			else // TODO: 他の属性みないとダメ。シンボリックリンクとか
			{
				Delete(finder.GetCurrent().c_str());
			}
			finder.Next();
		}
	}
	RemoveDirectoryImpl(localPath.c_str());
}
template void FileSystem::DeleteDirectoryInternal<char>(const GenericStringRef<char>& path, bool recursive);
template void FileSystem::DeleteDirectoryInternal<wchar_t>(const GenericStringRef<wchar_t>& path, bool recursive);
//------------------------------------------------------------------------------
template<typename TChar>
void FileSystem::CopyDirectoryInternal(const GenericStringRef<TChar>& srcPath, const GenericStringRef<TChar>& destPath, bool overwrite, bool recursive)
{
	if (LN_CHECK_ARG(!srcPath.IsEmpty())) return;
	if (LN_CHECK_ARG(!destPath.IsEmpty())) return;

	// 上書きしないとき、すでにフォルダが存在してはならない
	if (!overwrite)
	{
		LN_THROW(ExistsDirectory(srcPath.GetBegin()), IOException);	// TODO: range
	}

	// コピー先フォルダを作っておく
	FileSystem::CreateDirectory(destPath.GetBegin());	// TODO: range

	GenericFileFinder<TChar> finder(srcPath);
	while (finder.IsWorking())
	{
		const GenericPathName<TChar>& src = finder.GetCurrent();
		GenericPathName<TChar> dest(destPath, src.GetFileName());
		

		if (src.ExistsFile())
		{
			if (dest.ExistsDirectory())
			{
				// src と dest で同名なのに種類が違う。xcopy 的にはファイルを結合してしまうが・・・
				LN_NOTIMPLEMENTED();
			}

			// コピー先にファイルとして存在していて、上書きする場合はコピーする
			if (dest.ExistsFile())
			{
				if (overwrite)
				{
					copy(src, dest, true);
				}
			}
			else
			{
				copy(src, dest, false);
			}
		}
		else if (src.ExistsDirectory())
		{
			if (dest.ExistsFile())
			{
				// src と dest で同名なのに種類が違う。xcopy 的にはファイルを結合してしまうが・・・
				LN_NOTIMPLEMENTED();
			}

			if (recursive)
			{
				CopyDirectoryInternal<TChar>(src, dest, overwrite, recursive);
			}
		}

		finder.Next();
	}
}
template void FileSystem::CopyDirectoryInternal<char>(const GenericStringRef<char>& srcPath, const GenericStringRef<char>& destPath, bool overwrite, bool recursive);
template void FileSystem::CopyDirectoryInternal<wchar_t>(const GenericStringRef<wchar_t>& srcPath, const GenericStringRef<wchar_t>& destPath, bool overwrite, bool recursive);

//------------------------------------------------------------------------------
ByteBuffer FileSystem::ReadAllBytes(const StringRefA& filePath)
{
	detail::GenericStaticallyLocalPath<char> localPath(filePath);
	FILE* fp;
	errno_t err = fopen_s(&fp, localPath.c_str(), "rb");
	LN_THROW(err == 0, FileNotFoundException, localPath.c_str());
	size_t size = (size_t)GetFileSize(fp);

	ByteBuffer buffer(size);
	fread(buffer.getData(), 1, size, fp);
	return buffer;
}
ByteBuffer FileSystem::ReadAllBytes(const StringRefW& filePath)
{
	detail::GenericStaticallyLocalPath<wchar_t> localPath(filePath);
	FILE* fp;
	errno_t err = _wfopen_s(&fp, localPath.c_str(), L"rb");
	LN_THROW(err == 0, FileNotFoundException, localPath.c_str());
	size_t size = (size_t)GetFileSize(fp);

	ByteBuffer buffer(size);
	fread(buffer.getData(), 1, size, fp);
	return buffer;
}

//------------------------------------------------------------------------------
String FileSystem::ReadAllText(const StringRef& filePath, const Encoding* encoding)
{
	ByteBuffer buffer(FileSystem::ReadAllBytes(filePath));
	if (encoding == nullptr)
	{
		Encoding* e = Encoding::GetEncoding(EncodingType::UTF8);
		if (ByteBuffer::compare(buffer, e->GetPreamble(), 3, 3) == 0)
			encoding = e;
		else
			encoding = Encoding::GetUTF8Encoding();
	}

	String str;
	str.ConvertFrom(buffer.getData(), buffer.getSize(), encoding);
	return str;
}

//------------------------------------------------------------------------------
void FileSystem::WriteAllBytes(const TCHAR* filePath, const void* buffer, size_t size)
{
	RefPtr<FileStream> stream = FileStream::Create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
	stream->Write(buffer, size);
}

//------------------------------------------------------------------------------
void FileSystem::WriteAllText(const TCHAR* filePath, const String& str, const Encoding* encoding)
{
	encoding = (encoding == nullptr) ? Encoding::GetUTF8Encoding() : encoding;

	EncodingConversionResult result;
	EncodingConversionOptions options;
	options.NullTerminated = false;
	ByteBuffer buffer = Encoding::Convert(str.c_str(), str.GetByteCount(), Encoding::GetTCharEncoding(), encoding, options, &result);
	
	WriteAllBytes(filePath, buffer.getConstData(), buffer.getSize());
}

//------------------------------------------------------------------------------
int64_t FileSystem::CalcSeekPoint(int64_t curPoint, int64_t maxSize, int64_t offset, int origin)
{
	int64_t newPoint = curPoint;
	switch (origin)
	{
	case SEEK_CUR:
		newPoint += offset;
		break;

	case SEEK_END:
		newPoint = maxSize + offset;
		break;

	default:
		newPoint = offset;
		break;
	}

	if (newPoint < 0) {
		newPoint = 0;
	}
	if (newPoint > maxSize) {
		newPoint = maxSize;
	}
	return newPoint;
}
    
//------------------------------------------------------------------------------
bool FileSystem::ExistsDirectory(const char* path)
{
	FileAttribute attr;
	if (!GetAttributeInternal(path, &attr)) { return false; }
	return attr.TestFlag(FileAttribute::Directory);
}
bool FileSystem::ExistsDirectory(const wchar_t* path)
{
	FileAttribute attr;
	if (!GetAttributeInternal(path, &attr)) { return false; }
	return attr.TestFlag(FileAttribute::Directory);
}

//------------------------------------------------------------------------------
CaseSensitivity FileSystem::GetFileSystemCaseSensitivity()
{
#ifdef LN_OS_WIN32
	return CaseSensitivity::CaseInsensitive;
#else
	return CaseSensitivity::CaseSensitive;
#endif
}

//------------------------------------------------------------------------------
template<typename TChar>
void FileSystem::CreateDirectoryInternal(const TChar* path)
{
	List< GenericString<wchar_t> >	pathList;
	GenericString<wchar_t> dir;

	int i = StringTraits::tcslen(path) - 1;	// 一番後ろの文字の位置
	while (i >= 0)
	{
		dir.AssignCStr(path, 0, i + 1);
		if (FileSystem::ExistsDirectory(dir)) {
			break;
		}
		pathList.Add(dir);

		// セパレータが見つかるまで探す
		while (i > 0 && path[i] != PathTraits::DirectorySeparatorChar && path[i] != PathTraits::AltDirectorySeparatorChar) {
			--i;
		}
		--i;
	}

	if (pathList.IsEmpty()) { return; }	// path が存在している

	for (int i = pathList.GetCount() - 1; i >= 0; --i)
	{
		bool r = FileSystem::mkdir(pathList[i].c_str());
		LN_THROW(r, IOException);
	}
}
#pragma push_macro("CreateDirectory")
#undef CreateDirectory
void FileSystem::CreateDirectory(const char* path)
{
	LN_AFX_FUNCNAME(CreateDirectory)(path);
}
void FileSystem::CreateDirectory(const wchar_t* path)
{
	LN_AFX_FUNCNAME(CreateDirectory)(path);
}
void FileSystem::LN_AFX_FUNCNAME(CreateDirectory)(const char* path)
{
	CreateDirectoryInternal(path);
}
void FileSystem::LN_AFX_FUNCNAME(CreateDirectory)(const wchar_t* path)
{
	CreateDirectoryInternal(path);
}
#pragma pop_macro("CreateDirectory")


LN_NAMESPACE_END
