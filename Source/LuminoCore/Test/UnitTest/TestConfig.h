#pragma once
#include <gtest/gtest.h>
#include <memory>
#define LN_INTERNAL_ACCESS	public
#include <Lumino/Testing/TestHelper.h>
using namespace ln;

PathName Test_GetTempFilePath(const Char* fileName);
#define TEMPFILE(fileName)	Test_GetTempFilePath(_TT(fileName)).c_str()

class ScopedMemoryHook
{
public:
	static int newCount;
	static int deleteCount;

	ScopedMemoryHook()
	{
		newCount = 0;
		deleteCount = 0;
		ln::detail::setNewCallback(onNew);
		ln::detail::setDeleteCallback(onDelete);
	}

	~ScopedMemoryHook()
	{
		ln::detail::setNewCallback(nullptr);
		ln::detail::setDeleteCallback(nullptr);
	}

private:
	static void onNew(void* ptr, size_t size) { newCount++; }
	static void onDelete(void* ptr) { deleteCount--; }

};

