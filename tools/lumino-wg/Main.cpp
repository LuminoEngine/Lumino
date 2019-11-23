
#include <LuminoCore/Testing/TestHelper.hpp>
#include "SymbolDatabase.hpp"
#include "Parser.hpp"
#include "WrapperIF/WrapperIFClassesGenerator.hpp"
#include "WrapperIF/WrapperIFGenerator.hpp"
#include "DotNet/CSStructsGenerator.hpp"

#define TEST_ROOT u"C:/Proj/LN/Lumino/"

int main(int argc, char** argv)
{
	ln::List<ln::Path> files =
	{
		TEST_ROOT"include/LuminoCore/Math/Vector2.hpp",
		TEST_ROOT"include/LuminoCore/Math/Vector3.hpp",
		TEST_ROOT"include/LuminoCore/Math/Vector4.hpp",
		TEST_ROOT"include/LuminoCore/Math/Quaternion.hpp",
		TEST_ROOT"include/LuminoCore/Math/Matrix.hpp",
		TEST_ROOT"include/LuminoEngine/Engine/Application.hpp",
		//u"C:/Proj/GitHub/Lumino/src/LuminoEngine/include/Lumino/Graphics/Texture.hpp",
	};


	auto diag = ln::makeObject<ln::DiagnosticsManager>();
	SymbolDatabase db(diag);

	HeaderParser parser;
	//parser.addIncludePath(_T("D:/Proj/Lumino/Source/LuminoCore/Include"));
	//parser.addIncludePath(_T("D:/Proj/Lumino/Source/LuminoEngine/Include"));
	parser.addIncludePath(TEST_ROOT"include");
	parser.addIncludePath(TEST_ROOT"include");
	parser.addForceIncludeFile(TEST_ROOT"include/LuminoCore/Base/Common.hpp");

	for (auto& file : files)
	{
		parser.parse(file, &db, diag);
	}

	db.Link();

	ln::String moduleFullName = "Lumino";
	ln::String moduleShortName = "LN";
	ln::Path outputDir = TEST_ROOT"src/LuminoRuntime/src";

	{
		WrapperIFClassesGenerator gen;
		gen.setup(&db, LN_LOCALFILE("WrapperIF/Templates"), outputDir, moduleFullName, moduleShortName);
		gen.generate();
	}
	{
		WrapperIFGenerator gen;
		gen.setup(&db, LN_LOCALFILE("WrapperIF/Templates"), outputDir, moduleFullName, moduleShortName);
		gen.generate();
	}
	{
		CSStructsGenerator gen;
		gen.setup(&db, LN_LOCALFILE("Templates"), outputDir, moduleFullName, moduleShortName);
		gen.generate();
	}

	return 0;
}

