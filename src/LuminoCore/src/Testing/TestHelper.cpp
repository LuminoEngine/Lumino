
#include <Lumino/Base/StlHelper.hpp>
#include <Lumino/IO/FileSystem.hpp>
#include <Lumino/Testing/TestHelper.hpp>

namespace ln {

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
	Path base(baseFilePath);
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
	Path base(baseFilePath);
	Path path(base.parent(), fileName);
	return path;
}

Path TestHelper::getTempPath(const char* fileName)
{
	// TODO:
	return Path(String::fromCString(fileName));
}

Path TestHelper::getAssetPath(const char* fileName)
{
	// TODO:
	return Path(String::fromCString(fileName));
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
	return FileSystem::readAllBytes(filePath1).equals(FileSystem::readAllBytes(filePath2));
}

} // namespace ln
