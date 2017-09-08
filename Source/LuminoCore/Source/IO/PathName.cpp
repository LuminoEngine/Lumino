
#include <time.h>
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/Base/StringU.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/PathName.h>
#include <Lumino/IO/DirectoryUtils.h>


LN_NAMESPACE_BEGIN


//==============================================================================
// Path
//==============================================================================

const UChar Path::Separator = (UChar)PathTraits::DirectorySeparatorChar;
const UChar Path::AltSeparator = (UChar)PathTraits::AltDirectorySeparatorChar;
const UChar Path::VolumeSeparator = (UChar)PathTraits::VolumeSeparatorChar;

Path::Path()
{
}

Path::Path(const Path& path)
	: m_path(path.m_path)
{
}

void Path::assign(const UStringRef& path)
{
	m_path = path;
}

void Path::assignUnderBasePath(const Path& basePath, const UStringRef& relativePath)
{
	// フルパスの場合はそのまま割り当てる
	// basePath が空なら relativePath を使う
	if (PathTraits::isAbsolutePath(relativePath.data(), relativePath.getLength()) || basePath.isEmpty())
	{
		m_path = relativePath;
	}
	// フルパスでなければ結合する
	else
	{
		m_path = basePath.m_path;

		// 末尾にセパレータがなければ付加する
		if (!PathTraits::endWithSeparator(m_path.c_str(), m_path.getLength()))
		{
			m_path += Separator;
		}

		// relativePath 結合
		m_path += relativePath;
	}

	// 単純化する (.NET の Uri の動作に合わせる)
	//PathTraits::CanonicalizePath(&m_path);
	// ↑× 相対パスはそのまま扱いたい
}

void Path::assignUnderBasePath(const Path& basePath, const Path& relativePath)
{
	m_path = basePath.m_path;
	append(relativePath);
}

void Path::append(const UStringRef& path)
{
	if (PathTraits::isAbsolutePath(path.data(), path.getLength()))
	{
		m_path = path;
	}
	else
	{
		if (m_path.getLength() > 0 && !PathTraits::endWithSeparator(m_path.c_str(), m_path.getLength()))/*(*m_path.rbegin()) != Separator)*/	// 末尾セパレータ
		{
			m_path += Separator;
		}
		m_path += path;
	}
}

UString Path::getFileName() const
{
	const UChar* end = m_path.c_str() + m_path.getLength();
	return UString(PathTraits::getFileName(m_path.c_str(), end), end);
}

bool Path::isAbsolute() const
{
	return PathTraits::isAbsolutePath(m_path.c_str());
}

Path Path::getWithoutExtension() const
{
	const UChar* begin = m_path.c_str();
	return UString(begin, PathTraits::getWithoutExtensionEnd(begin, begin + m_path.getLength()));
}

UStringRef Path::getExtension(bool withDot) const
{
	const UChar* begin = m_path.c_str();
	const UChar* end = m_path.c_str() + m_path.getLength();
	return UStringRef(PathTraits::getExtensionBegin(begin, end, withDot), end);
}

Path Path::getParent() const
{
	const UChar* begin = m_path.c_str();
	const UChar* end = m_path.c_str() + m_path.getLength();
	return UStringRef(begin, PathTraits::getDirectoryPathEnd(begin, end));
}

UStringRef Path::getFileNameWithoutExtension() const
{
	const UChar* begin = m_path.c_str();
	const UChar* end = m_path.c_str() + m_path.getLength();
	return UStringRef(
		PathTraits::getFileName(m_path.c_str(), end),
		PathTraits::getExtensionBegin(begin, end, true));
}

Path Path::canonicalizePath() const
{
	// TODO: Length 制限無し
	Char tmpPath[LN_MAX_PATH + 1];
	memset(tmpPath, 0, sizeof(tmpPath));
	PathTraits::canonicalizePath(m_path.c_str(), tmpPath);
	return Path(tmpPath);
}

Path Path::makeRelative(const Path& target) const
{
	if (LN_REQUIRE(isAbsolute())) return Path();
	if (LN_REQUIRE(target.isAbsolute())) return Path();
	auto rel = PathTraits::diffPath<Char>(m_path.c_str(), m_path.getLength(), target.m_path.c_str(), target.m_path.getLength(), FileSystem::getFileSystemCaseSensitivity());
	return Path(rel.c_str());	// TODO: un copy
}

bool Path::equals(const Char* path) const { return PathTraits::equals(m_path.c_str(), path); }
bool Path::equals(const Path& path) const { return PathTraits::equals(m_path.c_str(), path.c_str()); }
bool Path::equals(const UString& path) const { return PathTraits::equals(m_path.c_str(), path.c_str()); }

const UString Path::getStrEndSeparator() const
{
	UString newStr = m_path;
	if (!newStr.isEmpty() && !PathTraits::endWithSeparator(newStr.c_str(), newStr.getLength())/*(*newStr.rbegin()) != Separator*/) {	// 末尾セパレータ
		newStr += Separator;
	}
	return newStr;
}

Path Path::getCurrentDirectory()
{
	// TODO: Max Len
	Char curDir[LN_MAX_PATH];
	DirectoryUtils::getCurrentDirectory(curDir);
	return Path(curDir);
}

Path Path::getSpecialFolderPath(SpecialFolder specialFolder, const Char* childDir, SpecialFolderOption option)
{
	if (childDir != NULL) {
		if (LN_REQUIRE(!PathTraits::isAbsolutePath(childDir))) return Path();
	}

	// TODO: Length
	Char path[LN_MAX_PATH];
	Environment::getSpecialFolderPath(specialFolder, path);

	Path path2(path);
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

Path Path::getUniqueFilePathInDirectory(const Path& directory, const Char* filePrefix, const Char* extName)
{
	UString dirPath = directory.getStrEndSeparator();
	uint64_t key = static_cast<uint64_t>(::time(NULL));

	// 同番号のファイルがあればインクリメントしつつ空き番号を調べる
	int number = 1;
	UString filePath;
	UString work;
	do
	{
		if (filePrefix != NULL && extName != NULL) {
			filePath = UString::sprintf(_LT("%s%s%llu%d%s"), dirPath.c_str(), filePrefix, key, number, extName);
		}
		else if (filePrefix == NULL && extName != NULL) {
			filePath = UString::sprintf(_LT("%s%llu%d%s"), dirPath.c_str(), key, number, extName);
		}
		else if (filePrefix != NULL && extName == NULL) {
			filePath = UString::sprintf(_LT("%s%s%llu%d"), dirPath.c_str(), filePrefix, key, number);
		}
		else {
			filePath = UString::sprintf(_LT("%s%llu%d"), dirPath.c_str(), key, number);
		}

		number++;
	} while (detail::FileSystemInternal::existsFile(filePath.c_str(), filePath.getLength()));

	return UString(filePath.c_str());
}

UString Path::getUniqueFileNameInDirectory(const Path& directory, const Char* filePrefix, const Char* extName)
{
	return getUniqueFilePathInDirectory(directory, filePrefix, extName).getFileName();
}


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

