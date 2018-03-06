
#include <time.h>
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/IO/FileSystem.h>
#include <Lumino/IO/PathName.h>
#include <Lumino/IO/DirectoryUtils.h>


LN_NAMESPACE_BEGIN


//==============================================================================
// Path
//==============================================================================

const Char Path::Separator = (Char)PathTraits::DirectorySeparatorChar;
const Char Path::AltSeparator = (Char)PathTraits::AltDirectorySeparatorChar;
const Char Path::VolumeSeparator = (Char)PathTraits::VolumeSeparatorChar;

Path::Path()
{
}

Path::Path(const Path& path)
	: m_path(path.m_path)
{
}

void Path::assign(const StringRef& path)
{
	m_path = path;
}

void Path::assignUnderBasePath(const Path& basePath, const StringRef& relativePath)
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

void Path::append(const StringRef& path)
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

String Path::getFileName() const
{
	const Char* end = m_path.c_str() + m_path.getLength();
	return String(PathTraits::getFileName(m_path.c_str(), end), end);
}

bool Path::isAbsolute() const
{
	return PathTraits::isAbsolutePath(m_path.c_str());
}

Path Path::getWithoutExtension() const
{
	const Char* begin = m_path.c_str();
	return String(begin, PathTraits::getWithoutExtensionEnd(begin, begin + m_path.getLength()));
}

StringRef Path::getExtension(bool withDot) const
{
	const Char* begin = m_path.c_str();
	const Char* end = m_path.c_str() + m_path.getLength();
	return StringRef(PathTraits::getExtensionBegin(begin, end, withDot), end);
}

Path Path::getParent() const
{
	const Char* begin = m_path.c_str();
	const Char* end = m_path.c_str() + m_path.getLength();
	return StringRef(begin, PathTraits::getDirectoryPathEnd(begin, end));
}

StringRef Path::getFileNameWithoutExtension() const
{
	const Char* begin = m_path.c_str();
	const Char* end = m_path.c_str() + m_path.getLength();
	return StringRef(
		PathTraits::getFileName(m_path.c_str(), end),
		PathTraits::getExtensionBegin(begin, end, true));
}

Path Path::canonicalizePath() const
{
	if (PathTraits::isCanonicalPath(m_path.c_str(), m_path.getLength()))
	{
		return *this;
	}

	String* path;

	if (isAbsolute())
	{
		std::vector<Char> tmpPath(m_path.getLength());
		int len = PathTraits::canonicalizePath(m_path.c_str(), m_path.getLength(), tmpPath.data());
		return Path(StringRef(tmpPath.data(), len));
	}
	else
	{
		Path fullPath(Environment::getCurrentDirectory(), m_path);
		std::vector<Char> tmpPath(fullPath.m_path.getLength());
		int len = PathTraits::canonicalizePath(fullPath.m_path.c_str(), fullPath.m_path.getLength(), tmpPath.data());
		return Path(StringRef(tmpPath.data(), len));
	}
}

Path Path::makeRelative(const Path& target) const
{
	const Path* path1 = this;
	const Path* path2 = &target;
	Path fullPath1;
	Path fullPath2;
	if (!isAbsolute())
	{
		fullPath1 = canonicalizePath();
		path1 = &fullPath1;
	}
	if (!target.isAbsolute())
	{
		fullPath2 = target.canonicalizePath();
		path2 = &fullPath2;
	}

	auto relPath = PathTraits::diffPath<Char>(path1->c_str(), path1->getLength(), path2->c_str(), path2->getLength(), FileSystem::getFileSystemCaseSensitivity());
	return Path(relPath.c_str());	// TODO: un copy
}

int Path::compare(const Char* path) const { return PathTraits::comparePathString(m_path.c_str(), m_path.getLength(), path, StringTraits::tcslen(path)); }
int Path::compare(const Path& path) const { return PathTraits::comparePathString(m_path.c_str(), m_path.getLength(), path.c_str(), path.getLength()); }
int Path::compare(const String& path) const { return PathTraits::comparePathString(m_path.c_str(), m_path.getLength(), path.c_str(), path.getLength()); }

const String Path::getStrEndSeparator() const
{
	String newStr = m_path;
	if (!newStr.isEmpty() && !PathTraits::endWithSeparator(newStr.c_str(), newStr.getLength())/*(*newStr.rbegin()) != Separator*/) {	// 末尾セパレータ
		newStr += Separator;
	}
	return newStr;
}

Path Path::currentDirectory()
{
	return Path(Environment::getCurrentDirectory());
}

Path Path::executablePath()
{
	return Path(detail::EnvironmentInternal::getExecutablePath());
}

Path Path::getSpecialFolderPath(SpecialFolder specialFolder, const Char* childDir, SpecialFolderOption option)
{
	if (childDir != NULL) {
		if (LN_REQUIRE(!PathTraits::isAbsolutePath(childDir))) return Path();
	}

	Path path2(Environment::getSpecialFolderPath(specialFolder));
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
	String dirPath = directory.getStrEndSeparator();
	uint64_t key = static_cast<uint64_t>(::time(NULL));

	// 同番号のファイルがあればインクリメントしつつ空き番号を調べる
	int number = 1;
	String filePath;
	String work;
	do
	{
		// TODO: filePrefix とかは この do ループの前で String にしておき、結合は cat でやれば効率的。
		if (filePrefix != NULL && extName != NULL) {
			filePath = String::format(_LT("{0}{1}{2}{3}{4}"), dirPath.c_str(), filePrefix, key, number, extName);
		}
		else if (filePrefix == NULL && extName != NULL) {
			filePath = String::format(_LT("{0}{1}{2}{3}"), dirPath.c_str(), key, number, extName);
		}
		else if (filePrefix != NULL && extName == NULL) {
			filePath = String::format(_LT("{0}{1}{2}{3}"), dirPath.c_str(), filePrefix, key, number);
		}
		else {
			filePath = String::format(_LT("{0}{1}{2}"), dirPath.c_str(), key, number);
		}

		number++;
	} while (detail::FileSystemInternal::existsFile(filePath.c_str(), filePath.getLength()));

	return String(filePath.c_str());
}

String Path::getUniqueFileNameInDirectory(const Path& directory, const Char* filePrefix, const Char* extName)
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

