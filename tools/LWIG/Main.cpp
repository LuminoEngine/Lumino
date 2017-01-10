
#include "DotNet/DotNetPInvokeLibGenerator.h"
#include "DotNet/DotNetClassLibGenerator.h"

int main()
{
	List<String> files =
	{
		"Lumino/Graphics/Texture.h"
	};
	
	SymbolDatabase database;
	
	Parser::ParseFiles(files, &database);
	
	
	{
		DotNetPInvokeLibGenerator gen;
		gen.Generate(&database);
	}
	
	{
		DotNetClassLibGenerator gen;
		gen.Generate(&database);
	}
	
	return 0;
}

