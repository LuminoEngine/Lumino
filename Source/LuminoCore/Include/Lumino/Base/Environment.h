/**
	@file	Environment.h
*/

#pragma once
#include "EnumFlags.h"

LN_NAMESPACE_BEGIN

/**
	@brief	システムの特別なフォルダパスを示す値です。
	@details
		SpecialFolder   | Windows                   | OS X    | Linux
		----------------|---------------------------|---------|---------
		ApplicationData | <USER>/AppData/Roaming    |         | ~/.local/share
		                |                           |         | /usr/local/share
		                |                           |         | /usr/share/
		Temporary       | <USER>/AppData/Local/Temp |         | /tmp
*/
enum class SpecialFolder
{
	ApplicationData = 0,
	Temporary,
};

LN_NAMESPACE_END


LN_NAMESPACE_BEGIN

/** エンディアンを示す値 */
enum class ByteOrder
{
	Little = 0,		/**< リトルエンディアン */
	Big,			/**< ビッグエンディアン */
};

/**
	@brief	現在の環境に関する情報にアクセスするためのクラスです。
*/
class Environment
{
public:
	static String getCurrentDirectory();


	/**
		@brief		環境変数の値を取得します。
		@param[in]	variableName	: 環境変数の名前
		@exception	KeyNotFoundException
	*/
	static String getEnvironmentVariable(const String& variableName);
	static String LN_AFX_FUNCNAME(getEnvironmentVariable)(const String& variableName);

	/**
		@brief		環境変数の値を取得します。
		@param[in]	variableName	: 環境変数の名前
		@param[out]	outValue		: 環境変数の値を格納する変数のポインタ
		@return		環境変数が存在し、値が取得できた場合は true。
	*/
	static bool tryGetEnvironmentVariable(const String& variableName, String* outValue);

	/**
		@brief		現在の環境のエンディアンを確認します。
	*/
	static ByteOrder getByteOrder();

	/**
		@brief		リトルエンディアン環境であるかを確認します。
	*/
	static bool isLittleEndian();

	/**
		@brief		システム起動からの時間を ms 単位で取得します。
	*/
	static uint64_t getTickCount();

	/**
		@brief		システム起動からの時間を ns 単位で取得します。
	*/
	static uint64_t getTickCountNS();

	/**
		@brief		現在の環境で定義されている改行文字列を取得します。
	*/
	template<typename TChar>
	static const TChar* getNewLine();

	/**
		@brief		システムの特別なフォルダのパスを取得します。
	*/
	static String getSpecialFolderPath(SpecialFolder specialFolder);
};

LN_NAMESPACE_END
