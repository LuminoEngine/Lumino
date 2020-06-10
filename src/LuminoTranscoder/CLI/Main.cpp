
#include "../Core/CodeAnalyzer.hpp"
#include "../Core/SymbolDatabase.hpp"
#include "../Core/Generators/FlatCGenerator.hpp"
#include "../Core/Generators/RubyExtGenerator.hpp"
#include "../Core/Generators/HSPGenerator.hpp"
#include "../Core/Generators/DotNetPInvokeGenerator.hpp"
#include "../Core/Generators/DotnetClassGenerator.hpp"

#define TEST_ROOT u"C:/Proj/LN/Lumino/"

int main(int argc, char** argv)
{
	ln::Logger::addStdErrAdapter();
	auto diag = ln::makeObject<ln::DiagnosticsManager>();
	auto pidb = ln::makeRef<PIDatabase>();

	if (!ln::FileSystem::existsFile(u"pidb.json"))
	{


		ln::List<ln::Path> files_LuminoCore =
		{
			//TEST_ROOT "include/LuminoCore/Math/Vector2.hpp",
			TEST_ROOT "include/LuminoCore/Math/Vector3.hpp",
			TEST_ROOT "include/LuminoCore/Math/Vector4.hpp",
			TEST_ROOT "include/LuminoCore/Math/Quaternion.hpp",
			TEST_ROOT "include/LuminoCore/Math/Matrix.hpp",
		};


		ln::List<ln::Path> files_LuminoEngine =
		{
            TEST_ROOT "include/LuminoEngine/Engine/Object.hpp",
			TEST_ROOT "include/LuminoEngine/Base/Event.hpp",
			TEST_ROOT "include/LuminoEngine/Base/Promise.hpp",
			TEST_ROOT "src/LuminoEngine/src/Runtime/BindingValidation.hpp",
			TEST_ROOT "include/LuminoEngine/Base/Collection.hpp",
			TEST_ROOT "include/LuminoEngine/Base/Serializer.hpp",
			TEST_ROOT "include/LuminoEngine/Platform/PlatformEvent.hpp",
            TEST_ROOT "include/LuminoEngine/Asset/AssetModel.hpp",
            TEST_ROOT "include/LuminoEngine/Asset/Assets.hpp",
			TEST_ROOT "include/LuminoEngine/Graphics/Common.hpp",
			TEST_ROOT "include/LuminoEngine/Graphics/ColorStructs.hpp",
			TEST_ROOT "include/LuminoEngine/Graphics/GeometryStructs.hpp",
			TEST_ROOT "include/LuminoEngine/Graphics/GraphicsResource.hpp",
			TEST_ROOT "include/LuminoEngine/Graphics/Texture.hpp",
			TEST_ROOT "include/LuminoEngine/Rendering/RenderView.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/Component.hpp",
			TEST_ROOT "include/LuminoEngine/Visual/VisualComponent.hpp",
			TEST_ROOT "include/LuminoEngine/Visual/SpriteComponent.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/World.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/WorldObject.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/VisualObject.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/Camera.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/Light.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/Sprite.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/CameraOrbitControlComponent.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/Raycaster.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/WorldRenderView.hpp",
			TEST_ROOT "include/LuminoEngine/Scene/Shapes/MeshPrimitives.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UIEvents.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UILayoutElement.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UIElement.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UIControl.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UIButton.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UITextBlock.hpp",
			TEST_ROOT "include/LuminoEngine/UI/UISprite.hpp",
			TEST_ROOT "include/LuminoEngine/Input/Input.hpp",
			TEST_ROOT "include/LuminoEngine/Input/Mouse.hpp",
			TEST_ROOT "include/LuminoEngine/Engine/EngineSettings.hpp",
			TEST_ROOT "include/LuminoEngine/Engine/Engine.hpp",
            TEST_ROOT "include/LuminoEngine/Engine/Application.hpp",
			TEST_ROOT "include/LuminoEngine/Engine/Debug.hpp",
		};

		CodeAnalyzer ca;
		ca.parserExecutable = ln::Path(ln::Path(ln::Environment::executablePath()).parent(), u"../../Parser/Release/LuminoTranscoder-Parser.exe").canonicalize();

		for (auto& file : files_LuminoCore) {
			CompilationDatabase cdb;
			cdb.inputFile = file;
			cdb.includeDirectories.add(TEST_ROOT "include");
			cdb.forceIncludeFiles.add(TEST_ROOT "src/LuminoCore/src/LuminoCore.PCH.h");
			ca.inputs.add(cdb);
		}

		for (auto& file : files_LuminoEngine) {
			CompilationDatabase cdb;
			cdb.inputFile = file;
			cdb.includeDirectories.add(TEST_ROOT "include");
            if (file.fileName().str() == u"Object.hpp") // Object class が force include されるものと合わせて再定義扱いになりクラス名が取れなくなるため、特別扱いする
                cdb.forceIncludeFiles.add(TEST_ROOT "include/LuminoCore.hpp");
            else
                cdb.forceIncludeFiles.add(TEST_ROOT "src/LuminoEngine/src/LuminoEngine.PCH.h");
			ca.inputs.add(cdb);
		}

		ca.analyze(pidb, diag);

		pidb->save(u"pidb.json");
		return 0;
	}
	else {
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
	config->flatCHeaderString = u"#include <LuminoEngine.hpp>\n#include \"BindingValidation.hpp\"";
    config->flatCHeaderOutputDirOverride = LN_LOCALFILE("../../../include/LuminoEngine/Runtime");
    config->flatCSourceOutputDirOverride = LN_LOCALFILE("../../../src/LuminoEngine/src/Runtime");

	//{
	//	FlatCHeaderGenerator g;
	//	g.setup(db, config);
	//	g.generate();
	//}
	//{
	//	FlatCSourceGenerator g;
	//	g.setup(db, config);
	//	g.generate();
	//}
	//{
	//	RubyExtGenerator g;
	//	g.setup(db, config);
	//	g.generate();
	//}
	//{
	//	RubyYARDOCSourceGenerator g;
	//	g.setup(db, config);
	//	g.generate();
	//}
	{
		HSPHeaderGenerator g;
		g.setup(db, config);
		g.generate();
	}
	//{
	//	HSPCommandsGenerator g;
	//	g.setup(db, config);
	//	g.generate();
	//}
    //{
    //    DotNetPInvokeGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}
    //{
    //    DotnetClassGenerator g;
    //    g.setup(db, config);
    //    g.generate();
    //}

	return 0;
}
