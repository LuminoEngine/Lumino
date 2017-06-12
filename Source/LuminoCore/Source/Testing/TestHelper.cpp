﻿
#include <Lumino/IO/FileSystem.h>
#include <Lumino/Testing/TestHelper.h>

LN_NAMESPACE_BEGIN

//------------------------------------------------------------------------------
String TestHelper::getFilePath(const char* baseSourceFilePath, const char* fileName)
{
	PathName base(baseSourceFilePath);
	PathName path(base.getParent(), fileName);
	return String(path.c_str());
}

//------------------------------------------------------------------------------
PathName TestHelper::GetDirPath(const char* baseFilePath)
{
	PathName base(baseFilePath);
	return base.getParent();
}

//------------------------------------------------------------------------------
PathNameA TestHelper::GetFilePathA(const char* baseFilePath, const char* fileName)
{
	PathNameA base(baseFilePath);
	PathNameA path(base.getParent(), fileName);
	return path;
}

//------------------------------------------------------------------------------
PathNameW TestHelper::GetFilePathW(const char* baseFilePath, const wchar_t* fileName)
{
	PathNameW base(baseFilePath);
	PathNameW path(base.getParent(), fileName);
	return path;
}

//------------------------------------------------------------------------------
bool TestHelper::CheckArrays(const void* ary1, const void* ary2, size_t count)
{
	byte_t* b1 = (byte_t*)ary1;
	byte_t* b2 = (byte_t*)ary2;
	for (size_t i = 0; i < count; ++i)
	{
		if (b1[i] != b2[i]) {
			printf("not match arrays [%u]\n", i);
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------------
bool TestHelper::EqualFiles(const PathName& filePath1, const PathName& filePath2)
{
	if (FileSystem::getFileSize(filePath1) != FileSystem::getFileSize(filePath2)) return false;
	return FileSystem::readAllBytes(filePath1).equals(FileSystem::readAllBytes(filePath2));
}

LN_NAMESPACE_END
