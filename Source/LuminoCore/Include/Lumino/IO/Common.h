
#pragma once
#include "../Base/EnumFlags.h"
#include "../Base/Common.h"

LN_NAMESPACE_BEGIN

/** ファイルとディレクトリの属性 */
LN_ENUM_FLAGS(FileAttribute)
{
	None		= 0x0000,
	Normal		= 0x0001,	/**< 特に属性を持たない通常のファイル */
	Directory	= 0x0002,	/**< ディレクトリ */
	ReadOnly	= 0x0004,	/**< 読み取り専用 */
	Hidden		= 0x0008,	/**< 隠しファイル */
	All			= 0xFFFF,
};
LN_ENUM_FLAGS_DECLARE(FileAttribute)

//
//
//
//template<typename TEnum>
//class EnumFlags
//{
//public:
//	typedef TEnum enum_type;
//
//	//EnumFlags()
//};
//
//#define LN_DECLARE_FLAGS(Flags, Enum) typedef EnumFlags<Enum> Flags;
//
//#define LN_DECLARE_OPERATORS_FOR_FLAGS(FlagsEnumClass) \
//	inline EnumFlags<FlagsEnumClass::enum_type> operator|(FlagsEnameType::enum_type f1, FlagsEnumClass::enum_type f2) throw() \
//	{ \
//		return EnumFlags<FlagsEnumClass::enum_type>(f1) | f2; \
//	}
//	//inline EnumFlags<FlagsEnumClass::enum_type> operator|(FlagsEnumClass::enum_type f1, EnumFlags<FlagsEnumClass::enum_type> f2) throw() \
//	//{ \
//	//	return f2 | f1; \
//	//}

/** ファイルを開く際のアクセスモードを表します。*/
LN_ENUM_FLAGS(FileOpenMode)
{
	None = 0x0000,
	read = 0x0001,				/**< 読み取りアクセス*/
	write = 0x0002,				/**< 書き込みアクセス*/
	ReadWrite = read | write,	/**< 読み取り及び書き込みアクセス*/

	append = 0x0004,			/**< ファイルを開き、ファイルの末尾をシークする (追記モード)*/
	Truncate = 0x0008,			/**< ファイルが存在する場合、サイズを 0 にする*/

	Deferring = 0x0100,			/**< ファイルを遅延モードで開く*/
};
LN_ENUM_FLAGS_DECLARE(FileOpenMode);


///< ファイルを書き込みモードでオープンする際の概要
enum FileWriteMode
{
	FileWriteMode_Truncate = 0,				///< ファイルが存在する場合、サイズを 0 にする
	FileWriteMode_Append,					///< ファイルを開き、ファイルの末尾をシークする (追記モード)
};

#if 0
/// ファイルを開く方法または作成する方法
enum FileMode
{
	FileMode_Create = 0,		///< 新しいファイルを作成する。既に存在する場合は上書きされる
	FileMode_Open,				///< 既存のファイルを開く
	FileMode_Append,			///< ファイルを開き、ファイルの末尾をシークする。存在しない場合は新しいファイルを作成。

	FileMode_Max,				///< (terminator)
};

/// ファイルにアクセスする方法
enum FileAccess
{
	FileAccess_Read = 0,		///< 読み取りアクセス
	FileAccess_ReadWrite,		///< 読み取り及び書き込みアクセス
	FileAccess_Write,			///< 書き込みアクセス

	FileAccess_Max,				///< (terminator)
};

/// ファイルの共有方法
enum FileShare
{
	FileShare_None = 0,			///< 他のすべての読み書きを拒否する
	FileShare_Read,				///< 他が読み取り専用でファイルを開くことを許可する
	FileShare_Write,			///< 他が書き込み専用でファイルを開くことを許可する
	FileShare_ReadWrite,		///< 他が読み取りまたは書き込みでファイルを開くことを許可する (すべての共有を許可する)

	FileShare_Max,				///< (terminator)
};
#endif

/// ファイルへのアクセス優先度
enum FileAccessPriority
{
	FileAccessPriority_ArchiveFirst = 0,	///< アーカイブ優先
	FileAccessPriority_DirectoryFirst,		///< ディレクトリ優先
	FileAccessPriority_ArchiveOnly,			///< アーカイブのみ   

	FileAccessPriority_TERMINATOR,
};

/// シーク位置の指定の基準
enum SeekOrigin
{
	SeekOrigin_Begin	= SEEK_SET,			/// ストリームの先頭
	SeekOrigin_Current	= SEEK_CUR,			/// ストリームの現在位置
	SeekOrigin_End		= SEEK_END,			/// ストリームの末尾
};

/// 非同期処理の状態
enum ASyncIOState
{
	ASyncIOState_Idle = 0,					///< 待機状態 (初期状態)
	ASyncIOState_Ready,						///< 実行可能状態 (実行待ちキューに入っている状態)
	ASyncIOState_Processing,				///< 実行状態 (処理中)
	ASyncIOState_Completed,					///< 終了状態 (正常終了)
	ASyncIOState_Failed,					///< 終了状態 (処理終了後・処理に失敗した)

	ASyncIOState_Max,
};

/** プロセスの状態 */
LN_ENUM(ProcessStatus)
{
	Running = 0,			/**< 実行中 */
	Finished,				/**< 正常終了した */
	Crashed,				/**< 異常終了した */
};
LN_ENUM_DECLARE(ProcessStatus);

/** 特別なフォルダのパスを取得を取得する際のオプションです。*/
LN_ENUM(SpecialFolderOption)
{
	None = 0,				/**< フォルダが存在しない場合空の文字列を返します。*/
	Create,					/**< フォルダが存在しない場合は作成します。*/
};
LN_ENUM_DECLARE(SpecialFolderOption);

LN_NAMESPACE_END
