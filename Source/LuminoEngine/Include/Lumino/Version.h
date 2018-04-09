/*
	this file was automatically generated. plise do not change.
	このファイルは自動生成されました。変更しないでください。
*/

#pragma once

/** メジャーバージョン */
#define LUMINO_VERSION_MAJOR		0

/** マイナーバージョン */
#define LUMINO_VERSION_MINOR		3

/** リビジョンバージョン */
#define LUMINO_VERSION_REVISION		0

/** ビルドバージョン */
#define LUMINO_VERSION_BUILD		0

/** バージョン文字列 */
#define LUMINO_VERSION_STRING		_LT("0.3.0")

LN_NAMESPACE_BEGIN

/**
	@brief		ライブラリのバージョン情報です。
	@details	このクラスから取得できる値はライブラリのバイナリファイルのバージョン番号です。
				ヘッダファイルのバージョンやプリプロセスで使用したい場合は 
				LUMINO_VERSION_MAJOR や LUMINO_VERSION_MINOR を使用してください。
*/
class Version
{
public:

	/**
		@brief	メジャーバージョンを取得します。
	*/
	static int getMajor();

	/**
		@brief	マイナーバージョンを取得します。
	*/
	static int getMinor();

	/**
		@brief	リビジョンバージョンを取得します。
	*/
	static int getRevision();

	/**
		@brief	ビルドバージョンを取得します。
	*/
	static int getBuild();

	/**
		@brief	バージョン文字列の取得を取得します。
	*/
	static const TCHAR* getString();

	/**
		@brief	指定したバージョン番号と、ライブラリファイルのコンパイルバージョン番号を比較します。
		@return	指定バージョン >= コンパイルバージョン である場合、true を返します。
	*/
	static bool isAtLeast(int major = LUMINO_VERSION_MAJOR, int minor = LUMINO_VERSION_MINOR, int revision = LUMINO_VERSION_REVISION);

};

LN_NAMESPACE_END

