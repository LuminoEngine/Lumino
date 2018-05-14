
#include "Workspace.hpp"

int main(int argc, char** argv)
{
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

