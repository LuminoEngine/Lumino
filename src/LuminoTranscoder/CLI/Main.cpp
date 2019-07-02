
#include "../Core/SymbolDatabase.hpp"
#include "../Core/HeaderParser2.hpp"
#include "../Core/Generators/FlatCGenerator.hpp"
#include "../Core/Generators/RubyExtGenerator.hpp"

#define TEST_ROOT u"C:/Proj/LN/Lumino/"

int main(int argc, char** argv)
{
    ln::List<ln::Path> files =
    {
        //TEST_ROOT "include/LuminoCore/Math/Vector2.hpp",
        //TEST_ROOT "include/LuminoCore/Math/Vector3.hpp",
        //TEST_ROOT "include/LuminoCore/Math/Vector4.hpp",
        //TEST_ROOT "include/LuminoCore/Math/Quaternion.hpp",
        //TEST_ROOT "include/LuminoCore/Math/Matrix.hpp",
        TEST_ROOT "include/LuminoEngine/Engine/Engine.hpp",
        //u"C:/Proj/GitHub/Lumino/src/LuminoEngine/include/Lumino/Graphics/Texture.hpp",
    };

	
	auto diag = ln::makeObject<ln::DiagnosticsManager>();

	auto pidb = ln::makeRef<PIDatabase>();
	for (auto& file : files) {
		HeaderParser2 parser;
		parser.addIncludePath(TEST_ROOT "include");
		parser.addIncludePath(TEST_ROOT "include");
		parser.addForceIncludeFile(TEST_ROOT "include/LuminoCore/Base/Common.hpp");
		parser.parse(file, pidb, diag);
	}

	auto db = ln::makeRef<SymbolDatabase>(diag);
	db->initTypes(pidb);
	db->linkTypes();

	auto config = ln::makeRef<GeneratorConfiguration>();
	config->moduleName = u"Lumino";
	config->outputDir = LN_LOCALFILE("../../Bindings");
	config->templateDir = LN_LOCALFILE("../Core/Generators/Templates");
	config->targetNamespace = u"ln";
	config->flatCOutputModuleName = u"Ln";
	config->flatCCommonHeader = LN_LOCALFILE("../../../include/LuminoRuntime/Common.hpp");

	//FlatCHeaderGenerator g1;
	//g1.setup(db, config);
	//g1.generate();

	//FlatCSourceGenerator g2;
	//g2.setup(db, config);
	//g2.generate();

	{
		RubyExtGenerator g;
		g.setup(db, config);
		g.generate();
	}


	return 0;
}
