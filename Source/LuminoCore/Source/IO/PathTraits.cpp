
#include "../Internal.h"
#include "../../Include/Lumino/Base/String.h"
#include "../../Include/Lumino/IO/PathTraits.h"
#include "../../Include/Lumino/IO/DirectoryUtils.h"
#include <Lumino/IO/FileSystem.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// PathTraits
//==============================================================================

// 見つからなかったら return begin;
template<typename TChar, typename TPred>
static const TChar* findLast(const TChar* begin, const TChar* end, TPred pred)
{
	const TChar* pos = end - 1;
	for (; pos >= begin; --pos)
	{
		if (pred(*pos))
		{
			return pos/* + 1*/;
		}
	}
	return begin;
}

template<typename TChar>
const TChar* PathTraits::getFileName(const TChar* begin, const TChar* end)
{
	const TChar* pos = findLast(begin, end, [](TChar ch) { return isSeparatorChar(ch); });
	if (isSeparatorChar(*pos)) return pos + 1;
	return begin;
}
template const char* PathTraits::getFileName(const char* begin, const char* end);
template const wchar_t* PathTraits::getFileName(const wchar_t* begin, const wchar_t* end);
template const char16_t* PathTraits::getFileName(const char16_t* begin, const char16_t* end);

template<typename TChar>
const TChar* PathTraits::getWithoutExtensionEnd(const TChar* begin, const TChar* end)
{
	const TChar* pos = findLast(begin, end, [](TChar ch) { return isSeparatorChar(ch) || ch == '.'; });
	if (*pos == '.') return pos;
	return end;
}
template const char* PathTraits::getWithoutExtensionEnd(const char* begin, const char* end);
template const wchar_t* PathTraits::getWithoutExtensionEnd(const wchar_t* begin, const wchar_t* end);
template const char16_t* PathTraits::getWithoutExtensionEnd(const char16_t* begin, const char16_t* end);

template<typename TChar>
const TChar* PathTraits::getExtensionBegin(const TChar* begin, const TChar* end, bool withDot)
{
	const TChar* pos = findLast(begin, end, [](TChar ch) { return isSeparatorChar(ch) || ch == '.'; });
	if (*pos == '.')
	{
		if (withDot)
		{
			if (end - pos == 1)	// .NET implementation
				return end;
			else
				return pos;
		}
		else
			return pos + 1;
	}
	return end;
}
template const char* PathTraits::getExtensionBegin(const char* begin, const char* end, bool withDot);
template const wchar_t* PathTraits::getExtensionBegin(const wchar_t* begin, const wchar_t* end, bool withDot);
template const char16_t* PathTraits::getExtensionBegin(const char16_t* begin, const char16_t* end, bool withDot);

template<typename TChar>
const TChar* PathTraits::getDirectoryPathEnd(const TChar* begin, const TChar* end)
{
	/* 参考：他のライブラリの、空文字やセパレータが無いなどで親ディレクトリが取れない時の動作
	"C:"	"C:/"	"C:/file"
	- Qt (QFileInfo)			…	"C:"	"C:/"	"C:/"
	- wxWidgets (wxFileName)	…
	- Python (os.path)			…
	- Ruby (Pathname)			…	"C:.."	"C:/"	"C:/"
	- Java (os.nio.Paths)		…
	- C# (Path, Uri)			…	""		"C:/"	"C:/"

	出力例
	- "C:\MyDir\MySubDir\myfile.ext" → "C:\MyDir\MySubDir"
	- "C:\MyDir\MySubDir" → "C:\MyDir"
	- "C:\MyDir\" → "C:\MyDir"
	- "C:\MyDir" → "C:\"
	- "C:\" → ""
	- "/MyDir/" → "/MyDir"
	- "/MyDir" → "/"
	- "/" → "
	*/

	// 後ろから前に調べて、最初に \\ か / が見つかるところを探す
	const TChar* pos = findLast(begin, end, [](TChar ch) { return isSeparatorChar(ch); });
	return pos;
}
template const char* PathTraits::getDirectoryPathEnd(const char* begin, const char* end);
template const wchar_t* PathTraits::getDirectoryPathEnd(const wchar_t* begin, const wchar_t* end);
template const char16_t* PathTraits::getDirectoryPathEnd(const char16_t* begin, const char16_t* end);


//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::isSeparatorChar(TChar ch)
{
#ifdef LN_OS_WIN32
	return (ch == '\\' || ch == '/');
#else
	return (ch == '/');
#endif
}
template bool PathTraits::isSeparatorChar<char>(char ch);
template bool PathTraits::isSeparatorChar<wchar_t>(wchar_t ch);

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::isVolumeSeparatorChar(TChar ch)
{
#ifdef LN_OS_WIN32
	return (ch == ':');
#else
	return false;
#endif
}
template bool PathTraits::isVolumeSeparatorChar<char>(char ch);
template bool PathTraits::isVolumeSeparatorChar<wchar_t>(wchar_t ch);

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::isRootPath(const TChar* path)
{
#ifdef LN_OS_WIN32
	// windows の場合
	size_t len = StringTraits::tcslen(path);
	if (isAbsolutePath(path) && len >= 2)
	{
		if (path[len - 1] == VolumeSeparatorChar) {
			return true;	// 末尾が : である → "C:" 等
		}
		if (path[len - 2] == VolumeSeparatorChar &&
			(path[len - 1] == DirectorySeparatorChar || path[len - 1] == AltDirectorySeparatorChar))
		{
			return true;	// 末尾が \ か / で、その前が : である → "C:/" 等
		}
	}
	return false;
#else
	// UNIX の場合、/ だけであるか
	size_t len = StringTraits::tcslen(path);
	if (len == 1 && path[0] == '/') {
		return true;
	}
	return false;
#endif
}
template bool PathTraits::isRootPath<char>(const char* path);
template bool PathTraits::isRootPath<wchar_t>(const wchar_t* path);

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::isAbsolutePath(const TChar* path, int len)
{
	if (LN_REQUIRE(path != NULL)) return false;
	if (len < 0) {
		len = INT_MAX;
	}

	// UNIX ルートパス
	if (path[0] == '/') return true;

	// Windows ドライブレター (: の次が / であるか)
	for (; *path; ++path) {
		if (path[0] == ':') {
			return true;
			//if (path[1] == '/' || path[1] == '\\') {
			//	return true;
			//}
		}

		--len;
		if (len == 0) {
			break;
		}
	}
	return false;
}
template bool PathTraits::isAbsolutePath<char>(const char* path, int len);
template bool PathTraits::isAbsolutePath<wchar_t>(const wchar_t* path, int len);
template bool PathTraits::isAbsolutePath<char16_t>(const char16_t* path, int len);


//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::endWithSeparator(const TChar* path, int len)
{
	if (LN_REQUIRE(path != nullptr)) return false;

	len = (len < 0) ? StringTraits::tcslen(path) : len;
	if (len >= 1)
	{
		return isSeparatorChar(path[len - 1]);
	}
	return false;
}
template bool PathTraits::endWithSeparator<char>(const char* path, int len);
template bool PathTraits::endWithSeparator<wchar_t>(const wchar_t* path, int len);
template bool PathTraits::endWithSeparator<char16_t>(const char16_t* path, int len);


//------------------------------------------------------------------------------
template<typename TChar>
const TChar* PathTraits::getFileNameSub(const TChar* path)
{
	int len = StringTraits::tcslen(path);
	int pos = len - 1;

	// 後ろから前に調べて、最初にセパレータが見つかるところを探す
	while (pos >= 0)
	{
		if (path[pos] != 0x00)
		{
			if (path[pos] == DirectorySeparatorChar || path[pos] == AltDirectorySeparatorChar || path[pos] == VolumeSeparatorChar) {
				return &path[pos + 1];
			}
		}
		--pos;
	}
	return path;
}
template const char* PathTraits::getFileNameSub(const char* path);
template const wchar_t* PathTraits::getFileNameSub(const wchar_t* path);

//------------------------------------------------------------------------------
template<typename TChar>
void PathTraits::getFileNameWithoutExtension(const TChar* path, TChar* outExt)
{
	if (outExt == NULL) { return; }
	outExt[0] = 0x00;

	if (path == NULL) { return; }

	const TChar DOT[] = { '.', '\0' };
	const TChar* fileName = getFileNameSub(path);
	int len = StringTraits::tcslen(fileName);
	int i = StringTraits::lastIndexOf(fileName, len, DOT, 1, (len-1), len, CaseSensitivity::CaseSensitive);
	if (i >= 0) {
		StringTraits::copyString(outExt, LN_MAX_PATH, fileName, i);
		outExt[i] = '\0';
	}
	else {
		StringTraits::copyString(outExt, LN_MAX_PATH, fileName, len);
		outExt[len] = '\0';
	}
}
template void PathTraits::getFileNameWithoutExtension(const char* path, char* outExt);
template void PathTraits::getFileNameWithoutExtension(const wchar_t* path, wchar_t* outExt);
template void PathTraits::getFileNameWithoutExtension(const char16_t* path, char16_t* outExt);

//------------------------------------------------------------------------------
template<typename TChar>
void PathTraits::getExtension(const TChar* path, TChar* outExt)
{
	if (path == NULL || outExt == NULL) { return; }

	outExt[0] = 0x00;
	int len = StringTraits::tcslen(path);
	for (int i = len; i >= 0; --i)
	{
		TChar ch = path[i];
		if (ch == '.')
		{
			if (i != len - 1) {
				StringTraits::strncpy(outExt, LN_MAX_PATH, &path[i], len - i);
			}
			else {
				break;	// "file." のようなパターン
			}
		}
		if (ch == DirectorySeparatorChar || ch == AltDirectorySeparatorChar || ch == VolumeSeparatorChar) {
			break;		// . の前にセパレータが見つかった
		}
	}
}
template void PathTraits::getExtension(const char* path, char* outExt);
template void PathTraits::getExtension(const wchar_t* path, wchar_t* outExt);

//------------------------------------------------------------------------------
template<typename TChar>
Result PathTraits::combine(const TChar* path1, int path1Len, const TChar* path2, int path2Len, TChar* outPath, int pathCapacity) LN_NOEXCEPT
{
	if (path1 == nullptr) return Result::ArgumentError;
	if (path2 == nullptr) return Result::ArgumentError;
	if (outPath == nullptr) return Result::ArgumentError;

	if (!isAbsolutePath(path2, path2Len))
	{
		// path1 結合
		if (path1Len > pathCapacity) return Result::ArgumentError;
		memcpy(outPath, path1, path1Len * sizeof(TChar));
		outPath += path1Len;
		pathCapacity -= path1Len;

		// セパレータ 結合
		if (path1Len > 0 && !isSeparatorChar(*(outPath - 1)))
		{
			if (1 > pathCapacity) return Result::ArgumentError;
			outPath[0] = DirectorySeparatorChar;
			outPath += 1;
			pathCapacity -= 1;
		}
	}

	// path2 結合
	if (path2Len > pathCapacity) return Result::ArgumentError;
	memcpy(outPath, path2, path2Len * sizeof(TChar));
	outPath += path2Len;
	pathCapacity -= path2Len;

	// 終端 null
	if (1 > pathCapacity) return Result::ArgumentError;
	outPath[0] = '\0';

	return Result::Success;
}
template Result PathTraits::combine(const char* path1, int path1Len, const char* path2, int path2Len, char* outPath, int pathCapacity) LN_NOEXCEPT;
template Result PathTraits::combine(const wchar_t* path1, int path1Len, const wchar_t* path2, int path2Len, wchar_t* outPath, int pathCapacity) LN_NOEXCEPT;

//------------------------------------------------------------------------------
template<typename TChar>
int PathTraits::canonicalizePath(const TChar* srcPath, size_t srcLen, TChar* outPath)
{
	/*	
		Ubuntu の realpath は実際にファイルが存在しないと変なパスを返した。(/A/B/../C が /A だけになってしまう)
		なので、自前実装。

		パスは以下のように、/ を右寄せしたトークンのように解析する。

		C:/Projects/bin/Debug/	→ [C:/][Projects/][bin/][Debug/]
		/mnt/test				→ [/][mnt/][test]
		A/B/.					→ [A/][B/][.]
		
		解析は srcPath の後ろから行っている。
		後ろから前へ行うことで、一時バッファや巻き戻しを使用することなく、比較的シンプルに実装できる。
		ただし、結果は outPath に右詰で出力されるため、最後に左詰にする必要がある。
	*/

	if (srcLen == 0) {
		outPath[0] = '\0';
		return 0;
	}

	const TChar*	readPos = (srcPath + srcLen);		// 終端 \0 を指す
	TChar*			outBegin = outPath;
	TChar*			writeEnd = (outPath + srcLen);		// 書き込み先の終端。
	TChar*			writePos = writeEnd;				// 書き込み位置。この1つ前から書ける
	int				depth = 0;							// /.. の深さ
	bool			isFullPath = false;

	while (srcPath < readPos)
	{
		// 1つ前の / を探す
		const TChar* tokenBegin = readPos - 1;	// readPos はこの時点では終端 \0 か前回のトークンの先頭文字を指している
		for (; srcPath < tokenBegin && !isSeparatorChar(*(tokenBegin - 1)); --tokenBegin);
		//++tokenBegin;
		size_t tokenLen = readPos - tokenBegin;

		bool isSpecial = false;
		if (tokenLen > 0)
		{
			// [/]
			if (tokenLen == 1 && isSeparatorChar(tokenBegin[0]))
			{
				if (tokenBegin == srcPath) {
					isFullPath = true;	// 先頭文字の / だったらルート要素扱い
										// スキップせずに / を出力する
				}
				else {
					// 書き込みはスキップ
					isSpecial = true;
				}
			}
			// [.] or [./]
			else if (	(tokenLen == 1 && tokenBegin[0] == '.') ||
						(tokenLen == 2 && tokenBegin[0] == '.' && isSeparatorChar(tokenBegin[1])))
			{
				// 書き込みはスキップ
				isSpecial = true;
			}
			// [..] or [../]
			else if (	(tokenLen == 2 && tokenBegin[0] == '.' && tokenBegin[1] == '.') ||
						(tokenLen == 3 && tokenBegin[0] == '.' && tokenBegin[1] == '.' && isSeparatorChar(tokenBegin[2])))
			{
				// .. をカウントして書き込みはスキップ
				++depth;
				isSpecial = true;
			}
		}

		// [xxxx] or [xxxx/]
		if (!isSpecial)
		{
			// パスの先頭トークンである場合、ルート要素であるかチェック
			if (tokenBegin == srcPath)
			{
#ifdef LN_OS_WIN32
				for (size_t i = 0; i < tokenLen; ++i) {
					if (isVolumeSeparatorChar(tokenBegin[i])) {
						isFullPath = true;	// "C:" のように、トークン内に : が含まれていた
						break;
					}
				}
#else
				//if (IsSeparatorChar(tokenBegin[0])) {
				//	isRoot = true;		// パス先頭が / である
				//	isFullPath = true;
				//}
#endif
			}

			// .. のネストが 0 か、ルート要素を示すトークンであれば出力する
			if (depth == 0 || isFullPath)
			{
				writePos -= tokenLen;
				if (memcpy_s(writePos, (writeEnd - writePos) * sizeof(TChar), tokenBegin, tokenLen * sizeof(TChar)) != 0) {
					return -1;	// FailSafe.
				}
			}
			else if (depth > 0) {
				// .. がある場合、このフォルダ名は捨てて、.. のカウントを下げる
				--depth;
			}
		}

		readPos = tokenBegin;
	}

	// outPath には右詰でパス文字列が格納されているので、左に詰める
	int outLen = writeEnd - writePos;
	while (writePos < writeEnd)
	{
		*outPath = *writePos;
		++outPath;
		++writePos;
	}

	// 入力パスの末尾が / だったのに、生成した出力パスの末尾は / ではなかった。("./" などで発生する)
	// .NET の動作にあわせ、/ をつける
	if (isSeparatorChar(*(srcPath + srcLen - 1)) && !isSeparatorChar(*(outPath - 1))) {
		*outPath = *(srcPath + srcLen - 1);
		++outPath;
	}

	// to native sparator
	{
		for (int i = 0; i < outLen; i++)
		{
			if (isSeparatorChar(outBegin[i]))
			{
				outBegin[i] = DirectorySeparatorChar;
			}
		}
	}

	*outPath = '\0';
	return outLen;

#if 0	// ↓はトークンの / を左詰するパターン。↑の方が若干短くなったけどあまり変わらなかったかも。。。
	/*	
		Ubuntu の realpath は実際にファイルが存在しないと変なパスを返した。(/A/B/../C が /A だけになってしまう)
		なので、自前実装。

		パスは以下のように、/ を左寄せしたトークンのように解析する。

		C:/Projects/ConsoleApplication1/bin/Debug/	→ [C:][/Projects][/ConsoleApplication1][/bin][/Debug][/]
		/mnt/test									→ [/mnt][/test]
		A/B/.										→ [A][/B][/.]
		
		解析は srcPath の後ろから行っている。
		後ろから前へ行うことで、一時バッファや巻き戻しを使用することなく、比較的シンプルに実装できる。
		ただし、結果は outPath に右詰で出力されるため、最後に左詰にする必要がある。
	*/

	const TChar*	readPos = (srcPath + srcLen);		// 終端 \0 を指す
	TChar*			writeEnd = (outPath + srcLen);		// 書き込み先の終端。
	TChar*			writePos = writeEnd;				// 書き込み位置。この1つ前から書ける
	int				depth = 0;							// /.. の深さ
	bool			isFullPath = false;

	while (srcPath < readPos)
	{
		// 1つ前の / を探す
		const TChar* tokenBegin = readPos - 1;	// readPos はこの時点では終端 \0 か前回のトークンの / を指している
		for (; srcPath < tokenBegin && !IsSeparatorChar(*tokenBegin); --tokenBegin);
		size_t tokenLen = readPos - tokenBegin;

		// 名前部分の先頭を指しておく (/ があれば / の次)	/* "./A" に備える */
		const TChar* nameBegin = tokenBegin;
		const TChar* tokenEnd = tokenBegin + tokenLen;
		if (IsSeparatorChar(tokenBegin[0])) {
			++nameBegin;
		}
		size_t nameLen = tokenEnd - nameBegin;

		bool isSpecial = false;
		if (tokenLen > 0 /*&& tokenBegin[0] == '/'*/)
		{
			// [空]
			if (nameBegin == tokenEnd/*tokenLen == 1*/) {
				isSpecial = true;
				// 書き込みはスキップ
			}
			// [.]
			else if (nameLen == 1 && /*tokenLen == 2 && */nameBegin[0] == '.')
			{
				isSpecial = true;
				// 書き込みはスキップ
			}
			// [..]
			else if (nameLen == 2 && /*tokenLen == 3 && */nameBegin[0] == '.' && nameBegin[1] == '.')
			{
				isSpecial = true;
				// .. をカウントして書き込みはスキップ
				++depth;
			}
		}

		// [/xxxx] or [xxxx]
		if (!isSpecial)
		{
			// パスの先頭トークンである場合、ルート要素であるかチェック
			bool isRoot = false;
			if (tokenBegin == srcPath)
			{
#ifdef LN_OS_WIN32
				for (size_t i = 0; i < tokenLen; ++i) {
					if (IsVolumeSeparatorChar(tokenBegin[i])) {
						isRoot = true;	// "C:" のように、トークン内に : が含まれていた
						isFullPath = false;
						break;
					}
				}
#else
				if (IsSeparatorChar(tokenBegin[0])) {
					isRoot = true;		// パス先頭が / である
					isFullPath = false;
				}
#endif
			}

			// .. のネストが 0 か、ルート要素を示すトークンであれば出力する
			if (depth == 0 || isRoot)
			{
				writePos -= tokenLen;
				if (memcpy_s(writePos, (writeEnd - writePos) * sizeof(TChar), tokenBegin, tokenLen * sizeof(TChar)) != 0) {
					return -1;	// FailSafe.
				}
			}
			else if (depth > 0) {
				// .. がある場合、このフォルダ名は捨てて、.. のカウントを下げる
				--depth;
			}
		}

		readPos = tokenBegin;
	}

	if (!isFullPath && IsSeparatorChar(writePos[0])) {
		++writePos;
	}

	// outPath には右詰でパス文字列が格納されているので、左に詰める
	int outLen = writeEnd - writePos;
	while (writePos < writeEnd)
	{
		*outPath = *writePos;
		++outPath;
		++writePos;
	}
	*outPath = '\0';
	return outLen;
#endif
}
template int PathTraits::canonicalizePath(const char* srcPath, size_t srcLen, char* outPath);
template int PathTraits::canonicalizePath(const wchar_t* srcPath, size_t srcLen, wchar_t* outPath);
template int PathTraits::canonicalizePath(const char16_t* srcPath, size_t srcLen, char16_t* outPath);

//------------------------------------------------------------------------------
//template<typename TChar>
//void PathTraits::canonicalizePath(const TChar* srcPath, TChar* outPath)
//{
//	size_t srcLen = StringTraits::tcslen(srcPath);
//	if (isAbsolutePath(srcPath)) {
//		// 絶対パスであればそのまま出力してしまう
//		canonicalizePath(srcPath, srcLen, outPath);
//	}
//	else
//	{
//		LN_NOTIMPLEMENTED();
//		//// 相対パスであれば、カレントディレクトリと結合してから変換する
//		//TChar src[LN_MAX_PATH];
//		//size_t len2 = DirectoryUtils::getCurrentDirectory(src);
//		//src[len2] = DirectorySeparatorChar;
//		//++len2;
//		//StringTraits::copyString(src + len2, LN_MAX_PATH - len2, srcPath, srcLen);
//		////memcpy(src + len2, srcPath, srcLen * sizeof(TChar));
//		//srcLen += len2;
//		//canonicalizePath(src, srcLen, outPath);
//	}
//	// セパレータを統一する
//	normalizeSeparator(outPath);
//}
//template void PathTraits::canonicalizePath<char>(const char* srcPath, char* outPath);
//template void PathTraits::canonicalizePath<wchar_t>(const wchar_t* srcPath, wchar_t* outPath);
//template void PathTraits::canonicalizePath<char16_t>(const char16_t* srcPath, char16_t* outPath);
//
#if 0
//------------------------------------------------------------------------------
template<>
void PathTraits::CanonicalizePath(const char* srcPath, char* outPath)
{
	if (IsAbsolutePath(srcPath)) {
		char src[LN_MAX_PATH];
		str
	}
#if 0
#ifdef LN_OS_WIN32
	char* canonPath = _fullpath(outPath, srcPath, LN_MAX_PATH);
	LN_THROW(canonPath != NULL, ArgumentException);
#else
	char* canonPath = realpath(srcPath, outPath);	// C:/ とかあるとエラーになり、NULL を返す
	
	// realpath() は、パスの示すファイルが実際に存在しない場合も NULL を返す。
	// outPath は生成されているが、もしそれ以外の理由でエラーになっている場合は例外にする。
	if (canonPath == NULL)
	{
		if (errno != ENOENT &&		// 指定されたファイルが存在しない。
			errno != ENOTDIR) {		// パスのディレクトリ要素が、ディレクトリでない。
			LN_THROW(0, ArgumentException);
		}
	}
#endif
#endif
}
template<>
void PathTraits::CanonicalizePath(const wchar_t* srcPath, wchar_t* outPath)
{
#if 0
#ifdef LN_OS_WIN32
	wchar_t* canonPath = _wfullpath(outPath, srcPath, LN_MAX_PATH);
#else
	char mbcsSrc[LN_MAX_PATH];
	if (wcstombs(mbcsSrc, srcPath, LN_MAX_PATH) < 0) {
		LN_THROW(0, IOException);
	}

	char mbcsOut[LN_MAX_PATH];
	char* canonPath = realpath(mbcsSrc, mbcsOut);
	
	//char mbcsSrc[PATH_MAX];
	if (mbstowcs(outPath, mbcsOut, LN_MAX_PATH) < 0) {
		LN_THROW(0, IOException);
	}
#endif
	LN_THROW(canonPath != NULL, ArgumentException);
#endif
}
#endif
//template void PathTraits::CanonicalizePath<char>(const TChar* srcPath, TChar* outPath);
//template void PathTraits::CanonicalizePath<wchar_t>(const TChar* srcPath, TChar* outPath);


//------------------------------------------------------------------------------
template<typename TChar>
void PathTraits::normalizeSeparator(TChar* srcPath)
{
#ifdef LN_OS_WIN32
	for (; *srcPath; ++srcPath)
	{
		if (*srcPath == AltDirectorySeparatorChar) {
			*srcPath = DirectorySeparatorChar;
		}
	}
#endif
}


/**
@brief		2つのパス文字列を比較する
@param[in]	path1	: パス文字列
@param[in]	path2	: パス文字列
@return		並べ替え順序を示す整数 (0 で一致)
@details	DirectorySeparatorChar と AltDirectorySeparatorChar は等価とみなします。
また、大文字小文字を区別しません。
*/
//------------------------------------------------------------------------------


template<typename TChar>
int PathTraits::comparePathString(const TChar* path1, int len1, const TChar* path2, int len2)
{
	if (len1 != len2)
	{
		return len1 - len2;
	}

	const TChar* s1 = path1;
	const TChar* s2 = path2;

	if (FileSystem::getFileSystemCaseSensitivity() == CaseSensitivity::CaseInsensitive)
	{
		while (len1 && *s1 && *s2)
		{
			if (StringTraits::toUpper(*s1) != StringTraits::toUpper(*s2))
			{
				// セパレータの差は区別しない
				if ((*s1 == DirectorySeparatorChar || *s1 == AltDirectorySeparatorChar) &&
					(*s2 == DirectorySeparatorChar || *s2 == AltDirectorySeparatorChar)
					) {
					// 継続
				}
				else {
					return ((StringTraits::toUpper(*s1) - StringTraits::toUpper(*s2)));
				}
			}
			s1++;
			s2++;
			len1--;
		}

		return len1 ? ((StringTraits::toUpper(*s1) - StringTraits::toUpper(*s2))) : 0;
	}
	else
	{
		while (len1 && *s1 && *s2)
		{
			if (*s1 != *s2)
			{
				// セパレータの差は区別しない
				if ((*s1 == DirectorySeparatorChar || *s1 == AltDirectorySeparatorChar) &&
					(*s2 == DirectorySeparatorChar || *s2 == AltDirectorySeparatorChar)
					) {
					// 継続
				}
				else {
					return *s1 - *s2;
				}
			}
			s1++;
			s2++;
			len1--;
		}

		return len1 ? (*s1 - *s2) : 0;
	}
}
template int PathTraits::comparePathString(const char* path1, int len1, const char* path2, int len2);
template int PathTraits::comparePathString(const wchar_t* path1, int len1, const wchar_t* path2, int len2);
template int PathTraits::comparePathString(const char16_t* path1, int len1, const char16_t* path2, int len2);

////------------------------------------------------------------------------------
//template<typename TChar>
//bool PathTraits::equals(const TChar* path1, const TChar* path2)
//{
//	return compare(path1, path2) == 0;
//}
//template bool PathTraits::equals<char>(const char* path1, const char* path2);
//template bool PathTraits::equals<wchar_t>(const wchar_t* path1, const wchar_t* path2);
//template bool PathTraits::equals<char16_t>(const char16_t* path1, const char16_t* path2);

//------------------------------------------------------------------------------
template<typename TChar>
int PathTraits::compare(TChar ch1, TChar ch2, CaseSensitivity cs)
{
	if (isSeparatorChar(ch1) && isSeparatorChar(ch2)) { return 0; }
	return StringTraits::compare(ch1, ch2, cs);
}
template int PathTraits::compare<char>(char ch1, char ch2, CaseSensitivity cs);
template int PathTraits::compare<wchar_t>(wchar_t ch1, wchar_t ch2, CaseSensitivity cs);
template int PathTraits::compare<char16_t>(char16_t ch1, char16_t ch2, CaseSensitivity cs);

//------------------------------------------------------------------------------
// path1 から見たときの path2 の相対パス
//------------------------------------------------------------------------------
template<typename TChar>
static bool IsInternalSeparator(const TChar* path, int i, int len/*, int slen*/)
{
	if (i == len)
	{
		if (!PathTraits::isSeparatorChar(path[i - 1])) {
			return true;
		}
		else {
			return false;
		}
	}
	if (PathTraits::isSeparatorChar(path[i])) {
		return true;
	}
	return false;
}
template<typename TChar>
std::basic_string<TChar> PathTraits::diffPath(const TChar* path1, int len1, const TChar* path2, int len2, CaseSensitivity cs)
{
	// パス終端がセパレータでなければもう１字見るようにし、以降の処理でそれはセパレータとする
	int slen1 = (isSeparatorChar(path1[len1 - 1])) ? len1 : len1 + 1;
	int slen2 = (isSeparatorChar(path2[len2 - 1])) ? len2 : len2 + 1;

	// 双方のパスの先頭から完全に一致する部分を探す。
	int i = 0;	// 最初の不一致を指す
	int si = 0;	// 一致部分の中の最後のセパレータ位置
	for (; (i < slen1) && (i < slen2); ++i)
	{
		if (IsInternalSeparator(path1, i, len1) && IsInternalSeparator(path2, i, len2)) {
			// "/a/b/c" vs "/a/b" のようなとき、path2 の b の後ろをセパレータ扱いにしたい
			si = i;
		}
		//else if ((i >= len1) && (i >= len2)) {
		//	// ↑の if の後、1ループしてここで終了する
		//	break;
		//}
		else if (compare(path1[i], path2[i], cs) != 0) {
			break;
		}
		else if (isSeparatorChar(path1[i])) {
			si = i;
		}
	}

	//if (i == len2 && i != len1 && IsSeparatorChar(path2[i]))
	//{

	//}

	// 終端に / が無いことに備えて終端 \0 までを見る。path1 はディレクトリパスと仮定する。
	//if (i == len1 && IsSeparatorChar(path2[i])) {
	//	si = i;
	//}

	// 完全不一致
	if (i == 0) {
		return path2;
	}
	// 完全一致
	if (i == slen1 && i == slen2) {
		return std::basic_string<TChar>(1, '.');
	}

	// path1 の残りの部分からセパレータを探す。このセパレータの数が、戻る深さ(..) の数になる。
	const TChar SLASH[] = { '/', '\0' };
	const TChar DDOT[] = { '.', '.', '\0' };
	std::basic_string<TChar> relLead;
	for (; i < slen1; ++i)
	{
		if (IsInternalSeparator(path1, i, len1))
		{
			if (!relLead.empty()) {
				relLead += SLASH;
			}
			relLead += DDOT;
		}
	}

	if (si >= len2) {	// 終端に仮のセパレータがあるとした場合
		return relLead;
	}

	// path2 のうちの後ろの不一致部分
	int subLen = len2 - (si + 1);
	if (isSeparatorChar(path2[len2-1])) {
		--subLen;
	}

	// read が無い。つまり、path2 は path1 以下を指している
	//if (relLead.empty())
	//{
	//	if (subLen < 0)
	//		return std::basic_string<TChar>(path2 + si + 1);
	//	else
	//		return std::basic_string<TChar>(path2 + si + 1, subLen);
	//}
	//else
	{
		if (!relLead.empty() && subLen != 0) relLead += '/';

		if (subLen < 0)
		{
			return relLead + std::basic_string<TChar>(path2 + si + 1);
		}
		else
		{
			return relLead + std::basic_string<TChar>(path2 + si + 1, subLen);
		}
	}
}
template std::basic_string<char> PathTraits::diffPath(const char* path1, int len1, const char* path2, int len2, CaseSensitivity cs);
template std::basic_string<wchar_t> PathTraits::diffPath(const wchar_t* path1, int len1, const wchar_t* path2, int len2, CaseSensitivity cs);
template std::basic_string<char16_t> PathTraits::diffPath(const char16_t* path1, int len1, const char16_t* path2, int len2, CaseSensitivity cs);

LN_NAMESPACE_END
