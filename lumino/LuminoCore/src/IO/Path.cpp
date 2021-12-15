/*
    # Note: "String base" vs "Entity name array"
    Path クラスとしては String をラップするだけにしたい。Lumino の用途としては読み取り操作が多い。
    エンティティ分割は Uri クラスで。

    # Note: "dir/"
    "dir/" は、「dir 内の無名ファイルをさしている」みたいなイメージ。
    "dir/XXXX" にマッチする。
    それでいいのかはいろいろ意見あるだろうけど・・・。
*/

#include <time.h>
#include "Internal.hpp"
#include "PathHelper.hpp"
#include <LuminoCore/Base/Environment.hpp>
#include <LuminoCore/IO/FileSystem.hpp>
#include <LuminoCore/IO/Path.hpp>

namespace ln {

//==============================================================================
// Path

//const Char Path::Separator = (Char)PathTraits::DirectorySeparatorChar;
//const Char Path::AltSeparator = (Char)PathTraits::AltDirectorySeparatorChar;
//const Char Path::VolumeSeparator = (Char)PathTraits::VolumeSeparatorChar;

const Path Path::Empty;

Path::Path()
{
}

Path::Path(const Path& path)
    : m_path(path.m_path)
{
}

Path::Path(const Char* path)
{
    assign(path);
}
Path::Path(const String& path)
{
    assign(path);
}
Path::Path(const StringRef& path)
{
    assign(path);
}
Path::Path(const Path& basePath, const Char* relativePath)
{
    assignUnderBasePath(basePath, StringRef(relativePath));
}
Path::Path(const Path& basePath, const String& relativePath)
{
    assignUnderBasePath(basePath, StringRef(relativePath));
}
Path::Path(const Path& basePath, const StringRef& relativePath)
{
    assignUnderBasePath(basePath, relativePath);
}
Path::Path(const Path& basePath, const Path& relativePath)
{
    assignUnderBasePath(basePath, relativePath);
}

Path Path::fromStdPath(const std::filesystem::path& path)
{
    return Path(String::fromCString(path.c_str()));
}

std::filesystem::path Path::toStdPath() const
{
    return std::filesystem::path(m_path.toStdWString());
}

void Path::assign(const StringRef& path)
{
    m_path = path;
}

void Path::assignUnderBasePath(const Path& basePath, const StringRef& relativePath)
{
    // フルパスの場合はそのまま割り当てる
    // basePath が空なら relativePath を使う
    if (detail::PathTraits::isAbsolutePath(relativePath.data(), relativePath.length()) || basePath.isEmpty()) {
        m_path = relativePath;
    }
    // フルパスでなければ結合する
    else {
        m_path = basePath.m_path;

		if (!relativePath.isEmpty())
		{
			// 末尾にセパレータがなければ付加する
			if (!detail::PathTraits::endWithSeparator(m_path.c_str(), m_path.length())) {
				m_path += detail::PathTraits::DirectorySeparatorChar;
			}

			// relativePath 結合
			m_path += relativePath;
		}
    }
}

bool Path::isAbsolute() const
{
    return detail::PathTraits::isAbsolutePath(m_path.c_str(), m_path.length());
}

bool Path::isRelative() const
{
    return !isAbsolute();
}

bool Path::isRoot() const
{
    return detail::PathTraits::isRootPath(m_path.c_str(), m_path.length());
}

bool Path::isUnified() const
{
    return !m_path.contains(u'\\');
}

bool Path::hasExtension(const StringRef& ext) const
{
    const Char* begin = m_path.c_str();
    const Char* end = m_path.c_str() + m_path.length();
    StringRef thisExt(detail::PathTraits::getExtensionBegin(begin, end, false), end); // . は除く
    if (thisExt.isEmpty()) {
        return false;
    } else if (ext.isEmpty()) {
        return !thisExt.isEmpty();
    } else {
        StringRef otherExt = ext;
        if (otherExt[0] == '.') {
            otherExt = StringRef(detail::PathTraits::getExtensionBegin(ext.data(), ext.data() + ext.length(), false), ext.data() + ext.length()); // . は除く
        }
        if (thisExt.length() != otherExt.length()) {
            return false;
        }
        return StringHelper::endsWith(thisExt.data(), thisExt.length(), otherExt.data(), otherExt.length(), Environment::pathCaseSensitivity());
    }
}

Path Path::fileName() const
{
    const Char* end = m_path.c_str() + m_path.length();
    return Path(String(detail::PathTraits::getFileName(m_path.c_str(), end), end));
}

Path Path::withoutExtension() const
{
    /*
        ".git" など隠しファイル全体が拡張子扱いされるのは C# や boost の動作。
    */
    const Char* begin = m_path.c_str();
    return String(begin, detail::PathTraits::getWithoutExtensionEnd(begin, begin + m_path.length()));
}

StringRef Path::fileNameWithoutExtension() const
{
    const Char* begin = m_path.c_str();
    const Char* end = m_path.c_str() + m_path.length();
    return StringRef(
        detail::PathTraits::getFileName(m_path.c_str(), end),
        detail::PathTraits::getExtensionBegin(begin, end, true));
}

StringRef Path::extension(bool withDot) const
{
    const Char* begin = m_path.c_str();
    const Char* end = m_path.c_str() + m_path.length();
    return StringRef(detail::PathTraits::getExtensionBegin(begin, end, withDot), end);
}

Path Path::parent() const
{
    if (isEmpty()) {
        return Path(_TT(".."));
    } else if (isRoot() && !detail::PathTraits::endWithSeparator(m_path.c_str(), m_path.length())) {
        return *this;
    } else if (m_path.endsWith(_TT(".."))) {
        return Path(*this, _TT(".."));
    } else {
        const Char* begin = m_path.c_str();
        const Char* end = m_path.c_str() + m_path.length();
        return StringRef(begin, detail::PathTraits::getDirectoryPathEnd(begin, end));
    }
}

Path Path::native() const
{
#ifdef _WIN32
    String newPath = m_path;
    for (int i = 0; i < newPath.length(); i++) {
        if (newPath[i] == detail::PathTraits::AltDirectorySeparatorChar) {
            newPath[i] = detail::PathTraits::DirectorySeparatorChar;
        }
    }
    return Path(newPath);
#else
    return Path(*this); // copy
#endif
}

Path Path::unify() const
{
	String newPath = m_path;
	for (int i = 0; i < newPath.length(); i++) {
		if (newPath[i] == '\\') {
			newPath[i] = '/';
		}
	}
	return Path(newPath);
}

Path Path::canonicalize() const
{
    if (detail::PathTraits::isCanonicalPath(m_path.c_str(), m_path.length())) {
        return *this;
    }

    if (isAbsolute()) {
        std::vector<Char> tmpPath(m_path.length() + 1);
        int len = detail::PathTraits::canonicalizePath(m_path.c_str(), m_path.length(), tmpPath.data(), tmpPath.size());
        return Path(StringRef(tmpPath.data(), len));
    } else {
        Path fullPath(Environment::currentDirectory(), m_path);
        std::vector<Char> tmpPath(fullPath.m_path.length() + 1);
		int len = detail::PathTraits::canonicalizePath(fullPath.m_path.c_str(), fullPath.m_path.length(), tmpPath.data(), tmpPath.size());
        return Path(StringRef(tmpPath.data(), len));
    }
}

Path Path::makeRelative(const Path& target) const
{
    const Path* path1 = this;
    const Path* path2 = &target;
    Path fullPath1;
    Path fullPath2;
    if (!isAbsolute()) {
        fullPath1 = canonicalize();
        path1 = &fullPath1;
    }
    if (!target.isAbsolute()) {
        fullPath2 = target.canonicalize();
        path2 = &fullPath2;
    }

    auto relPath = detail::PathTraits::diffPath<Char>(path1->c_str(), path1->length(), path2->c_str(), path2->length(), Environment::pathCaseSensitivity());
    return Path(relPath.c_str()); // TODO: un copy
}

Path Path::replaceExtension(const StringRef& newExt) const
{
    const Char* begin = m_path.c_str();
    StringRef prefix(begin, detail::PathTraits::getWithoutExtensionEnd(begin, begin + m_path.length()));
    if (newExt.length() <= 0) {
        return Path(prefix);
    } else if (newExt[0] == '.') {
        return Path(String::concat(prefix, newExt));
    } else {
        return Path(String::concat(prefix, _TT("."), newExt));
    }
}

Path Path::withEndSeparator() const
{
    String newStr = m_path;
    if (!newStr.isEmpty() && !detail::PathTraits::endWithSeparator(newStr.c_str(), newStr.length())) {
        newStr += detail::PathTraits::DirectorySeparatorChar;
    }
    return newStr;
}

void Path::append(const StringRef& path)
{
    if (detail::PathTraits::isAbsolutePath(path.data(), path.length())) {
        m_path = path;
    } else {
        if (m_path.length() > 0 && !detail::PathTraits::endWithSeparator(m_path.c_str(), m_path.length())) {
            m_path += detail::PathTraits::DirectorySeparatorChar;
        }
        m_path += path;
    }
}

int Path::compare(const StringRef& path1, const StringRef& path2)
{
    return detail::PathTraits::comparePathString(path1.data(), path1.length(), path2.data(), path2.length());
}

bool Path::contains(const Path& subdir) const
{
    return detail::PathTraits::comparePathString(str().c_str(), length(), subdir.str().c_str(), length()) == 0;
}

Path Path::getUniqueFilePathInDirectory(const Path& directory, const Char* filePrefix, const Char* extName)
{
#if 1
    String dirPath = directory.withEndSeparator();

    // 同番号のファイルがあればインクリメントしつつ空き番号を調べる
    int number = 1;
    String filePath;
    String work;
    do
    {
        // TODO: filePrefix とかは この do ループの前で String にしておき、結合は cat でやれば効率的。
        if (filePrefix != NULL && extName != NULL) {
            filePath = String::format(_LT("{0}{1}{2}{3}"), dirPath.c_str(), filePrefix, number, extName);
        }
        else if (filePrefix == NULL && extName != NULL) {
            filePath = String::format(_LT("{0}{1}{2}"), dirPath.c_str(), number, extName);
        }
        else if (filePrefix != NULL && extName == NULL) {
            filePath = String::format(_LT("{0}{1}{2}"), dirPath.c_str(), filePrefix, number);
        }
        else {
            filePath = String::format(_LT("{0}{1}"), dirPath.c_str(), number);
        }

        number++;

    } while (detail::FileSystemInternal::existsFile(filePath.c_str(), filePath.length()));

    return String(filePath.c_str());
#endif
#if 0   // ランダムキーを付加するパターン (TODO: どこかで必要なら public にする)
    String dirPath = directory.withEndSeparator();
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
    } while (detail::FileSystemInternal::existsFile(filePath.c_str(), filePath.length()));

    return String(filePath.c_str());
#endif
}

#if 0
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





int Path::compare(const Char* path) const { return PathTraits::comparePathString(m_path.c_str(), m_path.getLength(), path, StringTraits::tcslen(path)); }
int Path::compare(const Path& path) const { return PathTraits::comparePathString(m_path.c_str(), m_path.getLength(), path.c_str(), path.getLength()); }
int Path::compare(const String& path) const { return PathTraits::comparePathString(m_path.c_str(), m_path.getLength(), path.c_str(), path.getLength()); }


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

String Path::getUniqueFileNameInDirectory(const Path& directory, const Char* filePrefix, const Char* extName)
{
    return getUniqueFilePathInDirectory(directory, filePrefix, extName).getFileName();
}
#endif

} // namespace ln
