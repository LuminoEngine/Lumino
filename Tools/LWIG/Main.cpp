
//#include "Global.h"
#include "Parser.h"
#include "SymbolDatabase.h"
//#include "DotNet/DotNetPInvokeLibGenerator.h"
//#include "DotNet/DotNetClassLibGenerator.h"
//#include "DotNet/CSClassLibGenerator.h"
//#include "DotNet/DotNetCommon.h"
//#include "WrapperIF/WrapperIFGenerator.h"
//#include "WrapperIF/WrapperIFClassesGenerator.h"
//
//PathName		g_templateDir;
//SymbolDatabase	g_database;
//PathName		g_csOutputDir;

int main()
{
	List<Path> files =
	{
		L"C:/Proj/LN/Lumino/Tools/LWIG/Test.cpp",
		//_T(LUMINO_ROOT_DIR"/Bindings/Runtime/include/Common.h"),
		//LUMINO_ROOT_DIR"/Source/LuminoCore/Include/Lumino/Math/Vector3.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Engine.h",
		//_T(LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Audio/Sound.h"),
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Base/GeometryStructs.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/Common.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/Texture.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/GraphicsResourceObject.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/SceneNode.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/VisualNode.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/Sprite.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/StaticMesh.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Game/Component.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Framework/Application.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Framework/GameScene.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Physics/CollisionShape.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Physics/PhysicsObject.h",
		//LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Physics/Collider.h",
	};

	//g_templateDir = LUMINO_ROOT_DIR"/Tools/LWIG/";
	//g_csOutputDir = LUMINO_ROOT_DIR"/Bindings/DotNet/LuminoDotNet/";

	try
	{
		SymbolDatabase db;

		HeaderParser::parse(files[0], &db);

		//HeaderParser parser(&g_database);
		//parser.ParseFiles(files/*, &database*/);

		//g_database.Link();

		//DotNetCommon::initialize();

		//{
		//	WrapperIFGenerator gen;
		//	gen.generate(&g_database);
		//}
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

