
#include "Internal.hpp"
#include <limits.h>
#include <LuminoCore/Base/StringHelper.hpp>
#include <LuminoCore/Base/Environment.hpp>
#include "PathHelper.hpp"

namespace ln {
namespace detail {

//==============================================================================
// PathHelper

template<typename TChar, typename TPred>
static const TChar* findLast(const TChar* begin, const TChar* end, TPred pred) {
    const TChar* pos = end - 1;
    for (; pos >= begin; --pos) {
        if (pred(*pos)) {
            return pos /* + 1*/;
        }
    }
    return begin;
}

template<typename TChar>
const TChar* PathTraits::getFileName(const TChar* begin, const TChar* end) {
    const TChar* pos = findLast(begin, end, [](TChar ch) { return isSeparatorChar(ch); });
    if (isSeparatorChar(*pos))
        return pos + 1;
    return begin;
}
template const char* PathTraits::getFileName(const char* begin, const char* end);
template const wchar_t* PathTraits::getFileName(const wchar_t* begin, const wchar_t* end);
template const Char* PathTraits::getFileName(const Char* begin, const Char* end);

template<typename TChar>
const TChar* PathTraits::getWithoutExtensionEnd(const TChar* begin, const TChar* end) {
    const TChar* pos = findLast(begin, end, [](TChar ch) { return isSeparatorChar(ch) || ch == '.'; });
    if (*pos == '.')
        return pos;
    return end;
}
template const char* PathTraits::getWithoutExtensionEnd(const char* begin, const char* end);
template const wchar_t* PathTraits::getWithoutExtensionEnd(const wchar_t* begin, const wchar_t* end);
template const Char* PathTraits::getWithoutExtensionEnd(const Char* begin, const Char* end);

template<typename TChar>
const TChar* PathTraits::getExtensionBegin(const TChar* begin, const TChar* end, bool withDot) {
    const TChar* pos = findLast(begin, end, [](TChar ch) { return isSeparatorChar(ch) || ch == '.'; });
    if (*pos == '.') {
        if (withDot) {
            if (end - pos == 1) // .NET implementation
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
template const Char* PathTraits::getExtensionBegin(const Char* begin, const Char* end, bool withDot);

template<typename TChar>
const TChar* PathTraits::getDirectoryPathEnd(const TChar* begin, const TChar* end) {
    /* 参考：他のライブラリの、空文字やセパレータが無いなどで親ディレクトリが取れない時の動作
	"C:"	"C:/"	"C:/file"
	- Qt (QFileInfo)			…	"C:"	"C:/"	"C:/"
	- wxWidgets (wxFileName)	…
	- Python (os.path)			…
	- Ruby (Pathname)			…	"C:.."	"C:/"	"C:/"
	- Java (os.nio.Paths)		…
	- C# (Path, Uri)			…	""		"C:/"	"C:/"
	*/
    int len = end - begin;
    if (len <= 0) {
        return begin;
    }
    if (isRootPath(begin, end - begin)) {
        if (len == 1 ||                      // "/"
            !isSeparatorChar(begin[len - 1]) // "C:"
        ) {
            return end;
        }
    }
    //if (isSeparatorChar(begin[len - 1])) {
    //	return end;
    //}

    // 後ろから前に調べて、最初に \\ か / が見つかるところを探す
    const TChar* pos = findLast(begin, end, [](TChar ch) { return isSeparatorChar(ch); });
    //if (isSeparatorChar(*pos)) {
    //	pos--;
    //}
    return pos;
}
template const char* PathTraits::getDirectoryPathEnd(const char* begin, const char* end);
template const wchar_t* PathTraits::getDirectoryPathEnd(const wchar_t* begin, const wchar_t* end);
template const Char* PathTraits::getDirectoryPathEnd(const Char* begin, const Char* end);

template<typename TChar>
bool PathTraits::endWithSeparator(const TChar* path, int len) {
    if (LN_REQUIRE(path != nullptr))
        return false;

    len = (len < 0) ? StringHelper::strlen(path) : len;
    if (len >= 1) {
        return isSeparatorChar(path[len - 1]);
    }
    return false;
}
template bool PathTraits::endWithSeparator<char>(const char* path, int len);
template bool PathTraits::endWithSeparator<wchar_t>(const wchar_t* path, int len);
template bool PathTraits::endWithSeparator<Char>(const Char* path, int len);

template<typename TChar>
static bool IsInternalSeparator(const TChar* path, int i, int len /*, int slen*/) {
    if (i == len) {
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
static int compare(TChar ch1, TChar ch2, CaseSensitivity cs) {
    if (PathTraits::isSeparatorChar(ch1) && PathTraits::isSeparatorChar(ch2)) {
        return 0;
    }
    return StringHelper::compare(ch1, ch2, cs);
}

template<typename TChar>
std::basic_string<TChar> PathTraits::diffPath(const TChar* path1, int len1, const TChar* path2, int len2, CaseSensitivity cs) {
    // パス終端がセパレータでなければもう１字見るようにし、以降の処理でそれはセパレータとする
    int slen1 = (isSeparatorChar(path1[len1 - 1])) ? len1 : len1 + 1;
    int slen2 = (isSeparatorChar(path2[len2 - 1])) ? len2 : len2 + 1;

    // 双方のパスの先頭から完全に一致する部分を探す。
    int i = 0;  // 最初の不一致を指す
    int si = 0; // 一致部分の中の最後のセパレータ位置
    for (; (i < slen1) && (i < slen2); ++i) {
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
    for (; i < slen1; ++i) {
        if (IsInternalSeparator(path1, i, len1)) {
            if (!relLead.empty()) {
                relLead += SLASH;
            }
            relLead += DDOT;
        }
    }

    if (si >= len2) { // 終端に仮のセパレータがあるとした場合
        return relLead;
    }

    // path2 のうちの後ろの不一致部分
    int subLen = len2 - (si + 1);
    if (isSeparatorChar(path2[len2 - 1])) {
        --subLen;
    }

    {
        std::basic_string<TChar> post = (subLen < 0) ? (std::basic_string<TChar>(path2 + si + 1)) : std::basic_string<TChar>(path2 + si + 1, subLen);

        if (!relLead.empty() && subLen != 0 && !post.empty()) {
            relLead += '/';
        }

        return relLead + post;
    }
}
template std::basic_string<char> PathTraits::diffPath(const char* path1, int len1, const char* path2, int len2, CaseSensitivity cs);
template std::basic_string<wchar_t> PathTraits::diffPath(const wchar_t* path1, int len1, const wchar_t* path2, int len2, CaseSensitivity cs);
template std::basic_string<Char> PathTraits::diffPath(const Char* path1, int len1, const Char* path2, int len2, CaseSensitivity cs);

//==============================================================================
// GenericStaticallyLocalPath

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath() {
    m_static[0] = '\0';
    m_length = 0;
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const char* path, size_t len) {
    m_static[0] = '\0';
    m_length = UStringConvert::convertNativeString(path, len, m_static, LocalPathBaseLength + 1);
    if (m_length < 0 || m_length >= LocalPathBaseLength) {
        // long path
        UStringConvert::convertToStdString(path, len, &m_path);
        m_length = m_path.length();
    }
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const wchar_t* path, size_t len) {
    m_static[0] = '\0';
    m_length = UStringConvert::convertNativeString(path, len, m_static, LocalPathBaseLength + 1);
    if (m_length < 0 || m_length >= LocalPathBaseLength) {
        // long path
        UStringConvert::convertToStdString(path, len, &m_path);
        m_length = m_path.length();
    }
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const char16_t* path, size_t len) {
    m_static[0] = '\0';
    m_length = UStringConvert::convertNativeString(path, len, m_static, LocalPathBaseLength + 1);
    if (m_length < 0 || m_length >= LocalPathBaseLength) {
        // long path
        UStringConvert::convertToStdString(path, len, &m_path);
        m_length = m_path.length();
    }
}

template<typename TChar>
GenericStaticallyLocalPath<TChar>::GenericStaticallyLocalPath(const char32_t* path, size_t len) {
    m_static[0] = '\0';
    m_length = UStringConvert::convertNativeString(path, len, m_static, LocalPathBaseLength + 1);
    if (m_length < 0 || m_length >= LocalPathBaseLength) {
        // long path
        UStringConvert::convertToStdString(path, len, &m_path);
        m_length = m_path.length();
    }
}

template class GenericStaticallyLocalPath<char>;
template class GenericStaticallyLocalPath<wchar_t>;

} // namespace detail
} // namespace ln
