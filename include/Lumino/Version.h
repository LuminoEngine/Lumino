
#ifndef LUMINO_ENGINE_VERSION_H
#define LUMINO_ENGINE_VERSION_H

/** メジャーバージョン */
#define LUMINO_ENGINE_VERSION_MAJOR			0

/** マイナーバージョン */
#define LUMINO_ENGINE_VERSION_MINOR			2

/** リビジョンバージョン */
#define LUMINO_ENGINE_VERSION_REVISION		0

/** ビルドバージョン */
#define LUMINO_ENGINE_VERSION_BUILD			0/*$(BuildVer)*/

/** バージョン文字列 */
#define LUMINO_ENGINE_VERSION_STRING		_T("0.2.0.$(BuildVer)")

LN_NAMESPACE_BEGIN

namespace Version
{
	
/**
	@brief		ライブラリのバージョン情報です。
	@details	このクラスから取得できる値はライブラリのバイナリファイルのバージョン番号です。
				ヘッダファイルのバージョンやプリプロセスで使用したい場合は 
				LUMINO_CORE_VERSION_MAJOR や LUMINO_CORE_VERSION_MINOR を使用してください。
*/
class Engine
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
		@brief	リビジョンバージョンを取得します。
	*/
	static int GetRevision();

	/**
		@brief	ビルドバージョンを取得します。
	*/
	static int GetBuild();

	/**
		@brief	バージョン文字列の取得を取得します。
	*/
	static const TCHAR* GetString();

	/**
		@brief	指定したバージョン番号と、ライブラリファイルのコンパイルバージョン番号を比較します。
		@return	指定バージョン >= コンパイルバージョン である場合、true を返します。
	*/
	static bool IsAtLeast(int major = LUMINO_ENGINE_VERSION_MAJOR, int minor = LUMINO_ENGINE_VERSION_MINOR, int revision = LUMINO_ENGINE_VERSION_REVISION);

};

} // namespace Version
LN_NAMESPACE_END

#endif // LUMINO_ENGINE_VERSION_H
