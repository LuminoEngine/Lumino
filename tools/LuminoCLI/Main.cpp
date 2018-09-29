
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

		if (argc == 3 && strcmp(argv[1], "--local-initial-setup"))
		{
			ln::Path packageDir = argv[2];
			ln::Path toolsDir = ln::Path(packageDir, u"tools");
			ln::List<ln::String> lines;

			if (ln::FileSystem::existsFile(u"~/.profile")) {
				// TODO: readAllLines
				{
					int beginLine = -1;
					int endLine = -1;
					ln::StreamReader r(u"~/.profile");
					ln::String line;
					while (r.readLine(&line)) {
						if (line.indexOf(u"# [Begin Lumino]") == 0)
							beginLine = lines.size();
						if (line.indexOf(u"# [End Lumino]") == 0)
							endLine = lines.size();
						lines.add(line);
					}

					if (beginLine >= 0 && endLine >= 0 && beginLine < endLine) {
						// already exists.
					}
					else {
						lines.add(u"# [Begin Lumino]");
						lines.add(ln::String::format(u"export LUMINO_ROOT={0}", packageDir));
						lines.add(ln::String::format(u"export PATH=$PATH:{0}", toolsDir));
						lines.add(u"# [End Lumino]");
					}
				}
			}
			else {
				lines.add(u"# [Begin Lumino]");
				lines.add(ln::String::format(u"export LUMINO_ROOT={0}", packageDir));
				lines.add(ln::String::format(u"export PATH=$PATH:{0}", toolsDir));
				lines.add(u"# [End Lumino]");
			}

			ln::StreamWriter w(u"~/.profile");
			for (auto& line : lines) {
				w.writeLine(line);
			}
			return 0;
		}

	

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

