
//#include "Global.h"
#include <Lumino/Testing/TestHelper.h>
#include "Parser.h"
#include "SymbolDatabase.h"
#include "Diagnostics.h"
//#include "DotNet/DotNetPInvokeLibGenerator.h"
#include "DotNet/CSStructsGenerator.h"
//#include "DotNet/CSClassLibGenerator.h"
//#include "DotNet/DotNetCommon.h"
#include "WrapperIF/WrapperIFClassesGenerator.h"
#include "WrapperIF/WrapperIFGenerator.h"
//
//PathName		g_templateDir;
//SymbolDatabase	g_database;
//PathName		g_csOutputDir;

int main()
{
	List<Path> files =
	{
		//L"D:/Proj/Lumino/Tools/LWIG/Test.cpp",
		_T(LUMINO_ROOT_DIR"/Source/LuminoCore/Include/Lumino/Math/Vector2.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoCore/Include/Lumino/Math/Vector3.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoCore/Include/Lumino/Math/Quaternion.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Engine.h"),
		//_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Audio/Sound.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/Common.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/GraphicsResourceObject.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/Texture.h"),

		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/WorldObject.h"),
		//_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Audio/Sound.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Base/GeometryStructs.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Game/Component.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/SceneNode.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/VisualNode.h"),
		_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/Sprite.h"),
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/StaticMesh.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Framework/Application.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Framework/GameScene.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Physics/CollisionShape.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Physics/PhysicsObject.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Physics/Collider.h",
	};

	String moduleFullName = "Lumino";
	String moduleShortName = "LN";
	Path outputDir = _T(LUMINO_ROOT_DIR"/Bindings/Runtime/src");//_T(".");//

	//g_templateDir = LUMINO_ROOT_DIR"/Tools/LWIG/";
	//g_csOutputDir = LUMINO_ROOT_DIR"/Bindings/DotNet/LuminoDotNet/";

	try
	{
		SymbolDatabase db;
		DiagManager diag;

		HeaderParser parser;
		//parser.addIncludePath(_T("D:/Proj/Lumino/Source/LuminoCore/Include"));
		//parser.addIncludePath(_T("D:/Proj/Lumino/Source/LuminoEngine/Include"));
		parser.addIncludePath(_T(LUMINO_ROOT_DIR"/Source/LuminoCore/Include"));
		parser.addIncludePath(_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include"));

		for (auto& file : files)
		{
			parser.parse(file, &db, &diag);
		}

		db.Link();

		diag.dump();

		//HeaderParser parser(&g_database);
		//parser.ParseFiles(files/*, &database*/);

		//g_database.Link();

		//DotNetCommon::initialize();

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

		//{
		//	WrapperIFClassesGenerator gen;
		//	gen.generate();
		//}
		//{
		//	DotNetPInvokeLibGenerator g;
		//	g.generate();
		//}
		//{
		//	CSStructsGenerator g;
		//	g.generate();
		//}
		//{
		//	CSClassLibGenerator g;
		//	g.generate();
		//}

		//
		//
		//{
		//	DotNetPInvokeLibGenerator gen;
		//	gen.generate(&database);
		//}
		//
		//{
		//	DotNetClassLibGenerator gen;
		//	gen.generate(&database);
		//}
	}
	catch (Exception& e)
	{
		Console::writeLine(e.getMessage());
	}
	
	return 0;
}

