// Copyright (c) 2018 lriki. Distributed under the MIT license.
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

class Environment
{
public:

	/** カレントディレクトリのパスを取得します。 */
	static String currentDirectory();

	/** アプリケーションを開始した実行ファイルのパスを取得します。*/
	static String executablePath();

	/** パス文字列の大文字と小文字を区別するかを確認します。 */
	static CaseSensitivity pathCaseSensitivity();
};

} // namespace ln
