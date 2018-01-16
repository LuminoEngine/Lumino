/**
	@file	FileSystem.h
*/
#pragma once
#include <stdio.h>
#include "../Base/RefObject.h"
#include "../Base/String.h"
#include "../Base/EnumFlags.h"
#include "../Base/Enumerable.h"
#include "DirectoryUtils.h"

LN_NAMESPACE_BEGIN
class Encoding;
class Stream;
class String;
class StringRef;

/**
	@brief	ファイルユーティリティ
*/
class FileSystem
{
public:
	
	/** 現在の環境のファイルシステムが、パス文字列の大文字と小文字を区別するかを確認します。 */
	static CaseSensitivity getFileSystemCaseSensitivity();

	/**
		@brief		指定されたファイルが存在するか確認します。
		@details	この関数は指定されたファイルにアクセスできる場合に true を返します。
					例えば Windows では、実際にファイルが存在していてもそのファイルが
					別のパスワードロックされたユーザーフォルダ内に存在している場合は false を返します。
					(ユーザーA は ユーザーB の "マイドキュメント" フォルダのファイルにアクセスできない)
					また、パスが空文字や NULL の場合も false を返します。
	*/
	static bool existsFile(const StringRef& filePath);

	/**
		@brief		ファイルの属性を取得します。
		@param[in]	filePath		: ファイル名
		@return		ファイルの属性 (FileAttribute のビットの組み合わせ)
	*/
	static FileAttribute getAttribute(const StringRef& filePath);

	/**
		@brief		ファイルの属性を設定します。
		@param[in]	filePath		: ファイル名
		@param[in]	attr			: ファイルの属性 (FileAttribute のビットの組み合わせ)
		@details	この関数により変更できる属性は、読み取り属性のみです。(Unix では隠し属性はファイル名で表現されるためです)
					それ以外のフラグビットは無視されます。
	*/
	static void setAttribute(const StringRef& filePath, FileAttribute attr);

	/**
		@brief		ファイルをコピーする
		@param[in]	sourceFileName	: コピー元ファイル名
		@param[in]	destFileName	: コピー先ファイル名
		@param[in]	overwrite		: コピー先を上書きする場合は true
		@details	読み取り専用ファイルに上書きすることはできません。
	*/
	static void copyFile(const StringRef& sourceFileName, const StringRef& destFileName, bool overwrite);

	/**
		@brief		ファイルを削除する
		@param[in]	filePath		: 削除するファイルのパス
		@details	削除するファイルが存在しない場合、例外はスローされません。
	*/
	static void deleteFile(const StringRef& filePath);

	/**
		@brief		指定したディレクトリが存在するかを確認します。
		@param[in]	path	: ディレクトリのパス
		@return		ディレクトリが存在すれば true。それ以外の場合は false。
	*/
	static bool existsDirectory(const StringRef&path);

	/**
		@brief		ディレクトリを作成します。
		@param[in]	path	: 作成するディレクトリのパス
		@details	指定したパスへの全てのディレクトリを作成します。
					既に存在する場合は作成しません。
	*/
	static void createDirectory(const StringRef& path);

	/**
		@brief		ディレクトリを削除します。
		@param[in]	path			: 削除するディレクトリのパス
		@param[in]	recursive		: 
	*/
	static void deleteDirectory(const StringRef& path, bool recursive);

	/**
		@brief		
		@param[in]	overwrite		: コピー先のファイルとディレクトリを上書きする場合は true
	*/
	static void copyDirectory(const StringRef& srcPath, const StringRef& dstPath, bool overwrite, bool recursive);

	/** パスのファイル名と1つ以上のファイル名パターンを照合します。 */
	static bool matchPath(const StringRef& filePath, const StringRef& pattern);

	/** ファイルサイズを取得します。 */
	static uint64_t getFileSize(const StringRef& filePath);

	/** ファイルサイズを取得します。 */
	static uint64_t getFileSize(FILE* stream);

	/** ファイルの内容をすべて読み込みます。 (バイナリ形式)  */
	static ByteBuffer readAllBytes(const StringRef& filePath);






	/// 
	/// encoding 省略時は UTF8(BOM 無し)
	/**
		@brief		ファイルの内容をすべて読み込み、文字列として返します。
		@param[in]	filePath	: 読み込むファイルのパス
		@param[in]	encoding	: ファイルのエンコーディング
		@return		読み込んだ文字列
		@details	encoding が nullptr である場合、UTF8 テキストとして読み込みます。
					BOM の有無は自動判別します。
	*/
	static String readAllText(const StringRef& filePath, Encoding* encoding = nullptr);

	static String readAllText(Stream* stream, Encoding* encoding = nullptr);

	/// 配列の内容をバイナリファイルとして書き出す
	static void writeAllBytes(const TCHAR* filePath, const void* buffer, size_t size);

	/// 文字列をテキストファイルとして書き出す
	/// encoding 省略時は UTF8 (BOM 無し)
	static void writeAllText(const TCHAR* filePath, const String& str, Encoding* encoding = nullptr);


	static tr::Enumerator<PathName> getFiles(const StringRef& dirPath, const StringRef& pattern = StringRef());

	static tr::Enumerator<PathName> enumerateFiles(const StringRef& dirPath, const StringRef& pattern = StringRef());

	static void getCurrentDirectory(String* outPath);

private:
	static bool mkdir(const char* path);
	static bool mkdir(const wchar_t* path);
	static bool getAttributeInternal(const char* path, FileAttribute* outAttr);
	static bool getAttributeInternal(const wchar_t* path, FileAttribute* outAttr);
	template<typename TChar> static void createDirectoryInternal(const TChar* path);
};

namespace detail {

class FileSystemInternal
{
public:
	static bool existsFile(const char* filePath, int len);
	static bool existsFile(const wchar_t* filePath, int len);
	static bool existsFile(const char16_t* filePath, int len);

	static FileAttribute getAttribute(const char* filePath, int len);
	static FileAttribute getAttribute(const wchar_t* filePath, int len);
	static FileAttribute getAttribute(const char16_t* filePath, int len);

	static void setAttribute(const char* filePath, int len, FileAttribute attr);
	static void setAttribute(const wchar_t* filePath, int len, FileAttribute attr);
	static void setAttribute(const char16_t* filePath, int len, FileAttribute attr);

	static void copyFile(const char* sourceFileName, int sourceFileNameLen, const char* destFileName, int destFileNameLen, bool overwrite);
	static void copyFile(const wchar_t* sourceFileName, int sourceFileNameLen, const wchar_t* destFileName, int destFileNameLen, bool overwrite);
	static void copyFile(const char16_t* sourceFileName, int sourceFileNameLen, const char16_t* destFileName, int destFileNameLen, bool overwrite);

	static void deleteFile(const char* filePath, int len);
	static void deleteFile(const wchar_t* filePath, int len);
	static void deleteFile(const char16_t* filePath, int len);

	static bool existsDirectory(const char* path, int len);
	static bool existsDirectory(const wchar_t* path, int len);
	static bool existsDirectory(const char16_t* path, int len);

	static void createDirectory(const char* path, int len);
	static void createDirectory(const wchar_t* path, int len);
	static void createDirectory(const char16_t* path, int len);

	static void deleteDirectory(const char* path, int len, bool recursive);
	static void deleteDirectory(const wchar_t* path, int len, bool recursive);
	static void deleteDirectory(const char16_t* path, int len, bool recursive);

	static void copyDirectory(const char* srcPath, int srcPathLen, const char* dstPath, int dstPathLen, bool overwrite, bool recursive);
	static void copyDirectory(const wchar_t* srcPath, int srcPathLen, const wchar_t* dstPath, int dstPathLen, bool overwrite, bool recursive);
	static void copyDirectory(const char16_t* srcPath, int srcPathLen, const char16_t* dstPath, int dstPathLen, bool overwrite, bool recursive);

	static bool matchPath(const char* path, int pathLen, const char* pattern, int patternLen);
	static bool matchPath(const wchar_t* path, int pathLen, const wchar_t* pattern, int patternLen);
	static bool matchPath(const char16_t* path, int pathLen, const char16_t* pattern, int patternLen);

	static FILE* fopen(const char* path, int pathLen, const char* mode, int modeLen);
	static FILE* fopen(const wchar_t* path, int pathLen, const wchar_t* mode, int modeLen);
	static FILE* fopen(const char16_t* path, int pathLen, const char16_t* mode, int modeLen);

	static int64_t calcSeekPoint(int64_t curPoint, int64_t maxSize, int64_t offset, int origin);
};

} // namespace detail

LN_NAMESPACE_END
