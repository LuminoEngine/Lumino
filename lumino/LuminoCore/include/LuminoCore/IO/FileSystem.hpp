// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once
#include "../Base/RefObject.hpp"
#include "../Base/String.hpp"
#include "Common.hpp"
#include "Path.hpp"

namespace ln {
class TextEncoding;
class Stream;
class String;
class StringView;
class FileSystem;

namespace detail {
class DirectoryIterator2Impl;

enum class SearchTargetEntity
{
    File,
    Directory,
};

class DirectoryIterator2 : public std::iterator<std::forward_iterator_tag, int> {
public:
    DirectoryIterator2();
    DirectoryIterator2(const StringView& dirPath, const StringView& pattern, SearchOption searchOption, SearchTargetEntity targetEntity);
    DirectoryIterator2(const DirectoryIterator2& other);
    ~DirectoryIterator2();

    DirectoryIterator2& operator=(const DirectoryIterator2& other);
    DirectoryIterator2& operator++();   // prefix
    DirectoryIterator2 operator++(int); // postfix

    const Path& operator*() const { return m_current; }
    Path& operator*() { return m_current; }
    const Path* operator->() const { return &m_current; }
    Path* operator->() { return &m_current; }

    bool operator==(const DirectoryIterator2& othre) const { return m_current == othre.m_current; }
    bool operator!=(const DirectoryIterator2& othre) const { return m_current != othre.m_current; }

private:
    Ref<DirectoryIterator2Impl> m_impl;
    Path m_current;
};

} // namespace detail

enum class FileCopyOption
{
    None,
    Overwrite,
};

/** ファイルリストを STL の範囲ベースのインターフェイスとして列挙するためのクラスです。 */
class DirectoryIteratorRange
{
public:
    bool isEmpty() const { return m_begin == m_end; }
    detail::DirectoryIterator2 begin() { return m_begin; }
    detail::DirectoryIterator2 end() { return m_end; }

private:
    DirectoryIteratorRange(const StringView& dirPath, const StringView& pattern, SearchOption searchOption, detail::SearchTargetEntity targetEntity);
    detail::DirectoryIterator2 m_begin;
    detail::DirectoryIterator2 m_end;

    friend class FileSystem;
};

/** ファイルやディレクトリの作成、コピー、削除や列挙など、一般的な操作を提供します。 */
class FileSystem
{
public:
    /**
     * 指定されたファイルが存在するか確認します。
     * 
     * この関数はファイルが存在し、かつ必要なアクセス許可を持つ場合 true を返します。
     */
    static bool existsFile(const StringView& filePath);

    /**
     * ファイルの属性を取得します。
     * 
     * @param[in]    filePath    : ファイル名
     * @return        ファイルの属性 (FileAttribute のビットの組み合わせ)
     */
    static FileAttribute getAttribute(const StringView& filePath);

    /**
     * ファイルの属性を設定します。
     * 
     * @param[in]    filePath    : ファイル名
     * @param[in]    attr        : ファイルの属性 (FileAttribute のビットの組み合わせ)
     * 
     * この関数により変更できる属性は、読み取り属性のみです。(Unix では隠し属性はファイル名で表現されるためです)
     * それ以外のフラグビットは無視されます。
     */
    static void setAttribute(const StringView& filePath, FileAttribute attr);

    /**
     * ファイルをコピーします。
     * 
     * @param[in]    sourceFileName    : コピー元ファイル名
     * @param[in]    destFileName    : コピー先ファイル名
     * @param[in]    option        : コピー先を上書きするかどうか
     */
    static void copyFile(const StringView& sourceFileName, const StringView& destFileName, FileCopyOption option = FileCopyOption::None);

    /**
     * ファイルを削除します。
     * 
     * @param[in]    filePath        : 削除するファイルのパス
     * @details    削除するファイルが存在しない場合、例外はスローされません。
     */
    static void removeFile(const StringView& filePath);

    /**
     * 指定したディレクトリが存在するかを確認します。
     * @param[in]    path    : ディレクトリのパス
     */
    static bool existsDirectory(const StringView& path);

    /**
     * ディレクトリを作成します。
     * 
     * 指定したパスへの全てのディレクトリを作成します。
     * 例えば "a/b" を指定した場合、ディレクトリ a が無ければ作成してから、b を作成します。
     */
    static void createDirectory(const StringView& path);
    // TODO: createDirectory と createDirectories

    /**
     * ディレクトリを削除します。
     * 
     * @param[in]    path            : 削除するディレクトリのパス
     * @param[in]    recursive        : 全てのサブディレクトリも含めて削除する場合は true
     */
    static void removeDirectory(const StringView& path, bool recursive = false);

    /**
     * ディレクトリをコピーします。
     * 
     * @param[in]    srcPath    : コピー元ディレクトリパス
     * @param[in]    dstPath    : コピー先ディレクトリパス
     * @param[in]    overwrite        : コピー先のファイルとディレクトリを上書きする場合は true
     * @param[in]    recursive        : 全てのサブディレクトリも含めて削除する場合は true
     */
    static void copyDirectory(const StringView& srcPath, const StringView& dstPath, bool overwrite, bool recursive);

    /** パスのファイル名とファイル名パターンを照合します。 */
    static bool matchPath(const StringView& filePath, const StringView& pattern);

    /** ファイルサイズを取得します。 */
    static uint64_t getFileSize(const StringView& filePath);

    /**
     * ファイルの内容をバイナリ形式としてすべて読み込みます。 
     * @param[in]    filePath    : 読み込むファイルのパス
     * @return        読み込んだバイナリデータ
     */
    static ByteBuffer readAllBytes(const StringView& filePath);

    /**
     * ファイルの内容をテキスト形式としてすべて読み込ます。
     * 
     * @param[in]    filePath    : 読み込むファイルのパス
     * @param[in]    encoding    : ファイルのエンコーディング (省略した場合、UTF8 テキストとして読み込みます)
     * @return        読み込んだ文字列
     * 
     * ファイル先頭に UTF 系の BOM が含まれている場合、それに対応するエンコーディングで読み込みます。
     */
    static String readAllText(const StringView& filePath, TextEncoding* encoding = nullptr);

    static String readAllText(Stream* stream, TextEncoding* encoding = nullptr);

    /** 
     * 指定されたバッファの内容をバイナリデータとしてすべてファイルに書き込みます。 
     * 
     * @param[in]    filePath    : 書き込み先のファイルのパス
     * @param[in]    buffer        : バッファの先頭アドレス
     * @param[in]    size        : バッファのバイト数
     */
    static void writeAllBytes(const StringView& filePath, const void* buffer, size_t size);

    /**
     * 文字列をテキストデータとしてすべてファイルに書き込みます。 
     * 
     * @param[in]    filePath    : 書き込み先のファイルのパス
     * @param[in]    str    : ファイルに書き込む文字列
     * @param[in]    encoding    : 文字列に適用するエンコーディング (省略した場合、UTF8 テキストとして書き込みます)
     */
    static void writeAllText(const StringView& filePath, const String& str, TextEncoding* encoding = nullptr);

    /**
     * 指定したディレクトリ内の1つのファイルパス返します。
     *
     * @param[in]   dirPath    : 検索するディレクトリのパス
     * @param[in]   pattern    : ファイル名の検索文字列 (ワイルドカード (* および ?) を含めることができます。省略した場合はすべてのファイルを列挙します)
     * @return      ファイルが見つからない場合は空の Path を返します。
     *
     * これは getFiles() のユーティリティです。getFiles() で見つかった最初のファイルパスを返します。
     * 特定の拡張子を持つファイルを1つだけ検索したい場合に使用します。
     */
    static Path getFile(const StringView& dirPath, const StringView& pattern = StringView());

    /**
     * 指定したディレクトリ内のファイルパス返します。
     * 
     * @param[in]    dirPath    : 検索するディレクトリのパス
     * @param[in]    pattern    : ファイル名の検索文字列 (ワイルドカード (* および ?) を含めることができます。省略した場合はすべてのファイルを列挙します)
     * @param[in]    searchOption    : サブディレクトリも含めて検索するかどうか
     */
    static DirectoryIteratorRange getFiles(const StringView& dirPath, const StringView& pattern = StringView(), SearchOption searchOption = SearchOption::TopDirectoryOnly);

    /**
     * 指定したディレクトリ内のディレクトリパス返します。
     * 
     * @param[in]    dirPath    : 検索するディレクトリのパス
     * @param[in]    pattern    : ディレクトリ名の検索文字列 (ワイルドカード (* および ?) を含めることができます。省略した場合はすべてのファイルを列挙します)
     * @param[in]    searchOption    : サブディレクトリも含めて検索するかどうか
     */
    static DirectoryIteratorRange getDirectories(const StringView& dirPath, const StringView& pattern = StringView(), SearchOption searchOption = SearchOption::TopDirectoryOnly);

private:
    static bool mkdir(const char* path);
    static bool mkdir(const wchar_t* path);
};

namespace detail {

class FileSystemInternal
{
public:
    // TODO: ↓ StringView でいい気が

    static bool existsFile(const char* filePath, int len);
    static bool existsFile(const wchar_t* filePath, int len);
    static bool existsFile(const Char* filePath, int len);

    static FileAttribute getAttribute(const char* filePath, int len);
    static FileAttribute getAttribute(const wchar_t* filePath, int len);
    static FileAttribute getAttribute(const Char* filePath, int len);

    static void setAttribute(const char* filePath, int len, FileAttribute attr);
    static void setAttribute(const wchar_t* filePath, int len, FileAttribute attr);
    static void setAttribute(const Char* filePath, int len, FileAttribute attr);

    static time_t getLastModifiedTime(const char* filePath, int len);
    static time_t getLastModifiedTime(const wchar_t* filePath, int len);
    static time_t getLastModifiedTime(const Char* filePath, int len);

    static void copyFile(const char* sourceFileName, int sourceFileNameLen, const char* destFileName, int destFileNameLen, FileCopyOption option);
    static void copyFile(const wchar_t* sourceFileName, int sourceFileNameLen, const wchar_t* destFileName, int destFileNameLen, FileCopyOption option);
    static void copyFile(const Char* sourceFileName, int sourceFileNameLen, const Char* destFileName, int destFileNameLen, FileCopyOption option);

    static void removeFile(const char* filePath, int len);
    static void removeFile(const wchar_t* filePath, int len);
    static void removeFile(const Char* filePath, int len);

    static bool existsDirectory(const char* path, int len);
    static bool existsDirectory(const wchar_t* path, int len);
    static bool existsDirectory(const Char* path, int len);

    static void createDirectory(const char* path, int len);
    static void createDirectory(const wchar_t* path, int len);
    static void createDirectory(const Char* path, int len);

    static void removeDirectory(const char* path, int len, bool recursive);
    static void removeDirectory(const wchar_t* path, int len, bool recursive);
    static void removeDirectory(const Char* path, int len, bool recursive);

    static void copyDirectory(const char* srcPath, int srcPathLen, const char* dstPath, int dstPathLen, bool overwrite, bool recursive);
    static void copyDirectory(const wchar_t* srcPath, int srcPathLen, const wchar_t* dstPath, int dstPathLen, bool overwrite, bool recursive);
    static void copyDirectory(const Char* srcPath, int srcPathLen, const Char* dstPath, int dstPathLen, bool overwrite, bool recursive);

    static bool matchPath(const char* path, int pathLen, const char* pattern, int patternLen);
    static bool matchPath(const wchar_t* path, int pathLen, const wchar_t* pattern, int patternLen);
    static bool matchPath(const Char* path, int pathLen, const Char* pattern, int patternLen);

    static FILE* fopen(const char* path, int pathLen, const char* mode, int modeLen);
    static FILE* fopen(const wchar_t* path, int pathLen, const wchar_t* mode, int modeLen);
    static FILE* fopen(const Char* path, int pathLen, const Char* mode, int modeLen);

    static uint64_t getFileSize(FILE* stream);
    static int64_t calcSeekPoint(int64_t curPoint, int64_t maxSize, int64_t offset, int origin);
};

class FileGlob
{
public:
    static void glob(const Path& rootDir, const Path& pathspec, const List<String>& filters, bool recursive, List<Path>* outPathes);
    static bool filterFilePath(const List<String>& filters, const String& filePath);
};

} // namespace detail
} // namespace ln
