
#ifndef LUMINO_VERSION_H
#define LUMINO_VERSION_H

#define LUMINO_VERSION_MAJOR		0			/**< メジャーバージョン */
#define LUMINO_VERSION_MINOR		1			/**< マイナーバージョン */
#define LUMINO_MERSION_REVISION		0			/**< パッチ番号 */
#define LUMINO_VERSION_STRING		_T("0.1.0")	/**< バージョン文字列 */

LN_NAMESPACE_BEGIN

/**
	@brief		ライブラリのバージョン情報です。
	@details	このクラスから取得できる値はライブラリのバイナリファイルのバージョン番号です。
				ヘッダファイルのバージョンやプリプロセスで使用したい場合は 
				LUMINO_VERSION_MAJOR や LUMINO_VERSION_MINOR を使用してください。
*/
namespace Version
{

/**
	@brief	メジャーバージョンを取得します。
*/
int GetMajor();

/**
	@brief	マイナーバージョンを取得します。
*/
int GetMinor();

/**
	@brief	リビジョンバージョンを取得します。
*/
int GetRevision();

/**
	@brief	バージョン文字列の取得を取得します。
*/
const TCHAR* GetString();

/**
	@brief	指定したバージョン番号と、ライブラリファイルのコンパイルバージョン番号を比較します。
	@return	指定バージョン >= コンパイルバージョン である場合、true を返します。
*/
bool IsAtLeast(int major = LUMINO_VERSION_MAJOR, int minor = LUMINO_VERSION_MINOR, int revision = LUMINO_MERSION_REVISION);


} // namespace Version
LN_NAMESPACE_END

#endif // LUMINO_VERSION_H
