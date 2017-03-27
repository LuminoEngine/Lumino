
#include "../Internal.h"
#include "../../include/Lumino/Base/String.h"
#include "../../include/Lumino/IO/PathTraits.h"
#include "../../include/Lumino/IO/DirectoryUtils.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// PathTraits
//==============================================================================

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::IsSeparatorChar(TChar ch)
{
#ifdef LN_OS_WIN32
	return (ch == '\\' || ch == '/');
#else
	return (ch == '/');
#endif
}
template bool PathTraits::IsSeparatorChar<char>(char ch);
template bool PathTraits::IsSeparatorChar<wchar_t>(wchar_t ch);

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::IsVolumeSeparatorChar(TChar ch)
{
#ifdef LN_OS_WIN32
	return (ch == ':');
#else
	return false;
#endif
}
template bool PathTraits::IsVolumeSeparatorChar<char>(char ch);
template bool PathTraits::IsVolumeSeparatorChar<wchar_t>(wchar_t ch);

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::IsRootPath(const TChar* path)
{
#ifdef LN_OS_WIN32
	// windows の場合
	size_t len = StringTraits::tcslen(path);
	if (IsAbsolutePath(path) && len >= 2)
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
template bool PathTraits::IsRootPath<char>(const char* path);
template bool PathTraits::IsRootPath<wchar_t>(const wchar_t* path);

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::IsAbsolutePath(const TChar* path, int len)
{
	LN_THROW(path != NULL, ArgumentException);
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
template bool PathTraits::IsAbsolutePath<char>(const char* path, int len);
template bool PathTraits::IsAbsolutePath<wchar_t>(const wchar_t* path, int len);

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::EndWithSeparator(const TChar* path, int len)
{
	if (LN_CHECK_ARG(path != nullptr)) return false;

	len = (len < 0) ? StringTraits::tcslen(path) : len;
	if (len >= 1)
	{
		return IsSeparatorChar(path[len - 1]);
	}
	return false;
}
template bool PathTraits::EndWithSeparator<char>(const char* path, int len);
template bool PathTraits::EndWithSeparator<wchar_t>(const wchar_t* path, int len);

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> PathTraits::GetDirectoryPath(const TChar* path)
{
	LN_THROW(path != NULL, ArgumentException);


	/* 参考：他のライブラリの、空文字やセパレータが無いなどで親ディレクトリが取れない時の動作
									"C:"	"C:/"	"C:/file"
	- Qt (QFileInfo)			…	"C:"	"C:/"	"C:/"
	- wxWidgets (wxFileName)	…
	- Python (os.path)			…
	- Ruby (Pathname)			…	"C:.."	"C:/"	"C:/"
	- Java (os.nio.Paths)		… 
	- C# (Path, Uri)			…	""		"C:/"	"C:/"
	*/

	// 後ろから前に調べて、最初に \\ か / が見つかるところを探す
	int pos = StringTraits::tcslen(path);
	TChar lastSep = 0;
	for ( ; pos >= 0; --pos ) {
		if ( path[pos] == '\\' || path[pos] == '/' ) {
			lastSep = path[pos];
			break;
		}
	}

	GenericString<TChar> str;
	if (pos >= 0) {
		str = GenericString<TChar>(path, pos);

		// ルートパスの末尾は必ずセパレータにする
		if (IsRootPath(str.c_str()))
		{
			// 末尾がセパレータでなければセパレータを追加する
			//if ((*str.rbegin() != DirectorySeparatorChar) && (*str.rbegin() != AltDirectorySeparatorChar)) {
			//if (str.LastIndexOf(DirectorySeparatorChar) != str.GetLength() &&
			//	str.LastIndexOf(AltDirectorySeparatorChar) != str.GetLength()){
			if (!str.EndsWith((TChar)DirectorySeparatorChar) && !str.EndsWith((TChar)AltDirectorySeparatorChar))
			{
				if (lastSep != 0) {
					str += (const char)lastSep;
				}
				else {
					str += (const char)DirectorySeparatorChar;
				}
			}
		}

	}
	else
	{
		// セパレータが見つからなかった。ただし、ルートパスの場合は空文字にしない。
		if (IsRootPath(path)) {
			str = path;
		}
	}

	return str;
}
template GenericString<char> PathTraits::GetDirectoryPath<char>(const char* path);
template GenericString<wchar_t> PathTraits::GetDirectoryPath<wchar_t>(const wchar_t* path);

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> PathTraits::GetFileName(const TChar* path)
{
	return GenericString<TChar>(GetFileNameSub(path));
}
template GenericString<char> PathTraits::GetFileName(const char* path);
template GenericString<wchar_t> PathTraits::GetFileName(const wchar_t* path);


//------------------------------------------------------------------------------
template<typename TChar>
const TChar* PathTraits::GetFileNameSub(const TChar* path)
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
template const char* PathTraits::GetFileNameSub(const char* path);
template const wchar_t* PathTraits::GetFileNameSub(const wchar_t* path);

//------------------------------------------------------------------------------
template<typename TChar>
void PathTraits::GetFileNameWithoutExtension(const TChar* path, TChar* outExt)
{
	if (outExt == NULL) { return; }
	outExt[0] = 0x00;

	if (path == NULL) { return; }

	const TChar* fileName = GetFileNameSub(path);
	int len = StringTraits::tcslen(fileName);
	int i = StringTraits::LastIndexOf(fileName, len, LN_T(TChar, "."), 1, (len-1), len, CaseSensitivity::CaseSensitive);
	if (i >= 0) {
		StringTraits::StrNCpy(outExt, LN_MAX_PATH, fileName, i);
		outExt[i] = '\0';
	}
	else {
		StringTraits::StrNCpy(outExt, LN_MAX_PATH, fileName, len);
		outExt[len] = '\0';
	}
}
template void PathTraits::GetFileNameWithoutExtension(const char* path, char* outExt);
template void PathTraits::GetFileNameWithoutExtension(const wchar_t* path, wchar_t* outExt);

//------------------------------------------------------------------------------
template<typename TChar>
void PathTraits::GetExtension(const TChar* path, TChar* outExt)
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
				StringTraits::StrNCpy(outExt, LN_MAX_PATH, &path[i], len - i);
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
template void PathTraits::GetExtension(const char* path, char* outExt);
template void PathTraits::GetExtension(const wchar_t* path, wchar_t* outExt);

//------------------------------------------------------------------------------
template<typename TChar>
Result PathTraits::GetExtension(const TChar* path, bool withDot, GenericStringRef<TChar>* outRef) LN_NOEXCEPT
{
	if (path == nullptr || outRef == nullptr) { return Result::ArgumentError; }
	outRef->Attach(path, 0, 0);

	int len = StringTraits::tcslen(path);
	for (int i = len; i >= 0; --i)
	{
		TChar ch = path[i];
		if (ch == '.')
		{
			if (withDot)
			{
				if (i != len - 1) {
					*outRef = GenericStringRef<TChar>(&path[i], len - i);
					break;
				}
			}
			else
			{
				if (i + 1 != len - 1) {
					*outRef = GenericStringRef<TChar>(&path[i + 1], len - i);
					break;
				}
			}

			break;	// "file." のようなパターン
		}
		
		if (IsSeparatorChar(ch)) {
			break;		// . の前にセパレータが見つかった
		}
	}
	return Result::Success;
}
template Result PathTraits::GetExtension(const char* path, bool withDot, GenericStringRef<char>* outRef) LN_NOEXCEPT;
template Result PathTraits::GetExtension(const wchar_t* path, bool withDot, GenericStringRef<wchar_t>* outRef) LN_NOEXCEPT;

//------------------------------------------------------------------------------
template<typename TChar>
Result PathTraits::Combine(const TChar* path1, int path1Len, const TChar* path2, int path2Len, TChar* outPath, int pathCapacity) LN_NOEXCEPT
{
	if (path1 == nullptr) return Result::ArgumentError;
	if (path2 == nullptr) return Result::ArgumentError;
	if (outPath == nullptr) return Result::ArgumentError;

	if (!IsAbsolutePath(path2, path2Len))
	{
		// path1 結合
		if (path1Len > pathCapacity) return Result::ArgumentError;
		memcpy(outPath, path1, path1Len * sizeof(TChar));
		outPath += path1Len;
		pathCapacity -= path1Len;

		// セパレータ 結合
		if (path1Len > 0 && !IsSeparatorChar(*(outPath - 1)))
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
template Result PathTraits::Combine(const char* path1, int path1Len, const char* path2, int path2Len, char* outPath, int pathCapacity) LN_NOEXCEPT;
template Result PathTraits::Combine(const wchar_t* path1, int path1Len, const wchar_t* path2, int path2Len, wchar_t* outPath, int pathCapacity) LN_NOEXCEPT;

//------------------------------------------------------------------------------
template<typename TChar>
int PathTraits::CanonicalizePath(const TChar* srcPath, size_t srcLen, TChar* outPath)
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
	TChar*			writeEnd = (outPath + srcLen);		// 書き込み先の終端。
	TChar*			writePos = writeEnd;				// 書き込み位置。この1つ前から書ける
	int				depth = 0;							// /.. の深さ
	bool			isFullPath = false;

	while (srcPath < readPos)
	{
		// 1つ前の / を探す
		const TChar* tokenBegin = readPos - 1;	// readPos はこの時点では終端 \0 か前回のトークンの先頭文字を指している
		for (; srcPath < tokenBegin && !IsSeparatorChar(*(tokenBegin - 1)); --tokenBegin);
		//++tokenBegin;
		size_t tokenLen = readPos - tokenBegin;

		bool isSpecial = false;
		if (tokenLen > 0)
		{
			// [/]
			if (tokenLen == 1 && IsSeparatorChar(tokenBegin[0]))
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
						(tokenLen == 2 && tokenBegin[0] == '.' && IsSeparatorChar(tokenBegin[1])))
			{
				// 書き込みはスキップ
				isSpecial = true;
			}
			// [..] or [../]
			else if (	(tokenLen == 2 && tokenBegin[0] == '.' && tokenBegin[1] == '.') ||
						(tokenLen == 3 && tokenBegin[0] == '.' && tokenBegin[1] == '.' && IsSeparatorChar(tokenBegin[2])))
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
					if (IsVolumeSeparatorChar(tokenBegin[i])) {
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
	if (IsSeparatorChar(*(srcPath + srcLen - 1)) && !IsSeparatorChar(*(outPath - 1))) {
		*outPath = *(srcPath + srcLen - 1);
		++outPath;
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
template int PathTraits::CanonicalizePath(const char* srcPath, size_t srcLen, char* outPath);
template int PathTraits::CanonicalizePath(const wchar_t* srcPath, size_t srcLen, wchar_t* outPath);

//------------------------------------------------------------------------------
template<typename TChar>
void PathTraits::CanonicalizePath(const TChar* srcPath, TChar* outPath)
{
	size_t srcLen = StringTraits::tcslen(srcPath);
	if (IsAbsolutePath(srcPath)) {
		// 絶対パスであればそのまま出力してしまう
		CanonicalizePath(srcPath, srcLen, outPath);
	}
	else
	{
		// 相対パスであれば、カレントディレクトリと結合してから変換する
		TChar src[LN_MAX_PATH];
		size_t len2 = DirectoryUtils::GetCurrentDirectory(src);
		src[len2] = DirectorySeparatorChar;
		++len2;
		StringTraits::StrNCpy(src + len2, LN_MAX_PATH - len2, srcPath, srcLen);
		srcLen += len2;
		CanonicalizePath(src, srcLen, outPath);
	}
	// セパレータを統一する
	NormalizeSeparator(outPath);
}
template void PathTraits::CanonicalizePath<char>(const char* srcPath, char* outPath);
template void PathTraits::CanonicalizePath<wchar_t>(const wchar_t* srcPath, wchar_t* outPath);

//------------------------------------------------------------------------------
template<typename TChar>
void PathTraits::CanonicalizePath(GenericString<TChar>* path)
{
	if (path == NULL) { return; }

	TChar tmpPath[LN_MAX_PATH + 1];
	memset(tmpPath, 0, sizeof(tmpPath));
	PathTraits::CanonicalizePath(path->c_str(), tmpPath);
	*path = tmpPath;
}
template void PathTraits::CanonicalizePath<char>(GenericString<char>* path);
template void PathTraits::CanonicalizePath<wchar_t>(GenericString<wchar_t>* path);

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
void PathTraits::NormalizeSeparator(TChar* srcPath)
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
int PathTraits::Compare(const TChar* path1, const TChar* path2)
{
	// まずは正規化。セキュリティ的推奨事項
	// https://www.jpcert.or.jp/java-rules/ids02-j.html
	TChar absPath1[LN_MAX_PATH + 1];
	TChar absPath2[LN_MAX_PATH + 1];
	memset(absPath1, 0, sizeof(absPath1));
	memset(absPath2, 0, sizeof(absPath2));
	CanonicalizePath(path1, absPath1);
	CanonicalizePath(path2, absPath2);

	TChar* s1 = absPath1;
	TChar* s2 = absPath2;

#ifdef LN_OS_WIN32
	// 大文字小文字区別せず、文字が等しい間繰り返す
	while (*s1 && *s2)
	{
		if (StringTraits::ToUpper(*s1) != StringTraits::ToUpper(*s2))
		{
			// セパレータの差は区別しない
			if ((*s1 == DirectorySeparatorChar || *s1 == AltDirectorySeparatorChar) &&
				(*s2 == DirectorySeparatorChar || *s2 == AltDirectorySeparatorChar)
				){
				// 継続
			}
			else {
				return ((StringTraits::ToUpper(*s1) - StringTraits::ToUpper(*s2)));
			}
		}
		s1++;
		s2++;
	}

	return ((StringTraits::ToUpper(*s1) - StringTraits::ToUpper(*s2)));
#else
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
		{
			// セパレータの差は区別しない
			if ((*s1 == DirectorySeparatorChar || *s1 == AltDirectorySeparatorChar) &&
				(*s2 == DirectorySeparatorChar || *s2 == AltDirectorySeparatorChar)
				){
				// 継続
			}
			else {
				return (*s1 - *s2);
			}
		}
		s1++;
		s2++;
	}

	return (*s1 - *s2);
#endif
}

//------------------------------------------------------------------------------
template<typename TChar>
bool PathTraits::Equals(const TChar* path1, const TChar* path2)
{
	return Compare(path1, path2) == 0;
}
template bool PathTraits::Equals<char>(const char* path1, const char* path2);
template bool PathTraits::Equals<wchar_t>(const wchar_t* path1, const wchar_t* path2);

//------------------------------------------------------------------------------
template<typename TChar>
int PathTraits::Compare(TChar ch1, TChar ch2, CaseSensitivity cs)
{
	if (IsSeparatorChar(ch1) && IsSeparatorChar(ch2)) { return 0; }
	return StringTraits::Compare(ch1, ch2, cs);
}
template int PathTraits::Compare<char>(char ch1, char ch2, CaseSensitivity cs);
template int PathTraits::Compare<wchar_t>(wchar_t ch1, wchar_t ch2, CaseSensitivity cs);

//------------------------------------------------------------------------------
// path1 から見たときの path2 の相対パス
//------------------------------------------------------------------------------
template<typename TChar>
static bool IsInternalSeparator(const TChar* path, int i, int len/*, int slen*/)
{
	if (i == len)
	{
		if (!PathTraits::IsSeparatorChar(path[i - 1])) {
			return true;
		}
		else {
			return false;
		}
	}
	if (PathTraits::IsSeparatorChar(path[i])) {
		return true;
	}
	return false;
}
template<typename TChar>
GenericString<TChar> PathTraits::DiffPath(const TChar* path1, int len1, const TChar* path2, int len2, CaseSensitivity cs)
{
	// パス終端がセパレータでなければもう１字見るようにし、以降の処理でそれはセパレータとする
	int slen1 = (IsSeparatorChar(path1[len1 - 1])) ? len1 : len1 + 1;
	int slen2 = (IsSeparatorChar(path2[len2 - 1])) ? len2 : len2 + 1;

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
		else if (Compare(path1[i], path2[i], cs) != 0) {
			break;
		}
		else if (IsSeparatorChar(path1[i])) {
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
		return GenericString<TChar>::FromNativeCharString(_T("."));	// TODO: 共通文字列にしたい。メモリ確保したくない//::GetEmpty();
	}

	// path1 の残りの部分からセパレータを探す。このセパレータの数が、戻る深さ(..) の数になる。
	GenericString<TChar> relLead;	// TODO: StringBuilder
	for (; i < slen1; ++i)
	{
		if (IsInternalSeparator(path1, i, len1))
		{
			if (!relLead.IsEmpty()) {
				relLead += LN_T(TChar, "/");
			}
			relLead += LN_T(TChar, "..");
		}
	}

	if (si >= len2) {	// 終端に仮のセパレータがあるとした場合
		return relLead;
	}

	int subLen = len2 - (si + 1);
	if (IsSeparatorChar(path2[len2-1])) {
		--subLen;
	}
	return relLead + GenericString<TChar>(path2, si + 1, subLen);
}
template GenericString<char> PathTraits::DiffPath(const char* path1, int len1, const char* path2, int len2, CaseSensitivity cs);
template GenericString<wchar_t> PathTraits::DiffPath(const wchar_t* path1, int len1, const wchar_t* path2, int len2, CaseSensitivity cs);

LN_NAMESPACE_END
