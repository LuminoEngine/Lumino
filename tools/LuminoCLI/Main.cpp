
#include "Workspace.hpp"
#include "Project.hpp"

int main(int argc, char** argv)
{
#if defined(LN_DEBUG) && defined(_WIN32)
	if (argc == 1)
	{
		//::SetCurrentDirectoryW(L"C:\\LocalProj\\LuminoProjects\\HelloLumino");
		::SetCurrentDirectoryW(L"C:\\LocalProj\\tmp");
	
		const char* debugArgv[] = {
			"<program>",
			"init", "HelloLumino",

			//"<program>", "dev-install-tools",

			//"<program>", "build", "Emscripten",
			//"<program>", "build", "Android",

			//"dev-openide", "vs",
			
			//"--local-initial-setup", "/Users/lriki/Proj/Lumino/ReleasePackage.macOS"

		};
		argc = sizeof(debugArgv) / sizeof(char*);
		argv = (char**)debugArgv;
	}
#endif
	try
	{
		if (argc == 3 && strcmp(argv[1], "--local-initial-setup") == 0)
		{
			ln::Path packageDir = argv[2];
			ln::Path toolsDir = ln::Path(packageDir, u"tools");
			ln::List<ln::String> lines;
			
			ln::Path profile = ln::Path(ln::Environment::specialFolderPath(ln::SpecialFolder::Home), u".profile");

			bool added = false;
			if (ln::FileSystem::existsFile(profile)) {
				int beginLine = -1;
				int endLine = -1;
				ln::StreamReader r(profile);
				ln::String line;
				while (r.readLine(&line)) {
					if (line.indexOf(u"# [Lumino begin]") == 0)
						beginLine = lines.size();
					if (line.indexOf(u"# [Lumino end]") == 0)
						endLine = lines.size();
					lines.add(line);
				}

				if (beginLine >= 0 && endLine >= 0 && beginLine < endLine) {
					// already exists.
					
					for (int i = beginLine; i < endLine; i++) {
						if (lines[i].indexOf("export LUMINO_ROOT") == 0) {
							lines[i] = ln::String::format(u"export LUMINO_ROOT={0}", packageDir);
							CLI::info(u"LUMINO_ROOT updating.");
						}
						if (lines[i].indexOf("export PATH") == 0) {
							lines[i] = ln::String::format(u"export PATH=$PATH:{0}", toolsDir);
							CLI::info(u"PATH updating.");
						}
					}
					
					added = true;
				}
			}
			
			if (!added) {
				lines.add(u"");
				lines.add(u"# [Lumino begin]");
				lines.add(ln::String::format(u"export LUMINO_ROOT={0}", packageDir));
				lines.add(ln::String::format(u"export PATH=$PATH:{0}", toolsDir));
				lines.add(u"# [Lumino end]");
			}

			ln::StreamWriter w(profile);
			w.setNewLine(u"\n");	// for macOS (\r is invalid)
			for (auto& line : lines) {
				w.writeLine(line);
			}
				
			CLI::info(ln::String::format(u"Lumino environment variable added to {0}.", profile));
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
		auto runCommand = parser.addCommand(_T("run"), _T("run description."));
		auto runCommand_targetArg = runCommand->addPositionalArgument(u"target", u"-");

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
					workspace->newProject(
						ln::Path(ln::Environment::currentDirectory(), init_projectName->value()),
						init_projectName->value());
				}
			}
			else if (parser.has(buildCommand))
			{
				workspace->openProject(ln::Environment::currentDirectory());
				workspace->buildProject(burildTargetArg->value());
			}
			else if (parser.has(runCommand))
			{
				workspace->openProject(ln::Environment::currentDirectory());
				workspace->runProject(runCommand_targetArg->value());
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

