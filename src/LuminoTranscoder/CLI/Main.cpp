
#include "../Core/SymbolDatabase.hpp"
#include "../Core/HeaderParser2.hpp"
#include "../Core/Generators/FlatCGenerator.hpp"
#include "../Core/Generators/RubyExtGenerator.hpp"

#define TEST_ROOT u"D:/Proj/LN/Lumino/"

int main(int argc, char** argv)
{
	ln::GlobalLogger::addStdErrAdapter();
	auto diag = ln::makeObject<ln::DiagnosticsManager>();
	auto pidb = ln::makeRef<PIDatabase>();

	if (!ln::FileSystem::existsFile(u"pidb.json"))
	//if (1)
	{
		ln::List<ln::Path> files_LuminoCore =
		{
			//TEST_ROOT "include/LuminoCore/Math/Vector2.hpp",
			TEST_ROOT "include/LuminoCore/Math/Vector3.hpp",
			//TEST_ROOT "include/LuminoCore/Math/Vector4.hpp",
			TEST_ROOT "include/LuminoCore/Math/Quaternion.hpp",
			//TEST_ROOT "include/LuminoCore/Math/Matrix.hpp",
		};

		for (auto& file : files_LuminoCore) {
			HeaderParser2 parser;
			parser.addIncludePath(TEST_ROOT "include");
			parser.addForceIncludeFile(TEST_ROOT "src/LuminoCore/src/LuminoCore.PCH.h");
			parser.parse(file, pidb, diag);
		}

		ln::List<ln::Path> files_LuminoEngine =
		{
			TEST_ROOT "include/LuminoEngine/Engine/Engine.hpp",
			TEST_ROOT "include/LuminoEngine/Graphics/Common.hpp",
			TEST_ROOT "include/LuminoEngine/Graphics/GraphicsResource.hpp",
			TEST_ROOT "include/LuminoEngine/Graphics/Texture.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/WorldObject.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/VisualObject.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/Sprite.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UIEvents.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UILayoutElement.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UIElement.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UIControl.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UIButton.hpp",
		};

		for (auto& file : files_LuminoEngine) {
			HeaderParser2 parser;
			parser.addIncludePath(TEST_ROOT "include");
			parser.addForceIncludeFile(TEST_ROOT "src/LuminoEngine/src/LuminoEngine.PCH.h");
			parser.parse(file, pidb, diag);
		}

		pidb->save(u"pidb.json");
		return 0;
	}
	else
	{
		pidb->load(u"pidb.json");
	}

	auto db = ln::makeRef<SymbolDatabase>(diag);
	db->initTypes(pidb);
	db->linkTypes();

	diag->dumpToLog();
	if (diag->hasError()) {
		return 1;
	}

	auto config = ln::makeRef<GeneratorConfiguration>();
	config->moduleName = u"Lumino";
	config->outputDir = LN_LOCALFILE("../../../tools/Bindings");
	config->templateDir = LN_LOCALFILE("../Core/Generators/Templates");
	config->targetNamespace = u"ln";
	config->flatCOutputModuleName = u"Ln";
	config->flatCCommonHeader = LN_LOCALFILE("../../../include/LuminoEngine/Runtime/FlatCommon.h");
	config->flatCHeaderString = u"#include <LuminoEngine.hpp>";

	{
		FlatCHeaderGenerator g;
		g.setup(db, config);
		g.generate();
	}
	{
		FlatCSourceGenerator g;
		g.setup(db, config);
		g.generate();
	}
	{
		RubyExtGenerator g;
		g.setup(db, config);
		g.generate();
	}


	return 0;
}
