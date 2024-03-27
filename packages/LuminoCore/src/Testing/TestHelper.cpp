
#include <LuminoCore/Base/StlHelper.hpp>
#include <LuminoCore/IO/FileSystem.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>

namespace ln {

static Path g_tempDirPath;
static Path g_assetsDirPath;

void TestHelper::setTempDirPath(const Path& dirPath)
{
	g_tempDirPath = dirPath.canonicalize();
	FileSystem::createDirectory(g_tempDirPath);

	std::cout << "TempDirPath : " << g_tempDirPath.str() << std::endl;
}

void TestHelper::setAssetsDirPath(const Path& dirPath)
{
	g_assetsDirPath = dirPath.canonicalize();
	FileSystem::createDirectory(g_assetsDirPath);

	std::cout << "AssetsDirPath : " << g_assetsDirPath.str() << std::endl;
}

//------------------------------------------------------------------------------
String TestHelper::getFilePath(const char* baseSourceFilePath, const char* fileName)
{
	Path base(String::fromCString(baseSourceFilePath));
	Path path(base.parent(), String::fromCString(fileName));
	return String(path.c_str());
}

//------------------------------------------------------------------------------
Path TestHelper::getDirPath(const char* baseFilePath)
{
	Path base(String::fromCString(baseFilePath));
	return base.parent();
}

//------------------------------------------------------------------------------
std::string TestHelper::getFilePathA(const char* baseFilePath, const char* fileName)
{
	return detail::StdStringHelper::cat<std::string>(baseFilePath, "/", fileName);
}

//------------------------------------------------------------------------------
std::wstring TestHelper::getFilePathW(const char* baseFilePath, const wchar_t* fileName)
{
	return detail::StdStringHelper::cat<std::wstring>(String::fromCString(baseFilePath).toStdWString().c_str(), L"/", fileName);
}

Path TestHelper::getFilePathU(const char* baseFilePath, const Char* fileName)
{
	Path base(String::fromCString(baseFilePath));
	Path path(base.parent(), fileName);
	return path;
}

Path TestHelper::getTempPath(const char* fileName)
{
	return Path(g_tempDirPath, String::fromCString(fileName));
}

Path TestHelper::getAssetPath(const char* fileName)
{
	return Path(g_assetsDirPath, String::fromCString(fileName));
}

//------------------------------------------------------------------------------
bool TestHelper::checkArrays(const void* ary1, const void* ary2, size_t count)
{
	byte_t* b1 = (byte_t*)ary1;
	byte_t* b2 = (byte_t*)ary2;
	for (size_t i = 0; i < count; ++i)
	{
		if (b1[i] != b2[i]) {
			printf("not match arrays [%zu]\n", i);
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
bool TestHelper::equalFiles(const Path& filePath1, const Path& filePath2)
{
	if (FileSystem::getFileSize(filePath1) != FileSystem::getFileSize(filePath2)) return false;
	return FileSystem::readAllBytes(filePath1).unwrap() == FileSystem::readAllBytes(filePath2).unwrap();
}

} // namespace ln
