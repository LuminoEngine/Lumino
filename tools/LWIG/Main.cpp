
#include "SymbolDatabase.h"
#include "Parser.h"
#include "DotNet/DotNetPInvokeLibGenerator.h"
#include "DotNet/DotNetClassLibGenerator.h"

int main()
{
	List<PathName> files =
	{
		"../../../../include/Lumino/Engine.h"
	};
	
	//SymbolDatabase database;
	
	HeaderParser parser;
	parser.ParseFiles(files/*, &database*/);
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

