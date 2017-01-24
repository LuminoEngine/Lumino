/**
	@file	Logger.h
*/
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief		ログファイルのユーティリティです。
*/
class Logger
{
public:

	/** 通知レベル */
	enum class Level
	{
		Info = 0,		/**< 情報 */
		Warning ,		/**< 警告 */
		Error,			/**< エラー */
	};

public:

	/**
		@brief		ログ機能を初期化し、ログファイルを新規作成します。
		@param[in]	filePath	: ログファイルのパス
		@return		true=成功 / false=失敗
	*/
	static bool Initialize(const TCHAR* filePath) throw();

	/**
		@brief		通知レベルを指定して書式指定メッセージを書き込みます。
		@param[in]	level	: 通知レベル (指定しない場合は Level_Info)
		@details	ログ機能が初期化されていない場合は何もしません。
	*/
	static void WriteLine(Level level, const char* format, ...) throw();
	static void WriteLine(Level level, const wchar_t* format, ...) throw();		///< @overload WriteLine
	static void WriteLine(const char* format, ...) throw();						///< @overload WriteLine
	static void WriteLine(const wchar_t* format, ...) throw();					///< @overload WriteLine

};

LN_NAMESPACE_END
