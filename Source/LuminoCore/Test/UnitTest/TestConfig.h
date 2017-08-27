#pragma once
#include <gtest/gtest.h>
#include <memory>
#define LN_INTERNAL_ACCESS	public
#include <Lumino/Testing/TestHelper.h>
using namespace ln;

PathName Test_GetTempFilePath(const Char* fileName);
#define TEMPFILE(fileName)	Test_GetTempFilePath(_TT(fileName)).c_str()

