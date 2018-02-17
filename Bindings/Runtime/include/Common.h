
#pragma once

/*
	次のエラーコードは lwig でパースするために用意したもの。
	この enum 自体を公開するものではない。
*/

/** 結果・エラーコード */
LN_ENUM()
enum class ResultCode
{
	/** 成功 */
    OK							= 0,

	/** 不明なエラー */
	ErrorUnknown				= 1,

	/** 前提条件の検証エラー */
	ErrorVERIFY					= 2,

	/** 引数が無効 */
	ErrorARGUMENT				= 3,

	/** オブジェクトの現在の状態に対して無効な呼び出しが行われた */
	ErrorINVALID_OPERATION		= 4,

	/** 未実装の機能を呼び出した */
	ErrorNOT_IMPLEMENTED		= 5,

	/** メモリ確保に失敗 */
	ErrorOUT_OF_MEMORY			= 6,

	/** 値が有効な範囲に存在しない */
	ErrorOUT_OF_RANGE			= 7,

	/** 検索に使用したキーが存在しない */
	ErrorKEY_NOT_FOUND			= 8,

	/** 数値演算によるオーバーフローが発生した */
	ErrorOVERFLOW				= 9,

	/** その他のIOエラー */
	ErrorIO						= 10,

	/** ファイルにアクセスしようとして失敗した */
	ErrorFILE_NOT_FOUND			= 11,

	/** 無効なディレクトリにアクセスしようとした */
	ErrorDIRECTORY_NOT_FOUND	= 12,

	/** ファイルや文字列等の形式が不正 */
	ErrorINVALID_FORMAT			= 13,

	/** ストリームの末尾を越えてアクセスしようとした */
	ErrorEND_OF_STREAM			= 14,

	/** 文字コードの変換中、マッピングできない文字または不正シーケンスが見つかった */
	ErrorENCODING				= 15,

	/** WindowsAPI のエラー */
	ErrorWIN32					= 16,

	/** COM のエラー */
	ErrorCOM					= 17,
};

typedef int		LNHandle;

//#ifdef __cplusplus
//extern "C" {
//#endif // __cplusplus
//
////==============================================================================
//// Common
////==============================================================================
//
//typedef int		LNHandle;
//

//
///** 結果・エラーコード */
//typedef enum tagLNResult
//{
//    LN_OK							= 0,	/**< 成功 */
//	LN_ERROR_UNKNOWN				= -1,	/**< 不明なエラー */
//	LN_ERROR_VERIFY					= -2,	/**< 前提条件の検証エラー */
//	LN_ERROR_ARGUMENT				= -3,	/**< 引数が無効 */
//	LN_ERROR_INVALID_OPERATION		= -4,	/**< オブジェクトの現在の状態に対して無効な呼び出しが行われた */
//	LN_ERROR_NOT_IMPLEMENTED		= -5,	/**< 未実装の機能を呼び出した */
//	LN_ERROR_OUT_OF_MEMORY			= -6,	/**< メモリ確保に失敗 */
//	LN_ERROR_OUT_OF_RANGE			= -7,	/**< 値が有効な範囲に存在しない */
//	LN_ERROR_KEY_NOT_FOUND			= -8,	/**< 検索に使用したキーが存在しない */
//	LN_ERROR_OVERFLOW				= -9,	/**< 数値演算によるオーバーフローが発生した */
//	LN_ERROR_IO						= -10,	/**< その他のIOエラー */
//	LN_ERROR_FILE_NOT_FOUND			= -11,	/**< ファイルにアクセスしようとして失敗した */
//	LN_ERROR_DIRECTORY_NOT_FOUND	= -12,	/**< 無効なディレクトリにアクセスしようとした */
//	LN_ERROR_INVALID_FORMAT			= -13,	/**< ファイルや文字列等の形式が不正 */
//	LN_ERROR_END_OF_STREAM			= -14,	/**< ストリームの末尾を越えてアクセスしようとした */
//	LN_ERROR_ENCODING				= -15,	/**< 文字コードの変換中、マッピングできない文字または不正シーケンスが見つかった */
//	LN_ERROR_WIN32					= -16,	/**< WindowsAPI のエラー */
//	LN_ERROR_COM					= -17,	/**< COM のエラー */
//
//} LNResult;
//
//#ifdef __cplusplus
//} // extern "C"
//#endif // __cplusplus



