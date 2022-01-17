// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

#include "../Base/String.hpp"
#include "../Base/Environment.hpp"
#include "Common.hpp"

namespace ln {

/**
 * ファイルパス文字列を表すクラスです。
 * 
 * このクラスはセパレータで区切られた文字列をラップして操作します。
 * 
 * セパレータは環境依存です。例えば、Windows では '\\'、UNIX 系では '/' です。
 * 入力された文字列に含まれるセパレータは元の文字を維持します。
 * セパレータをネイティブの文字に変換する場合は native() を使用します。
 * 
 * このクラスに含まれる比較機能は、動作中のファイルシステムによって大文字小文字の区別を決定します。
 * 
 * 
 * std::filesystem::path と似ていますが、内部エンコーディングは String と同一です。
 * プラットフォーム依存ではありません。
 * 
 * 
 * VS std::filesystem::path
 * ----------
 * ### 処理速度
 * - ln::String と頻繁に連携する場合は ln::Path のほうが処理速度的に有利です。
 *   std::filesystem::path の内部エンコーディングはプラットフォーム依存であるため、ln::String のエンコーディングと一致しない場合はオーバーヘッドが発生します。
 * - パスが変更されず、頻繁にネイティブの API をコールする (std::filesystem::exists 等) 場合、std::filesystem::path のほうが処理速度的に有利です。
 *   ln::FileSystem::exists() 等 Lumino のファイルシステム API は、ネイティブの API をコールする前にエンコーディングの変換が発生します。
 *   変換自体は文字数が 256 以下である場合 std::filesystem::path よりも 1.5 倍ほど高速ですが、頻繁に実行される場合は変換の発生しない std::filesystem::path が有利です。
 * 
 *   
 */
class Path {
public:
    static const Path Empty;

    Path();
    Path(const Path& path);

    // TODO: String&&

    /** 指定されたパス文字列から Path を構築します。 */
    Path(const Char* path);

    /** 指定されたパス文字列から Path を構築します。 */
    Path(const String& path);

    /** 指定されたパス文字列から Path を構築します。 */
    Path(const StringView& path);

    /** 指定されたパス文字列を連結して Path を構築します。relativePath が絶対パスである場合、basePath は無視されます。 */
    Path(const Path& basePath, const Char* relativePath);
    Path(const Path& basePath, const String& relativePath);
    Path(const Path& basePath, const StringView& relativePath);
    Path(const Path& basePath, const Path& relativePath);

    // operators
    Path& operator=(const String& str) {
        assign(str.c_str());
        return (*this);
    }
    Path& operator=(const Char* str) {
        assign(str);
        return (*this);
    }
    //bool operator < (const Path& right) const;
    //bool operator < (const Char* right) const;

#ifdef LN_STRING_FUZZY_CONVERSION
    Path(const char* path)
        : Path(String(path)) {
    }
#endif

    static Path fromStdPath(const std::filesystem::path& path);
    std::filesystem::path toStdPath() const;

    static Path fromUtf8(const std::string_view& s) { return Path(String::fromUtf8(s)); }
    std::string toUtf8() const { return m_path.toUtf8(); }
    static Path fromUtf16(const std::u16string_view& s) { return Path(String::fromUtf16(s)); }
    std::u16string toUtf16() const { return m_path.toUtf16(); }

public:
    /** パスが空であるかを確認します。*/
    bool isEmpty() const { return m_path.isEmpty(); }

    /** パス文字列の長さを返します。*/
    int length() const { return m_path.length(); }

    /** C言語形式の文字列ポインタを返します。*/
    const Char* c_str() const { return m_path.c_str(); }

    /** パス文字列を String として返します。*/
    const String& str() const { return m_path; }

    /** このパスが絶対パスであるかを確認します。 */
    bool isAbsolute() const;

    /** このパスが相対パスであるかを確認します。 */
    bool isRelative() const;

    /** このパスがルートパスであるかを確認します。 */
    bool isRoot() const;

    /** セパレータがすべて '/' であるかを確認します。 */
    bool isUnified() const;

    /** このパスが指定された拡張子を持っているかを確認します。 (ext の . の有無は問わない) */
    bool hasExtension(const StringView& ext = StringView()) const;

    /**
     * パス文字列の中から拡張子を含むファイル名の部分を返します。
     * ~~~
     * "dir/file.txt"       => "file.txt"
     * "file.txt"           => "file.txt"
     * "file"               => "file"
     * "dir/.git"           => "dir/.git"
     * "."                  => "."
     * ~~~
     */
    Path fileName() const;

    /** このパスから拡張子を取り除いたパスを返します。
     * ~~~
     * "/dir/file.txt"      => "/dir/file"
     * "file.txt"           => "file"
     * "file"               => "file"
     * "C:/dir.sub/file"    => "C:/dir.sub/file"
     * "dir/.git"           => "dir/"
     * ".git"               => ""
     * ""                   => ""
     * ~~~
     */
    Path withoutExtension() const;

    /**
     * パス文字列の中から拡張子を含まないファイル名の部分を返します。
     * ~~~
     * "dir/file.txt"       => "file"
     * "file.txt"           => "file"
     * "dir/.file"          => ""
     * ".file"              => ""
     * ~~~
     */
    StringView fileNameWithoutExtension() const;

    /**
     * ファイルの拡張子を取得します。
     * @param[in]    withDot : true の場合、結果は '.' を含み、false の場合は含まない
     * ~~~
     * "file.txt"           => ".txt"
     * "file.tmp.txt"       => ".txt"
     * "file"               => ""
     * ".file"              => ".file"
     * "."                  => ""
     * ""                   => ""
     * ~~~
     */
    StringView extension(bool withDot = true) const;

    /**
     * 親ディレクトリのパスを取得します。
     * ~~~
     * "dir1/file1.txt"     => "dir1"
     * "dir"                => ""
     * "dir/"               => "dir"
     * "/"                  => "/"
     * "C:"                 => "C:"
     * ""                   => ".."
     * ".."                 => "../.."
     * "."                  => ""
     * ~~~
    */
    Path parent() const;

    /** セパレータをネイティブの文字に変換したパスを返します。 */
    Path native() const;

    /** セパレータを '/' に変換したパスを返します。 */
    Path unify() const;

    /**
     * パスを単純化し、絶対パスにしたものを返します。
     * 
     * パスが相対パスの場合はカレントディレクトリを基準にして絶対パスを求めます。
     * また、セパレータをネイティブの文字に変換されます。
     */
    Path canonicalize() const;

    /**
     * このパスから指定したパスへの相対パスを取得します。
     * 
     * このパスと target はディレクトリの絶対パスである必要があります。
     */
    Path makeRelative(const Path& target) const;
    static Path makeRelative(const Path& base, const Path& target) { return Path(base).makeRelative(target); }

    /**
     * このパスの拡張子を変更した新しいパスを返します。
     * @param[in]    newExt    : 新しい拡張子 (先頭の . は省略しても良い)
     * ~~~
     * "file.txt" with "md"     => "file.md"
     * "file" with "md"         => "file.md"
     * "file.txt" with md"      => "file"
     * "dir/file.txt" with "md" => "dir/file.md"
     * ~~~
     */
    Path replaceExtension(const StringView& newExt) const;

    /** 末尾にひとつのセパレータがあるようにした新しいパスを返します。 */
    Path withEndSeparator() const;

    /** パス文字列をクリアします。 */
    void clear() { m_path.clear(); }

    /**
     * 現在のパスに別のパス文字列を連結します。
     * @param[in]   path    : パス文字列
     * 現在のパスの末尾にセパレータが無い場合は付加し、文字列を連結します。path が絶対パスであれば、現在のパスを置き換えます。         
     */
    void append(const StringView& path);

    /**
     * このパスと別のパス文字列が等しいかを確認します。
     * 
     * 字句表現としての等価性を評価します。2つのパスが同一のファイルシステムオブジェクトを指すかは確認しません。
     * 環境により、ディレクトリセパレータ ('/' と '\') や大文字小文字を同一視します。
     */
    static int compare(const StringView& path1, const StringView& path2);

    // 指定パスが this に含まれるかを調べる。双方 が絶対パスでなければならない。
    bool contains(const Path& subdir) const;

public:
    template<class... TArgs>
    static Path combine(TArgs&&... args);

    /**    
        @brief        フォルダ内でユニークなファイルパス(絶対パス)を生成して返す
        @param[in]    directory    : フォルダパス
        @param[in]    filePrefix    : ファイル名の接頭辞 または NULL
        @param[in]    extName        : ファイルの拡張子 (プレフィックス。.を含めること) または NULL
        @details    生成されるファイルパスは "<directory>/<filePrefix><ID><extName>" となります。
                    ID は時間情報をキーとして生成、<br>
                    filePrefix、extName が NULL の場合は空文字扱いで結合されます。
                    例えば両方 NULL にすると、番号だけのファイル名になります。
    */
    static Path getUniqueFilePathInDirectory(const Path& directory, const Char* filePrefix, const Char* extName);

#if 0
    /// (こちらはファイル名だけを返す)
    static String getUniqueFileNameInDirectory(const Path& directory, const Char* filePrefix, const Char* extName);
#endif

private:
    void assign(const StringView& path);
    void assignUnderBasePath(const Path& basePath, const StringView& relativePath);

    String m_path;
};

namespace detail {

template<class T>
inline Path combinePathImpl(T&& s) {
    return s;
}

template<class T1, class T2, class... TRest>
inline Path combinePathImpl(T1&& parent, T2&& local, TRest&&... rest) {
    return combinePathImpl(Path(std::forward<T1>(parent), std::forward<T2>(local)), std::forward<TRest>(rest)...);
}

} // namespace detail

template<class... TArgs>
inline Path Path::combine(TArgs&&... args) {
    return detail::combinePathImpl(std::forward<TArgs>(args)...);
}

inline Path operator+(const Char* lhs, const Path& rhs) {
    return String::concat(lhs, rhs.str());
}

inline bool operator==(const Path& lhs, const Path& rhs) {
    return Path::compare(lhs, rhs) == 0;
}
inline bool operator!=(const Path& lhs, const Path& rhs) {
    return !operator==(lhs, rhs);
}

inline Path operator/(const Path& x, const Path& y) {
    return Path::combine(x, y);
}

inline Path operator/(const Path& x, const Char* y) {
    return Path::combine(x, Path(y));
}

inline std::u32string_view toStdStringView(const Path& v) {
    return std::u32string_view(v.str().c_str(), v.str().length());
}

class StringView;

//namespace fmt {
//
//template<typename Formatter>
//void formatArg(Formatter& f, const ln::Path& value)
//{
//	f.getSB().appendString(value.str());
//}
//
//}

} // namespace ln

//#include "../Base/Formatter.h"
