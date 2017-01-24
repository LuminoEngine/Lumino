#pragma once

#include <gtest/gtest.h>
#include <memory>
#include <LuminoCore.h>
#include <Lumino/Testing/TestHelper.h>
using namespace ln;

#include <Fluorite/Common.h>
#include <Fluorite/AnalyzerContext.h>
#include <Fluorite/Diagnostics.h>
using namespace fl;

#define DO_LEX(code) \
	AnalyzerContext ctx; \
	auto file = ctx.RegisterInputMemoryCode("test", code); \
	ctx.LexFile(file); \
	auto tokens = file->GetTokenList(); \

