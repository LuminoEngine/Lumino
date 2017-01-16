
#include "SymbolDatabase.h"
#include "Parser.h"
#include "DotNet/DotNetPInvokeLibGenerator.h"
#include "DotNet/DotNetClassLibGenerator.h"
#include "WrapperIF/WrapperIFGenerator.h"

int main()
{
	List<PathName> files =
	{
		"../../../../include/Lumino/Engine.h",
		"../../../../include/Lumino/Base/GeometryStructs.h",
		"../../../../external/Lumino.Core/include/Lumino/Math/Vector3.h"
	
	};

	SymbolDatabase database;
	
	HeaderParser parser(&database);
	parser.ParseFiles(files/*, &database*/);

	database.Link();

	{
		WrapperIFGenerator gen;
		gen.Generate(&database);
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

