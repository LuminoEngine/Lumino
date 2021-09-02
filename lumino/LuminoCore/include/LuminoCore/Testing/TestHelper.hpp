#pragma once
#include "../Base/String.hpp"
#include "../IO/Path.hpp"

namespace ln {

class TestHelper
{
public:
	static void setTempDirPath(const Path& dirPath);
	static void setAssetsDirPath(const Path& dirPath);


	/// baseFilePath のファイルと同じディレクトリの fileName を指すパスを得る
	static String getFilePath(const char* baseFilePath, const char* fileName);
	static String getFilePath(const char* baseFilePath, const Char* fileName) { return String(getFilePathU(baseFilePath, fileName).c_str()); }

	/// baseFilePath のファイルと同じディレクトリの fileName を指すパスを得る
	static Path getDirPath(const char* baseFilePath);

	/// baseFilePath のファイルと同じディレクトリの fileName を指すパスを得る
	static std::string getFilePathA(const char* baseFilePath, const char* fileName);
	static std::wstring getFilePathW(const char* baseFilePath, const wchar_t* fileName);
	static Path getFilePathU(const char* baseFilePath, const Char* fileName);

	static Path getTempPath(const char* fileName);
	static Path getAssetPath(const char* fileName);

	/// 2 つの配列をバイト単位で比較する
	static bool checkArrays(const void* ary1, const void* ary2, size_t count);

	// 2 つのファイル内容が一致するか確認する
	static bool equalFiles(const Path& filePath1, const Path& filePath2);
};

} // namespace ln

// obsolete: Please use LN_ASSETFILE
#define LN_TEST_GET_DIR_PATH() ln::TestHelper::getDirPath(__FILE__)

// obsolete: Please use LN_ASSETFILE
#define LN_LOCALFILE(fileName) ln::TestHelper::getFilePath(__FILE__, fileName).c_str()

// obsolete: Please use LN_ASSETFILE
#define LN_LOCALFILEA(fileName) ln::TestHelper::getFilePathA(__FILE__, fileName).c_str()

// obsolete: Please use LN_ASSETFILE
#define LN_LOCALFILEW(fileName) ln::TestHelper::getFilePathW(__FILE__, fileName).c_str()

#define LN_TEMPFILE(fileName) ln::TestHelper::getTempPath(fileName).c_str()

#define LN_ASSETFILE(fileName) ln::TestHelper::getAssetPath(fileName).c_str()
