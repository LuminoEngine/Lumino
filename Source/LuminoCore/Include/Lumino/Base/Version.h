/**
	@file	Version.h
*/
#ifndef LUMINO_CORE_VERSION_H
#define LUMINO_CORE_VERSION_H

#include "Common.h"

#define LUMINO_NAME					"Lumino"	///< ライブラリ名
#define LUMINO_CORE_VERSION_MAJOR	0			///< メジャーバージョン
#define LUMINO_CORE_VERSION_MINOR	0			///< マイナーバージョン
#define LUMINO_CORE_VERSION_PATCH	0			///< パッチ番号
#define LUMINO_CORE_VERSION_STRING	"0.0.0"		///< バージョン文字列

LN_NAMESPACE_BEGIN
namespace Version
{

/**
	@brief		コアライブラリのバージョン情報です。
	@details	このクラスから取得できる値はライブラリのバイナリファイルのバージョン番号です。
				ヘッダファイルのバージョンやプリプロセスで使用したい場合は 
				LUMINO_CORE_VERSION_MAJOR や LUMINO_CORE_VERSION_MINOR を使用してください。
*/
class Core
{
public:

	/**
		@brief	メジャーバージョンを取得します。
	*/
	static int GetMajor();

	/**
		@brief	マイナーバージョンを取得します。
	*/
	static int GetMinor();

	/**
		@brief	パッチ番号を取得します。
	*/
	static int GetPatch();

	/**
		@brief	バージョン文字列を取得します。
	*/
	static const char* GetVersionString();

	/**
		@brief	指定したバージョン番号と、ライブラリファイルのコンパイルバージョン番号を比較します。
		@return	指定バージョン >= コンパイルバージョン である場合、true を返します。
	*/
	static bool IsAtLeast(int major = LUMINO_CORE_VERSION_MAJOR, int minor = LUMINO_CORE_VERSION_MINOR, int patch = LUMINO_CORE_VERSION_PATCH);
};

} // namespace Version
LN_NAMESPACE_END

#endif // LUMINO_CORE_VERSION_H
