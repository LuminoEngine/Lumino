
#include <time.h>
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/Base/StringU.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/PathName.h>
#include <Lumino/IO/DirectoryUtils.h>


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
void GenericPathName<TChar>::assignUnderBasePath(const PathNameT& basePath, const char* relativePath, int len)
{
	// フルパスの場合はそのまま割り当てる
	// basePath が空なら relativePath を使う
	if (PathTraits::isAbsolutePath(relativePath, len) ||
		basePath.isEmpty())
	{
		m_path.assignCStr(relativePath, len);
	}
	// フルパスでなければ結合する
	else
	{
		m_path = basePath.m_path;
		// 末尾にセパレータがなければ付加する
		if (!PathTraits::endWithSeparator(m_path.c_str(), m_path.getLength())) {
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
void GenericPathName<TChar>::assignUnderBasePath(const PathNameT& basePath, const wchar_t* relativePath, int len)
{
	// フルパスの場合はそのまま割り当てる
	// basePath が空なら relativePath を使う
	if (PathTraits::isAbsolutePath(relativePath, len) ||
		basePath.isEmpty())
	{
		m_path.assignCStr(relativePath, len);
	}
	// フルパスでなければ結合する
	else
	{
		m_path = basePath.m_path;
		// 末尾にセパレータがなければ付加する
		if (!PathTraits::endWithSeparator(m_path.c_str(), m_path.getLength())) {
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
	if (PathTraits::isAbsolutePath(path)) {
		m_path = path;
	}
	else {
		if (m_path.getLength() > 0 && !PathTraits::endWithSeparator(m_path.c_str(), m_path.getLength()))/*(*m_path.rbegin()) != Separator)*/ {	// 末尾セパレータ
			m_path += Separator;
		}
		m_path += path;
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::getFileNameWithoutExtension() const
{
	TChar path[LN_MAX_PATH];
	PathTraits::getFileNameWithoutExtension(m_path.c_str(), path);
	return GenericPathName<TChar>(path);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericStringRef<TChar> GenericPathName<TChar>::getExtension(bool withDot) const LN_NOEXCEPT
{
	StringRefT ref;
	PathTraits::getExtension(m_path.c_str(), withDot, &ref);
	return ref;
}

//------------------------------------------------------------------------------
#if defined(LN_UNICODE)
template<>
GenericString<TCHAR> GenericPathName<char>::toString() const
{
	return GenericString<TCHAR>::fromNativeCharString(m_path.c_str(), m_path.getLength());
}
template<>
GenericString<TCHAR> GenericPathName<wchar_t>::toString() const
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
const GenericString<TChar> GenericPathName<TChar>::getStrEndSeparator() const
{
	GenericStringT newStr = m_path;
	if (!newStr.isEmpty() && !PathTraits::endWithSeparator(newStr.c_str(), newStr.getLength())/*(*newStr.rbegin()) != Separator*/) {	// 末尾セパレータ
		newStr += Separator;
	}
	return newStr;
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::getWithoutExtension() const
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
GenericPathName<TChar> GenericPathName<TChar>::changeExtension(const TChar* newExt) const
{
	GenericString<TChar> newPath;
	for (int i = m_path.getLength() - 1; i >= 0; --i)
	{
		TChar ch = m_path[i];
		if (ch == '.') {
			newPath = m_path.mid(0, i);
			break;
		}
		if (PathTraits::isSeparatorChar(ch) || PathTraits::isVolumeSeparatorChar(ch)) { break; }
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
bool GenericPathName<TChar>::isAbsolute() const
{
	return PathTraits::isAbsolutePath(m_path.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::isRoot() const
{
	return PathTraits::isRootPath(m_path.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::isDirectory() const
{
	return detail::FileSystemInternal::existsDirectory(m_path.c_str(), m_path.getLength());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::checkExt(const TChar* ext) const
{
	// TODO: 大文字小文字の区別をする
	return StringTraits::endsWith(m_path.c_str(), m_path.getLength(), ext, -1, CaseSensitivity::CaseInsensitive);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::getParent() const
{
	/* 参考：他のライブラリの、空文字やセパレータが無いなどで親ディレクトリが取れない時の動作
		- Qt (QFileInfo::dir())		… "." を返す
		- wxWidgets (wxFileName)	… "" を返す
		- Python (os.path)			… "" を返す
		- Ruby (Pathname)			… ".." を返す
		- Java (os.nio.Paths)		… null を返す
		- C# (Path, Uri)			… "" を返す (入力が "" だった場合は例外)
	*/
	return GenericPathName(PathTraits::getDirectoryPath(m_path.c_str()).c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::canonicalizePath() const
{
	TChar tmpPath[LN_MAX_PATH + 1];
	memset(tmpPath, 0, sizeof(tmpPath));
	PathTraits::canonicalizePath(m_path.c_str(), tmpPath);
	return GenericPathName<TChar>(tmpPath);
}

//------------------------------------------------------------------------------
template<typename TChar>
std::string GenericPathName<TChar>::toLocalChar() const
{
	GenericString<char> tmp;
	tmp.assignCStr(m_path.c_str());
	return std::string(tmp.c_str());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::existsFile() const
{
	return detail::FileSystemInternal::existsFile(m_path.c_str(), m_path.getLength());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::existsDirectory() const
{
	return detail::FileSystemInternal::existsDirectory(m_path.c_str(), m_path.getLength());
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericPathName<TChar>::existsFileInDirectory(const StringRefT& relPath) const LN_NOEXCEPT
{
	TChar path[LN_MAX_PATH];
	PathTraits::combine(m_path.c_str(), m_path.getLength(), relPath.getBegin(), relPath.getLength(), path, LN_MAX_PATH);
	return detail::FileSystemInternal::existsFile(path, StringTraits::tcslen(path));
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::makeRelative(const GenericPathName<TChar>& target) const
{
	if (LN_CHECK_ARG(isAbsolute())) return GenericPathName<TChar>();
	if (LN_CHECK_ARG(target.isAbsolute())) return GenericPathName<TChar>();
	GenericString<TChar> rel = PathTraits::diffPath<TChar>(m_path.c_str(), m_path.getLength(), target.m_path.c_str(), target.m_path.getLength(), FileSystem::getFileSystemCaseSensitivity());
	return GenericPathName<TChar>(rel);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericPathName<TChar>::createDirectory() const { LN_AFX_FUNCNAME(createDirectory)(); }
template<typename TChar>
void GenericPathName<TChar>::LN_AFX_FUNCNAME(createDirectory)() const
{
	detail::FileSystemInternal::createDirectory(m_path.c_str(), m_path.getLength());
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
template<>
GenericPathName<char> GenericPathName<char>::getExecutablePath()
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
GenericPathName<wchar_t> GenericPathName<wchar_t>::getExecutablePath()
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
		if (LN_CHECK_ARG(!PathTraits::isAbsolutePath(childDir))) return GenericPathName<TChar>();
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
		if (!detail::FileSystemInternal::existsDirectory(path2.c_str(), path2.getLength())) {
			path2.clear();
		}
		return path2;
	}
	else {
		// フォルダが無かったら作成して返す
		if (!detail::FileSystemInternal::existsDirectory(path2.c_str(), path2.getLength())) {
			detail::FileSystemInternal::createDirectory(path2.c_str(), path2.getLength());
		}
		return path2;
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericPathName<TChar> GenericPathName<TChar>::getUniqueFilePathInDirectory(const PathNameT& directory, const TChar* filePrefix, const TChar* extName)
{
	GenericStringT dirPath = directory.getStrEndSeparator();
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
	} while (detail::FileSystemInternal::existsFile(filePath.c_str(), filePath.getLength()));

	return PathNameT(filePath.c_str());
}


/// (こちらはファイル名だけを返す)
template<typename TChar>
GenericString<TChar> GenericPathName<TChar>::getUniqueFileNameInDirectory(const PathNameT& directory, const TChar* filePrefix, const TChar* extName)
{
	return getUniqueFilePathInDirectory(directory,filePrefix, extName).getFileName();
}

// テンプレートのインスタンス化
template class GenericPathName<char>;
template class GenericPathName<wchar_t>;



//==============================================================================
// GenericStaticallyLocalPath
//==============================================================================
namespace detail {

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath()
{
	m_static[0] = '\0';
	m_length = 0;
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const char* path, int len)
{
	m_static[0] = '\0';
	m_length = UStringConvert::convertNativeString(path, len, m_static, LocalPathBaseLength + 1);
	if (m_length < 0 || m_length >= LocalPathBaseLength)
	{
		// long path
		UStringConvert::convertToStdString(path, len, &m_path);
		m_length = m_path.length();
	}
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const wchar_t* path, int len)
{
	m_static[0] = '\0';
	m_length = UStringConvert::convertNativeString(path, len, m_static, LocalPathBaseLength + 1);
	if (m_length < 0 || m_length >= LocalPathBaseLength)
	{
		// long path
		UStringConvert::convertToStdString(path, len, &m_path);
		m_length = m_path.length();
	}
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const char16_t* path, int len)
{
	m_static[0] = '\0';
	m_length = UStringConvert::convertNativeString(path, len, m_static, LocalPathBaseLength + 1);
	if (m_length < 0 || m_length >= LocalPathBaseLength)
	{
		// long path
		UStringConvert::convertToStdString(path, len, &m_path);
		m_length = m_path.length();
	}
}

template class GenericStaticallyLocalPath<char>;
template class GenericStaticallyLocalPath<wchar_t>;
//template class GenericStaticallyLocalPath<char16_t>;

} // namespace detail

LN_NAMESPACE_END

