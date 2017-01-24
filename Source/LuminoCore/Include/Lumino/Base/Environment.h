/**
	@file	Environment.h
*/

#pragma once
#include "EnumExtension.h"

LN_NAMESPACE_BEGIN

/** エンディアンを示す値 */
enum class ByteOrder
{
	Little = 0,		/**< リトルエンディアン */
	Big,			/**< ビッグエンディアン */
};

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
LN_ENUM(SpecialFolder)
{
	ApplicationData = 0,
	Temporary,
};
LN_ENUM_DECLARE(SpecialFolder);

/**
	@brief	現在の環境に関する情報にアクセスするためのクラスです。
*/
class Environment
{
public:
#pragma push_macro("GetEnvironmentVariable")
#undef GetEnvironmentVariable
	/**
		@brief		環境変数の値を取得します。
		@param[in]	variableName	: 環境変数の名前
		@exception	KeyNotFoundException
	*/
	static String GetEnvironmentVariable(const String& variableName);
	static String LN_AFX_FUNCNAME(GetEnvironmentVariable)(const String& variableName);
#pragma pop_macro("GetEnvironmentVariable")

	/**
		@brief		環境変数の値を取得します。
		@param[in]	variableName	: 環境変数の名前
		@param[out]	outValue		: 環境変数の値を格納する変数のポインタ
		@return		環境変数が存在し、値が取得できた場合は true。
	*/
	static bool TryGetEnvironmentVariable(const String& variableName, String* outValue);

	/**
		@brief		現在の環境のエンディアンを確認します。
	*/
	static ByteOrder GetByteOrder();

	/**
		@brief		リトルエンディアン環境であるかを確認します。
	*/
	static bool IsLittleEndian();

	/**
		@brief		システム起動からの時間を ms 単位で取得します。
	*/
	static uint64_t GetTickCount();

	/**
		@brief		システム起動からの時間を ns 単位で取得します。
	*/
	static uint64_t GetTickCountNS();

	/**
		@brief		現在の環境で定義されている改行文字列を取得します。
	*/
	template<typename TChar>
	static const TChar* GetNewLine();

	/**
		@brief		システムの特別なフォルダのパスを取得します。
		@see	PathName.GetSpecialFolderPath
	*/
	template<typename TChar>
	static void GetSpecialFolderPath(SpecialFolder specialFolder, TChar* outPath);
};

LN_NAMESPACE_END
