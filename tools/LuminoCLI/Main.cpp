
#include "Workspace.hpp"

int main(int argc, char** argv)
{
#if 0
	::SetCurrentDirectoryW(L"D:\\Documents\\LuminoProjects\\Hello\\Build");
	
	ln::String s1, s2;
	ln::Process::execute(
		_T("D:\\ProgramFiles\\Emscripten\\python\\2.7.5.3_64bit\\python.exe"),
		
		{ _T("D:\\ProgramFiles\\Emscripten\\emscripten\\1.35.0\\emcmake"), _T("cmake"), _T(".."), _T("-G"), _T("\"MinGW Makefiles\""),
		
		//_T("-DCMAKE_LIBRARY_PATH=\"D:\\Proj\\LN\\Lumino\\build\\Package\\lib\\Emscripten\\lib\"")
		//_T("-DCMAKE_LIBRARY_PATH=\"D:\\Proj\\LN\\Lumino\\build\\Package\\lib\\Emscripten\"")

		//_T("-DLUMINO_ROOT=\"D:\\Proj\\LN\\Lumino\\build\\Package\"")
		//_T("-DCMAKE_PREFIX_PATH=\"D:\\Proj\\LN\\Lumino\\build\\EnginePackage\"")
		//_T("-DCMAKE_PREFIX_PATH=\"D:\\Proj\\LN\\Lumino\\build\\CMakeInstallTemp\\MSVC2017-x86-MT\\lib\\cmake\\Lumino\"")
		//_T("-DCMAKE_PREFIX_PATH=\"D:\\Proj\\LN\\Lumino\\build\\CMakeInstallTemp\\MSVC2017-x86-MT\\lib\"")
		//_T("-DCMAKE_PREFIX_PATH=\"D:/Proj/LN/Lumino/build/CMakeInstallTemp/MSVC2017-x86-MT/lib/cmake/Lumino\"")
		_T("-DCMAKE_PREFIX_PATH=\"D:/Proj/LN/Lumino/build/CMakeInstallTemp/Emscripten/lib/cmake/Lumino\"")
		}/*, &s1, &s2*/);

	return 0;
#endif

#if defined(_DEBUG) && defined(_WIN32)
	if (argc == 1)
	{
		//::SetCurrentDirectoryW(L"C:\\LocalProj\\LuminoProjects\\HelloLumino");
		::SetCurrentDirectoryW(L"D:\\Documents\\LuminoProjects\\HelloLumino");
	
		const char* debugArgv[] = {
			//"<program>", "init", "HelloLumino",

			//"<program>", "dev-install-tools",

			//"<program>", "build", "Emscripten",
			//"<program>", "build", "Android",

			"/Users/lriki/Proj/Lumino/build/macOS/tools/LuminoCLI/Debug/lumino", "dev-openide",
		};
		argc = sizeof(debugArgv) / sizeof(char*);
		argv = (char**)debugArgv;
	}
#endif
	try
	{

	

		ln::CommandLineParser parser;
		//auto installCommand = parser.addCommand(_T("install"), _T("install."));

		auto initCommand = parser.addCommand(_T("init"), _T("init description."));
		auto projectnameArg = initCommand->addPositionalArgument(_T("project-name"), _T("prooject name."));

		auto buildCommand = parser.addCommand(_T("build"), _T("init description."));
		auto burildTargetArg = buildCommand->addPositionalArgument(u"target", u"-");

		auto dev_installTools = parser.addCommand(_T("dev-install-tools"), _T("description."));


		auto dev_openide = parser.addCommand(u"dev-openide", u"description.");
		auto dev_openide_targetArg = dev_openide->addPositionalArgument(u"target", u"target.");

		//auto forceOption1 = initCommand->addFlagOption(_T("f"), _T("force"), _T("force description."));

		//auto addCommand = parser.addCommand(_T("add"), _T("add description."));
		//auto forceOption2 = addCommand->addFlagOption(_T("f"), _T("force"), _T("force description."));

		if (parser.process(argc, argv))
		{
			auto workspace = ln::makeRef<Workspace>();

			//if (parser.has(installCommand))
			//{

			//}
			if (parser.has(initCommand))
			{
				workspace->newProject(ln::Environment::currentDirectory(), projectnameArg->value());
			}
			else if (parser.has(buildCommand))
			{
				workspace->openProject(ln::Environment::currentDirectory());
				workspace->buildProject(burildTargetArg->value());
			}
			else if (parser.has(dev_installTools))
			{
				workspace->dev_installTools();
			}
			else if (parser.has(dev_openide))
			{
				workspace->openProject(ln::Environment::currentDirectory());
				workspace->dev_openIde(dev_openide_targetArg->value());
			}
		}
	}
	catch (ln::Exception& e)
	{
		ln::String m = e.getMessage();
		std::cout << m << std::endl;
	}

	return 0;
}

