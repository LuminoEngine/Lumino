
#include <time.h>
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/PathName.h>
#include <Lumino/IO/DirectoryUtils.h>
#if defined(LN_OS_WIN32)
#define NOMINMAX
#include <windows.h>
#endif


LN_NAMESPACE_BEGIN

//==============================================================================
// GenericPathName
//==============================================================================

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar>::GenericPathName(const GenericPathName& obj)
	: m_path(obj.m_path)
{
}
	
//------------------------------------------------------------------------------
template<typename TChar>
void GenericPathName<TChar>::assign(const char* path, int length)
{
	m_path.assignCStr(path, length);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericPathName<TChar>::assign(const wchar_t* path, int length)
{
	m_path.assignCStr(path, length);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericPathName<TChar>::AssignUnderBasePath(const PathNameT& basePath, const char* relativePath, int len)
{
	// フルパスの場合はそのまま割り当てる
	if (PathTraits::IsAbsolutePath(relativePath, len))
	{
		m_path.assignCStr(relativePath, len);
	}
	// フルパスでなければ結合する
	else
	{
		m_path = basePath.m_path;
		// 末尾にセパレータがなければ付加する
		if (!PathTraits::EndWithSeparator(m_path.c_str(), m_path.getLength())) {
			m_path += Separator;
		}

		// relativePath 結合
		GenericStringT rel;
		rel.assignCStr(relativePath, len);
		m_path += rel;
	}

	// 単純化する (.NET の Uri の動作に合わせる)
	//PathTraits::CanonicalizePath(&m_path);
	// ↑× 相対パスはそのまま扱いたい
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericPathName<TChar>::AssignUnderBasePath(const PathNameT& basePath, const wchar_t* relativePath, int len)
{
	// フルパスの場合はそのまま割り当てる
	if (PathTraits::IsAbsolutePath(relativePath, len))
	{
		m_path.assignCStr(relativePath, len);
	}
	// フルパスでなければ結合する
	else
	{
		m_path = basePath.m_path;
		//if ((*m_path.rbegin()) != Separator) {	// 末尾セパレータ
		// 末尾がセパレータでなければ付加する
		if (!PathTraits::EndWithSeparator(m_path.c_str(), m_path.getLength())) {
			m_path += Separator;
		}

		// relativePath 結合
		GenericStringT rel;
		rel.assignCStr(relativePath, len);
		m_path += rel;
	}

	// 単純化する (.NET の Uri の動作に合わせる)
	//PathTraits::CanonicalizePath(&m_path);
	// ↑× 相対パスはそのまま扱いたい
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericPathName<TChar>::append(const TChar* path)
{
	if (PathTraits::IsAbsolutePath(path)) {
		m_path = path;
	}
	else {
		if (m_path.getLength() > 0 && !PathTraits::EndWithSeparator(m_path.c_str(), m_path.getLength()))/*(*m_path.rbegin()) != Separator)*/ {	// 末尾セパレータ
			m_path += Separator;
		}
		m_path += path;
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::GetFileNameWithoutExtension() const
{
	TChar path[LN_MAX_PATH];
	PathTraits::GetFileNameWithoutExtension(m_path.c_str(), path);
	return GenericPathName<TChar>(path);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericStringRef<TChar> GenericPathName<TChar>::GetExtension(bool withDot) const LN_NOEXCEPT
{
	StringRefT ref;
	PathTraits::GetExtension(m_path.c_str(), withDot, &ref);
	return ref;
}

//------------------------------------------------------------------------------
#if defined(LN_UNICODE)
template<>
GenericString<TCHAR> GenericPathName<char>::ToString() const
{
	return GenericString<TCHAR>::FromNativeCharString(m_path.c_str(), m_path.getLength());
}
template<>
GenericString<TCHAR> GenericPathName<wchar_t>::ToString() const
{
	return m_path;
}
#else
template<>
GenericString<TCHAR> GenericPathName<char>::toString() const
{
	return m_path;
}
template<>
GenericString<TCHAR> GenericPathName<wchar_t>::toString() const
{
	return GenericString<TCHAR>::fromNativeCharString(m_path.c_str(), m_path.getLength());
}
#endif

//------------------------------------------------------------------------------
template<typename TChar>
const GenericString<TChar> GenericPathName<TChar>::GetStrEndSeparator() const
{
	GenericStringT newStr = m_path;
	if (!newStr.isEmpty() && !PathTraits::EndWithSeparator(newStr.c_str(), newStr.getLength())/*(*newStr.rbegin()) != Separator*/) {	// 末尾セパレータ
		newStr += Separator;
	}
	return newStr;
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::GetWithoutExtension() const
{
	CaseSensitivity cs = FileSystem::getFileSystemCaseSensitivity();

	// 最後の . 位置確認
	int dotPos = m_path.lastIndexOf('.', -1, -1, cs);
	if (dotPos == -1) {
		return (*this);		// . が無ければ何もせずそのまま返す
	}

	// 最後のセパレータ位置確認
	int separatorPos = m_path.lastIndexOf(Separator, -1, -1, cs);
	if (AltSeparator != 0) {
		int altPos = m_path.lastIndexOf(AltSeparator, -1, -1, cs);
		separatorPos = std::max(separatorPos, altPos);
	}

	if (dotPos < separatorPos) {
		return (*this);		// "dir.sub/file" のように、. の後ろにセパレータがあった。ファイル名に付く . ではないので、何もせずそのまま返す
	}

	return GenericPathName<TChar>(m_path.left(dotPos));
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::ChangeExtension(const TChar* newExt) const
{
	GenericString<TChar> newPath;
	for (int i = m_path.getLength() - 1; i >= 0; --i)
	{
		TChar ch = m_path[i];
		if (ch == '.') {
			newPath = m_path.mid(0, i);
			break;
		}
		if (PathTraits::IsSeparatorChar(ch) || PathTraits::IsVolumeSeparatorChar(ch)) { break; }
	}

	// . が無かった場合
	if (newPath.isEmpty()) {
		newPath = m_path;
	}

	if (newExt != NULL) {
		if (StringTraits::tcslen(newExt) != 0 && newExt[0] != '.') {
			newPath += '.';
		}
		newPath += newExt;
	}
	return GenericPathName<TChar>(newPath);
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::IsAbsolute() const
{
	return PathTraits::IsAbsolutePath(m_path.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::IsRoot() const
{
	return PathTraits::IsRootPath(m_path.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::IsDirectory() const
{
	return FileSystem::existsDirectory(m_path.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::CheckExt(const TChar* ext) const
{
	// TODO: 大文字小文字の区別をする
	return StringTraits::endsWith(m_path.c_str(), m_path.getLength(), ext, -1, CaseSensitivity::CaseInsensitive);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::GetParent() const
{
	/* 参考：他のライブラリの、空文字やセパレータが無いなどで親ディレクトリが取れない時の動作
		- Qt (QFileInfo::dir())		… "." を返す
		- wxWidgets (wxFileName)	… "" を返す
		- Python (os.path)			… "" を返す
		- Ruby (Pathname)			… ".." を返す
		- Java (os.nio.Paths)		… null を返す
		- C# (Path, Uri)			… "" を返す (入力が "" だった場合は例外)
	*/
	return GenericPathName(PathTraits::GetDirectoryPath(m_path.c_str()).c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::CanonicalizePath() const
{
	TChar tmpPath[LN_MAX_PATH + 1];
	memset(tmpPath, 0, sizeof(tmpPath));
	PathTraits::CanonicalizePath(m_path.c_str(), tmpPath);
	return GenericPathName<TChar>(tmpPath);
}

//------------------------------------------------------------------------------
template<typename TChar>
std::string GenericPathName<TChar>::ToLocalChar() const
{
	GenericString<char> tmp;
	tmp.assignCStr(m_path.c_str());
	return std::string(tmp.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::existsFile() const
{
	return FileSystem::existsFile(m_path.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::existsDirectory() const
{
	return FileSystem::existsDirectory(m_path.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::ExistsFileInDirectory(const StringRefT& relPath) const LN_NOEXCEPT
{
	TChar path[LN_MAX_PATH];
	PathTraits::Combine(m_path.c_str(), m_path.getLength(), relPath.getBegin(), relPath.getLength(), path, LN_MAX_PATH);
	return FileSystem::existsFile(path);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::MakeRelative(const GenericPathName<TChar>& target) const
{
	if (LN_CHECK_ARG(IsAbsolute())) return GenericPathName<TChar>();
	if (LN_CHECK_ARG(target.IsAbsolute())) return GenericPathName<TChar>();
	GenericString<TChar> rel = PathTraits::DiffPath<TChar>(m_path.c_str(), m_path.getLength(), target.m_path.c_str(), target.m_path.getLength(), FileSystem::getFileSystemCaseSensitivity());
	return GenericPathName<TChar>(rel);
}

//------------------------------------------------------------------------------
#pragma push_macro("CreateDirectory")
#undef createDirectory
template<typename TChar>
void GenericPathName<TChar>::createDirectory() const { LN_AFX_FUNCNAME(createDirectory)(); }
template<typename TChar>
void GenericPathName<TChar>::LN_AFX_FUNCNAME(createDirectory)() const
{
	FileSystem::createDirectory(m_path.c_str());
}
#pragma pop_macro("CreateDirectory")

//------------------------------------------------------------------------------
#pragma push_macro("GetCurrentDirectory")
#undef getCurrentDirectory
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::getCurrentDirectory() { return LN_AFX_FUNCNAME(getCurrentDirectory)(); }
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::LN_AFX_FUNCNAME(getCurrentDirectory)()
{
	static GenericPathName<TChar> path;

	TChar curDir[LN_MAX_PATH];
	DirectoryUtils::getCurrentDirectory(curDir);

	// メモリ確保を抑える。
	if (path.m_path != curDir) {
		path = curDir;
	}
	return path;
}
#pragma pop_macro("GetCurrentDirectory")

//------------------------------------------------------------------------------
template<>
GenericPathName<char> GenericPathName<char>::GetExecutablePath()
{
#if defined(LN_OS_WIN32)
	char path[LN_MAX_PATH];
	::GetModuleFileNameA(NULL, path, LN_MAX_PATH);
	return GenericPathName<char>(path);
#else
	LN_NOTIMPLEMENTED();
#endif
}
//------------------------------------------------------------------------------
template<>
GenericPathName<wchar_t> GenericPathName<wchar_t>::GetExecutablePath()
{
#if defined(LN_OS_WIN32)
	wchar_t path[LN_MAX_PATH];
	::GetModuleFileNameW(NULL, path, LN_MAX_PATH);
	return GenericPathName<wchar_t>(path);
#else
	LN_NOTIMPLEMENTED();
#endif
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::getSpecialFolderPath(SpecialFolder specialFolder, const TChar* childDir, SpecialFolderOption option)
{
	if (childDir != NULL) {
		if (LN_CHECK_ARG(!PathTraits::IsAbsolutePath(childDir))) return GenericPathName<TChar>();
	}

	TChar path[LN_MAX_PATH];
	Environment::getSpecialFolderPath(specialFolder, path);

	GenericPathName<TChar> path2(path);
	if (childDir != NULL) {
		path2.append(childDir);
	}

	if (option == SpecialFolderOption::None)
	{
		// フォルダが無かったら空文字列にして返す
		if (!FileSystem::existsDirectory(path2)) {
			path2.clear();
		}
		return path2;
	}
	else {
		// フォルダが無かったら作成して返す
		if (!FileSystem::existsDirectory(path2)) {
			FileSystem::createDirectory(path2);
		}
		return path2;
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::GetUniqueFilePathInDirectory(const PathNameT& directory, const TChar* filePrefix, const TChar* extName)
{
	GenericStringT dirPath = directory.GetStrEndSeparator();
	uint64_t key = static_cast<uint64_t>(::time(NULL));

	// 同番号のファイルがあればインクリメントしつつ空き番号を調べる
	int number = 1;
	GenericStringT filePath;
	GenericStringT work;
	do
	{
		if (filePrefix != NULL && extName != NULL) {
			filePath = GenericStringT::sprintf(LN_T(TChar, "%s%s%llu%d%s"), dirPath.c_str(), filePrefix, key, number, extName);
		}
		else if (filePrefix == NULL && extName != NULL) {
			filePath = GenericStringT::sprintf(LN_T(TChar, "%s%llu%d%s"), dirPath.c_str(), key, number, extName);
		}
		else if (filePrefix != NULL && extName == NULL) {
			filePath = GenericStringT::sprintf(LN_T(TChar, "%s%s%llu%d"), dirPath.c_str(), filePrefix, key, number);
		}
		else {
			filePath = GenericStringT::sprintf(LN_T(TChar, "%s%llu%d"), dirPath.c_str(), key, number);
		}

		number++;
	} while (FileSystem::existsFile(filePath.c_str()));

	return PathNameT(filePath.c_str());
}


/// (こちらはファイル名だけを返す)
template<typename TChar>
GenericString<TChar> GenericPathName<TChar>::GetUniqueFileNameInDirectory(const PathNameT& directory, const TChar* filePrefix, const TChar* extName)
{
	return GetUniqueFilePathInDirectory(directory,filePrefix, extName).getFileName();
}

// テンプレートのインスタンス化
template class GenericPathName<char>;
template class GenericPathName<wchar_t>;



namespace detail
{

// dst の長さは LocalPathBaseLength であること
static int ConvertNativeString(const GenericStringRef<char>& src, char* dst)
{
	if (src.getLength() >= LocalPathBaseLength) return -1;
	return src.copyTo(dst, LocalPathBaseLength);
}
static int ConvertNativeString(const GenericStringRef<wchar_t>& src, wchar_t* dst)
{
	if (src.getLength() >= LocalPathBaseLength) return -1;
	return src.copyTo(dst, LocalPathBaseLength);
}
static int ConvertNativeString(const GenericStringRef<char>& src, wchar_t* dst)
{
	if (src.getLength() >= LocalPathBaseLength) return -1;
	char tmp[LocalPathBaseLength + 1];
	src.copyTo(tmp, LocalPathBaseLength);

	size_t size;
	errno_t err = mbstowcs_s(&size, dst, LocalPathBaseLength, tmp, LocalPathBaseLength);
	if (err != 0) return -1;
	return size;
}
static int ConvertNativeString(const GenericStringRef<wchar_t>& src, char* dst)
{
	if (src.getLength() >= LocalPathBaseLength) return -1;
	wchar_t tmp[LocalPathBaseLength + 1];
	src.copyTo(tmp, LocalPathBaseLength);

	size_t size;
	errno_t err = wcstombs_s(&size, dst, LocalPathBaseLength, tmp, LocalPathBaseLength);
	if (err != 0) return -1;
	return size;
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const GenericStringRef<char>& path)
{
	m_static[0] = '0';
	if (ConvertNativeString(path, m_static) < 0)
	{
		// 文字列が長すぎるなど、変換失敗したら String へ割り当てる
		m_path.assignCStr(path.getBegin(), path.getLength());
	}
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const GenericStringRef<wchar_t>& path)
{
	m_static[0] = '0';
	if (ConvertNativeString(path, m_static) < 0)
	{
		// 文字列が長すぎるなど、変換失敗したら String へ割り当てる
		m_path.assignCStr(path.getBegin(), path.getLength());
	}
}

template class GenericStaticallyLocalPath<char>;
template class GenericStaticallyLocalPath<wchar_t>;

} // namespace detail

LN_NAMESPACE_END

#pragma pop_macro("CreateDirectory")

