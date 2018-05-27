
#include "Workspace.hpp"

int main(int argc, char** argv)
{
	::SetCurrentDirectoryW(L"D:\\Documents\\LuminoProjects\\Hello\\Build");
	
	ln::String s1, s2;
	ln::Process::execute(
		_T("D:\\ProgramFiles\\Emscripten\\python\\2.7.5.3_64bit\\python.exe"),
		
		{ _T("D:\\ProgramFiles\\Emscripten\\emscripten\\1.35.0\\emcmake"), _T("cmake"), _T(".."), _T("-G"), _T("\"MinGW Makefiles\""),
		
		//_T("-DCMAKE_LIBRARY_PATH=\"D:\\Proj\\LN\\Lumino\\build\\Package\\lib\\Emscripten\\lib\"")
		//_T("-DCMAKE_LIBRARY_PATH=\"D:\\Proj\\LN\\Lumino\\build\\Package\\lib\\Emscripten\"")

		//_T("-DLUMINO_ROOT=\"D:\\Proj\\LN\\Lumino\\build\\Package\"")
		_T("-DCMAKE_PREFIX_PATH=\"D:\\Proj\\LN\\Lumino\\build\\EnginePackage\"")
		
		}/*, &s1, &s2*/);

	return 0;

#if 1
	char* debugArgv[] = {
		"<program>", "init", "HelloLumino"
	};
	argc = 3;
	argv = debugArgv;
	::SetCurrentDirectoryW(L"D:\\Documents\\LuminoProjects\\Hello");
#endif

	ln::CommandLineParser parser;
	auto initCommand = parser.addCommand(_T("init"), _T("init description."));
	auto projectnameArg = initCommand->addPositionalArgument(_T("project-name"), _T("prooject name."));

	auto buildCommand = parser.addCommand(_T("build"), _T("init description."));

	//auto forceOption1 = initCommand->addFlagOption(_T("f"), _T("force"), _T("force description."));

	//auto addCommand = parser.addCommand(_T("add"), _T("add description."));
	//auto forceOption2 = addCommand->addFlagOption(_T("f"), _T("force"), _T("force description."));

	if (parser.process(argc, argv))
	{
		auto workspace = ln::makeRef<Workspace>();

		if (parser.has(initCommand))
		{
			workspace->newProject(ln::Environment::currentDirectory(), projectnameArg->value());
		}
	}

	return 0;
}

