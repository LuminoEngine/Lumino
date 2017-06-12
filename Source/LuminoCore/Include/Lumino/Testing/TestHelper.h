#pragma once
#include "../Base/String.h"
#include "../IO/PathName.h"

LN_NAMESPACE_BEGIN

class TestHelper
{
public:
	/// baseFilePath のファイルと同じディレクトリの fileName を指すパスを得る
	static String getFilePath(const char* baseFilePath, const char* fileName);
	static String getFilePath(const char* baseFilePath, const wchar_t* fileName) { return String::fromNativeCharString(GetFilePathW(baseFilePath, fileName).c_str()); }

	/// baseFilePath のファイルと同じディレクトリの fileName を指すパスを得る
	static PathName GetDirPath(const char* baseFilePath);

	/// baseFilePath のファイルと同じディレクトリの fileName を指すパスを得る
	static PathNameA GetFilePathA(const char* baseFilePath, const char* fileName);
	static PathNameW GetFilePathW(const char* baseFilePath, const wchar_t* fileName);

	/// 2 つの配列をバイト単位で比較する
	static bool CheckArrays(const void* ary1, const void* ary2, size_t count);

	// 2 つのファイル内容が一致するか確認する
	static bool EqualFiles(const PathName& filePath1, const PathName& filePath2);
};

// __FILE__ のあるフォルダパス (PathName) を取得する
#define LN_TEST_GET_DIR_PATH() TestHelper::GetDirPath(__FILE__)

// __FILE__ と同じフォルダのファイルパス (TCHAR) を取得する
#define LN_LOCALFILE(fileName) TestHelper::getFilePath(__FILE__, fileName).c_str()

// __FILE__ と同じフォルダのファイルパス (char) を取得する
#define LN_LOCALFILEA(fileName) TestHelper::GetFilePathA(__FILE__, fileName).c_str()

// __FILE__ と同じフォルダのファイルパス (wchar_t) を取得する
#define LN_LOCALFILEW(fileName) TestHelper::GetFilePathW(__FILE__, fileName).c_str()

LN_NAMESPACE_END
