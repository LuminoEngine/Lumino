
#pragma once
#include "String.hpp"

namespace ln {

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
	ApplicationData,
	Temporary,
};

/** システムの特別なフォルダパス を取得する際の動作 */
enum class SpecialFolderOption
{
	/** フォルダパスが存在しなければ、空の文字列を返します。 */
	None,

	/** フォルダパスが存在しなければ、フォルダを作成します。 */
	Create,

	/** フォルダパスの存在を確認せずにパスを返します。 */
	DoNotVerify,
};

class Environment
{
public:

	/** カレントディレクトリのパスを取得します。 */
	static String currentDirectory();

	/** アプリケーションを開始した実行ファイルのパスを取得します。*/
	static String executablePath();

	/** パス文字列の大文字と小文字を区別するかを確認します。 */
	static CaseSensitivity pathCaseSensitivity();

	/** システムの特別なフォルダのパスを取得します。 */
	static String specialFolderPath(SpecialFolder specialFolder);

	/** 環境変数の値を取得します。 */
	static Optional<String> getEnvironmentVariable(const StringRef& variableName);
};

} // namespace ln
