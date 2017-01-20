
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
		"../../../../bindings/Runtime/include/Common.h",
		"../../../../include/Lumino/Engine.h",
		"../../../../include/Lumino/Audio/Sound.h",
		"../../../../include/Lumino/Base/GeometryStructs.h",
		"../../../../external/Lumino.Core/include/Lumino/Math/Vector3.h",
		"../../../../include/Lumino/Foundation/Application.h",
		"../../../../include/Lumino/Foundation/GameScene.h",
	};

	g_templateDir = LUMINO_ROOT_DIR"/tools/LWIG/";
	g_csOutputDir = LUMINO_ROOT_DIR"/bindings/DotNet/LuminoDotNet/";

	
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
	
	return 0;
}

