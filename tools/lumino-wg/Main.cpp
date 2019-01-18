
#include <LuminoCore/Testing/TestHelper.hpp>
#include "SymbolDatabase.hpp"
#include "Parser.hpp"
#include "WrapperIF/WrapperIFClassesGenerator.hpp"
#include "WrapperIF/WrapperIFGenerator.hpp"
#include "DotNet/CSStructsGenerator.hpp"

#define TEST_ROOT u"D:/Proj/GitHub/Lumino/"

int main(int argc, char** argv)
{
	ln::List<ln::Path> files =
	{
		TEST_ROOT"src/LuminoCore/include/Lumino/Math/Vector2.hpp",
		TEST_ROOT"src/LuminoCore/include/Lumino/Math/Vector3.hpp",
		TEST_ROOT"src/LuminoCore/include/Lumino/Math/Vector4.hpp",
		TEST_ROOT"src/LuminoCore/include/Lumino/Math/Quaternion.hpp",
		TEST_ROOT"src/LuminoCore/include/Lumino/Math/Matrix.hpp",
		TEST_ROOT"src/LuminoEngine/include/Lumino/Engine/Application.hpp",
		//u"C:/Proj/GitHub/Lumino/src/LuminoEngine/include/Lumino/Graphics/Texture.hpp",
	};


	auto diag = ln::newObject<ln::DiagnosticsManager>();
	SymbolDatabase db(diag);

	HeaderParser parser;
	//parser.addIncludePath(_T("D:/Proj/Lumino/Source/LuminoCore/Include"));
	//parser.addIncludePath(_T("D:/Proj/Lumino/Source/LuminoEngine/Include"));
	parser.addIncludePath(TEST_ROOT"src/LuminoCore/include");
	parser.addIncludePath(TEST_ROOT"src/LuminoEngine/include");
	parser.addForceIncludeFile(TEST_ROOT"src/LuminoCore/include/Lumino/Base/Common.hpp");

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

