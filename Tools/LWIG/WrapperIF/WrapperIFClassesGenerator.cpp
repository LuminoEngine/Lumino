
#include "../SymbolDatabase.h"
#include "WrapperIFClassesGenerator.h"

const String wrapperIFClassTemplate =
	"class LN%ClassName% : public %ClassName%\n"
	"{\n"
	"public:\n"
	"};\n";

void WrapperIFClassesGenerator::generate()
{
	//// classes
	//OutputBuffer wrapper;
	//for (auto& classInfo : g_database.classes)
	//{
	//	wrapper.AppendLines(wrapperIFClassTemplate.Replace("%ClassName%", classInfo->name));
	//}

	//{
	//	String src = FileSystem::ReadAllText(PathName(g_templateDir, "WrapperIF/Templates/WrapperIF.h"));
	//	src = src.Replace("%WrapperClasses%", wrapper.ToString());
	//	FileSystem::WriteAllText(LUMINO_ROOT_DIR"/bindings/Runtime/src/WrapperIF.generated.h", src);
	//}
}

