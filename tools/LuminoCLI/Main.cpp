
#include "Workspace.hpp"
#include "Project.hpp"

int main(int argc, char** argv)
{
#if defined(_DEBUG) && defined(_WIN32)
	if (argc == 1)
	{
		//::SetCurrentDirectoryW(L"C:\\LocalProj\\LuminoProjects\\HelloLumino");
		::SetCurrentDirectoryW(L"D:\\Documents\\LuminoProjects\\HelloLumino");
	
		const char* debugArgv[] = {
			"<program>",
			//, "init", "HelloLumino",

			//"<program>", "dev-install-tools",

			//"<program>", "build", "Emscripten",
			//"<program>", "build", "Android",

			"dev-openide", "vs",
		};
		argc = sizeof(debugArgv) / sizeof(char*);
		argv = (char**)debugArgv;
	}
#endif
	try
	{

	

		ln::CommandLineParser parser;
		//auto installCommand = parser.addCommand(_T("install"), _T("install."));

		//--------------------------------------------------------------------------------
		auto init = parser.addCommand(u"init", u"Create a Lumino project in the current directory.");
		auto init_projectName = init->addPositionalArgument(u"project-name", u"prooject name.");

		//--------------------------------------------------------------------------------
		auto buildCommand = parser.addCommand(_T("build"), _T("init description."));
		auto burildTargetArg = buildCommand->addPositionalArgument(u"target", u"-");

		//--------------------------------------------------------------------------------
		auto dev_installTools = parser.addCommand(_T("dev-install-tools"), _T("description."));


		auto dev_openide = parser.addCommand(u"dev-openide", u"description.");
		auto dev_openide_targetArg = dev_openide->addPositionalArgument(u"target", u"target.");


		if (parser.process(argc, argv))
		{
			auto workspace = ln::makeRef<Workspace>();

			if (parser.has(init))
			{
				if (Project::existsProjectFile(ln::Environment::currentDirectory())) {
					CLI::error("Project file already exists.");
					return 1;
				}
				else {
					workspace->newProject(ln::Environment::currentDirectory(), init_projectName->value());
				}
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

