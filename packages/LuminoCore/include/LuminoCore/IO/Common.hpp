// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once

#define LN_USE_STD_FILESYSTEM 1
#ifdef LN_USE_STD_FILESYSTEM
#include <filesystem>
#endif

#include "../Base/EnumFlags.hpp"

namespace ln {

#if defined(LN_OS_WIN32)
using PathChar = wchar_t;
using PathString = std::wstring;
#else
using PathChar = char;
using PathString = std::string;
#endif

/** ファイルとディレクトリの属性 */
enum class FileAttribute : ln::FlagsType
{
	None = 0x0000,

	/** 特に属性を持たない通常のファイル */
	Normal = 0x0001,

	/** ディレクトリ */
	Directory = 0x0002,

	/** 読み取り専用 */
	ReadOnly = 0x0004,

	/** 隠しファイル */
	Hidden = 0x0008,

	All = 0xFFFF,
};
LN_FLAGS_OPERATORS(FileAttribute);

/** ファイルを開く際のアクセスモード */
enum class FileOpenMode : ln::FlagsType
{
	None = 0x0000,

	/** 読み取りアクセス */
	Read = 0x0001,

	/** 書き込みアクセス */
	Write = 0x0002,

	/** 読み取り及び書き込みアクセス */
	ReadWrite = Read | Write,

	/** ファイルを開き、ファイルの末尾をシークする (追記モード) */
	Append = 0x0004,

	/** ファイルが存在する場合、サイズを 0 にする */
	Truncate = 0x0008,

	/** (deprecated) ファイルを遅延モードで開く*/
	Deferring = 0x0100,

};
LN_FLAGS_OPERATORS(FileOpenMode);

/** ファイルやディレクトリの検索時のサブフォルダの扱い */
enum class SearchOption
{
	/** トップディレクトリのみを検索する */
	TopDirectoryOnly,

	/** サブディレクトリもすべて検索する */
	Recursive,
};

/** シーク位置の指定の基準 */
enum class SeekOrigin
{
	/** ストリームの先頭 */
	Begin,

	/** ストリームの現在位置 */
	Current,

	/** ストリームの末尾 */
	End,
};

/** ファイルを書き込みモードでオープンする際の動作 */
enum FileWriteMode
{
	/** ファイルが存在する場合、サイズを 0 にする */
	Truncate,

	/** ファイルを開き、ファイルの末尾をシークする (追記モード) */
	Append,
};

} // namespace ln

// TODO: 間髪入れずに fclose → fopen,fread とかすると落ちる。その回避。詳細調査は TODO
#define LN_EMSCRIPTEN_LAYZY_FLASH	printf("")
