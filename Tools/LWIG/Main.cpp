
#include "Global.h"
#include "Parser.h"
#include "DotNet/DotNetPInvokeLibGenerator.h"
#include "DotNet/DotNetClassLibGenerator.h"
#include "DotNet/CSClassLibGenerator.h"
#include "DotNet/DotNetCommon.h"
#include "WrapperIF/WrapperIFGenerator.h"
#include "WrapperIF/WrapperIFClassesGenerator.h"

PathName		g_templateDir;
SymbolDatabase	g_database;
PathName		g_csOutputDir;

int main()
{
	List<PathName> files =
	{
		LUMINO_ROOT_DIR"/Bindings/Runtime/include/Common.h",
		LUMINO_ROOT_DIR"/Source/LuminoCore/Include/Lumino/Math/Vector3.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Engine.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Audio/Sound.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Base/GeometryStructs.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/Common.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/Texture.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Graphics/GraphicsResourceObject.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/SceneNode.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/VisualNode.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/Sprite.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Scene/StaticMesh.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Game/Component.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Foundation/Application.h",
		LUMINO_ROOT_DIR"/Source/LuminoEngine/Include/Lumino/Foundation/GameScene.h",
	};

	g_templateDir = LUMINO_ROOT_DIR"/Tools/LWIG/";
	g_csOutputDir = LUMINO_ROOT_DIR"/Bindings/DotNet/LuminoDotNet/";

	try
	{

		HeaderParser parser(&g_database);
		parser.ParseFiles(files/*, &database*/);

		g_database.Link();

		DotNetCommon::Initialize();

		{
			WrapperIFGenerator gen;
			gen.Generate(&g_database);
		}
		{
			WrapperIFClassesGenerator gen;
			gen.Generate();
		}
		{
			DotNetPInvokeLibGenerator g;
			g.Generate();
		}
		{
			CSStructsGenerator g;
			g.Generate();
		}
		{
			CSClassLibGenerator g;
			g.Generate();
		}

		//
		//
		//{
		//	DotNetPInvokeLibGenerator gen;
		//	gen.Generate(&database);
		//}
		//
		//{
		//	DotNetClassLibGenerator gen;
		//	gen.Generate(&database);
		//}
	}
	catch (Exception& e)
	{
		Console::WriteLine(e.GetMessage());
	}
	
	return 0;
}

