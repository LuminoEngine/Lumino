
#include <EnvironmentSettings.hpp>
#include <Workspace.hpp>
#include <Project/Project.hpp>
#include "../src/Engine/EngineManager.hpp"
#include "NewCommand.hpp"
#include "FxcCommand.hpp"
#include "BuildCommand.hpp"
#include "NewAssetCommand.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

static int commnad_localInitialSetup(const char* packageDir_);

static int processCommands(int argc, char** argv);

int main(int argc, char** argv)
{
#if defined(LN_DEBUG) && defined(_WIN32)
	if (argc == 1)
	{
        ::SetCurrentDirectoryW(L"C:/Proj/LN/PrivateProjects/HC4");
        ln::Logger::setLevel(ln::LogLevel::Verbose);
    
		const char* debugArgv[] = {
			"<program>", "-d",
			//"new", "test4",
			//"new", "TH-10", "-t", "cmake",
            
			//"build", "Web",
			//"build", "Windows"
            //"build", "assets",

			//"run", "Web", //"Windows",

			//"dev-openide", "vs",
			
			//"--local-initial-setup", "/Users/lriki/Proj/Lumino/ReleasePackage.macOS"

			//"restore",

            //"fxc", "Assets/LineWave.fx",
			"fxc", "C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/SSRRayTracing.fx", "-e", "D:/tmp/shader"
            //"fxc", "C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/ForwardGBufferPrepass.fx",
			//"fxc", "C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/ShadowCaster.fx", "-e", "D:/tmp/shader"
			//"fxc", "C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/FilmicPostEffect.fx",
			//"fxc", "C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/BloomComposite.fx", "C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/BloomComposite.lcfx",
			//"fxc", "C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/CopyScreen.fx",

			//"fxc", "D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/test/Assets/Graphics/SimplePosColor.fx"
            //"fxc", "D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/test/Assets/Graphics/SimpleConstantBuffer.fx"
			//"fxc", "D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Graphics/Resource/VulkanSampleDeviceContext_26_shader_depth.fx",
			//"fxc", "C:/Proj/LN/Lumino/src/LuminoEngine/src/Graphics/Resource/VulkanSampleDeviceContext_26_shader_depth.fx",
		};
		argc = sizeof(debugArgv) / sizeof(char*);
		argv = (char**)debugArgv;

	}
#endif

	int exitCode = 0;
	try
	{
		setlocale(LC_ALL, "");

		if (argc == 3 && strcmp(argv[1], "--local-initial-setup") == 0)
		{
			return commnad_localInitialSetup(argv[2]);
		}
	
        ln::Logger::addStdErrAdapter();

		ln::EngineContext::current()->initializeEngineManager();
		ln::EngineContext::current()->engineManager()->initializeAssetManager();

		exitCode = processCommands(argc, argv);

	}
	catch (ln::Exception& e)
	{
		ln::String m = e.message();
		std::cout << m << std::endl;
		exitCode = 1;
	}

	ln::EngineContext::current()->disposeEngineManager();
	//ln::EngineContext::current()->disposeRuntimeManager();

	return exitCode;
}

// .profile
static int commnad_localInitialSetup(const char* packageDir_)
{
	ln::Path packageDir = ln::String::fromCString(packageDir_);
	ln::Path toolsDir = ln::Path(packageDir, _TT("tools"));
	ln::List<ln::String> lines;

	ln::Path profile = ln::Path(ln::Environment::specialFolderPath(ln::SpecialFolder::Home), _TT(".profile"));

	bool added = false;
	if (ln::FileSystem::existsFile(profile)) {
		int beginLine = -1;
		int endLine = -1;
		ln::StreamReader r(profile);
		ln::String line;
		while (r.readLine(&line)) {
			if (line.indexOf(_TT("# [Lumino begin]")) == 0)
				beginLine = lines.size();
			if (line.indexOf(_TT("# [Lumino end]")) == 0)
				endLine = lines.size();
			lines.add(line);
		}

		if (beginLine >= 0 && endLine >= 0 && beginLine < endLine) {
			// already exists.

			for (int i = beginLine; i < endLine; i++) {
				if (lines[i].indexOf(_TT("export LUMINO_PATH")) == 0) {
					lines[i] = ln::format(_TT("export LUMINO_PATH={0}"), packageDir.str());
					CLI::info(_TT("LUMINO_PATH updating."));
				}
				if (lines[i].indexOf(_TT("export PATH")) == 0) {
					lines[i] = ln::format(_TT("export PATH=$PATH:{0}"), toolsDir.str());
					CLI::info(_TT("PATH updating."));
				}
			}

			added = true;
		}
	}

	if (!added) {
		lines.add(_TT(""));
		lines.add(_TT("# [Lumino begin]"));
		lines.add(ln::format(_TT("export LUMINO_PATH={0}"), packageDir.str()));
		lines.add(ln::format(_TT("export PATH=$PATH:{0}"), toolsDir.str()));
		lines.add(_TT("# [Lumino end]"));
	}

	ln::StreamWriter w(profile);
	w.setNewLine(_TT("\n"));	// for macOS (\r is invalid)
	for (auto& line : lines) {
		w.writeLine(line);
	}

	CLI::info(ln::format(_TT("Lumino environment variable added to {0}."), profile.str()));
	return 0;
}

static int processCommands(int argc, char** argv)
{

	ln::CommandLineParser parser;
	parser.addHelpOption();

	//auto langOption = parser.addNamedValueOption(_TT("l", _TT("lang", _TT("language.", { _TT("cpp", _TT("rb" });
	auto devOption = parser.addFlagOption(_TT("d"), _TT("dev"), _TT("Development mode."));

	//--------------------------------------------------------------------------------
	// new command
	auto newCommand = parser.addCommand(_TT("new"), _TT("Create a new project."));
	auto newCommand_pathArg = newCommand->addPositionalArgument(_TT("path"), _TT("Project directory path."), ln::CommandLinePositionalArgumentFlags::Optional);
	auto newCommand_nameOption = newCommand->addValueOption(_TT("n"), _TT("name"), _TT(" Set the resulting project name, defaults to the directory name."));
	auto newCommand_templateOption = newCommand->addValueOption(_TT("t"), _TT("template"), _TT("Project template."));
	auto newCommand_engineOption = newCommand->addValueOption(_TT("e"), _TT("engine"), _TT("Engine source."));

	//--------------------------------------------------------------------------------
	// build command
	auto buildCommand = parser.addCommand(_TT("build"), _TT("Build the project."));
	auto buildCommand_packageOption = buildCommand->addFlagOption(_TT("p"), _TT("package"), _TT("Build release package."));
	auto burildTargetArg = buildCommand->addPositionalArgument(_TT("target"), _TT("Specify the target to build."), ln::CommandLinePositionalArgumentFlags::Optional);

	//--------------------------------------------------------------------------------
	// run command
	auto runCommand = parser.addCommand(_TT("run"), _TT("Run the project."));
	auto runCommand_targetArg = runCommand->addPositionalArgument(_TT("target"), _TT("Specify the target to run."), ln::CommandLinePositionalArgumentFlags::Optional);

	//--------------------------------------------------------------------------------
	// restore command
	auto restoreCommand = parser.addCommand(_TT("restore"), _TT("Restore engines included in the project."));

	//--------------------------------------------------------------------------------
	// fxc command
	auto fxcCommand = parser.addCommand(_TT("fxc"), _TT("Compile shader."));
	auto fxcCommand_inputArg = fxcCommand->addPositionalArgument(_TT("input"), _TT("Input file."));
	auto fxcCommand_outputArg = fxcCommand->addPositionalArgument(_TT("output"), _TT("Output file."), ln::CommandLinePositionalArgumentFlags::Optional);
	auto fxcCommand_exportArg = fxcCommand->addValueOption(_TT("e"), _TT("export"), _TT("Export folder of output code."));


	//--------------------------------------------------------------------------------
	// new-asset command
	auto newAssetCommand = parser.addCommand(_TT("new-asset"), _TT("Create a new asset file."));
	auto newAssetCommand_nameArg = newAssetCommand->addPositionalArgument(_TT("name"), _TT("Asset type name."));
	auto newAssetCommand_outputArg = newAssetCommand->addValueOption(_TT("o"), _TT("output"), _TT("Output file path."));

	//--------------------------------------------------------------------------------
	// build-assets command
	//auto buildAssetsCommand = parser.addCommand(_TT("build-assets", _TT("Make assets archive.");


	//--------------------------------------------------------------------------------
	auto dev_installTools = parser.addCommand(_TT("dev-install-tools"), _TT("internal."));


	auto dev_openide = parser.addCommand(_TT("dev-openide"), _TT("internal."));
	auto dev_openide_targetArg = dev_openide->addPositionalArgument(_TT("target"), _TT("target."));


	if (parser.process(argc, argv))
	{
		if (devOption->isSet()) {
			lna::Workspace::developMode = true;
			CLI::info(_TT("Running on develop-mode."));
		}

		//--------------------------------------------------------------------------------
		// new command
		if (parser.has(newCommand))
		{
			auto workspace = ln::makeObject<lna::Workspace>();
			auto projectFile = lna::Workspace::findProejctFile(ln::Environment::currentDirectory());

			NewCommand cmd;
			if (newCommand_nameOption->hasValue()) {
				cmd.projectName = newCommand_nameOption->value();
			}
			if (newCommand_templateOption->hasValue()) {
				cmd.templateName = newCommand_templateOption->value();
			}
			if (newCommand_engineOption->hasValue()) {
				cmd.engineSource = newCommand_engineOption->value();
			}
			if (newCommand_pathArg->hasValue()) {
				cmd.projectDirectory = newCommand_pathArg->value();
			}
			return cmd.execute(workspace);
		}
		//--------------------------------------------------------------------------------
		// build command
		else if (parser.has(buildCommand))
		{
			auto workspace = ln::makeObject<lna::Workspace>();
			auto projectFile = lna::Workspace::findProejctFile(ln::Environment::currentDirectory());

			ln::String target = workspace->buildEnvironment()->defaultTargetName();
			if (burildTargetArg->hasValue()) {
				target = burildTargetArg->value();
			}

			if (!workspace->openMainProject(projectFile)) {
				return 1;
			}

			BuildCommand cmd;
			cmd.package = buildCommand_packageOption->isSet();
			cmd.target = target;
			return cmd.execute(workspace, workspace->mainProject());
		}
		//--------------------------------------------------------------------------------
		// run command
		else if (parser.has(runCommand))
		{
			auto workspace = ln::makeObject<lna::Workspace>();
			auto projectFile = lna::Workspace::findProejctFile(ln::Environment::currentDirectory());

			ln::String target = workspace->buildEnvironment()->defaultTargetName();
			if (runCommand_targetArg->hasValue()) {
				target = runCommand_targetArg->value();
			}

			if (!workspace->openMainProject(projectFile)) {
				return 1;
			}


			//BuildCommand cmd;
			//cmd.target = target;
			//if (cmd.execute(workspace, workspace->mainProject()) != 0) {
			//	return 1;
			//}

			workspace->buildEnvironment()->prepareEmscriptenSdk();
			if (!workspace->runProject(target)) {
				return 1;
			}
		}
		//--------------------------------------------------------------------------------
		// restore command
		else if (parser.has(restoreCommand))
		{
			auto workspace = ln::makeObject<lna::Workspace>();
			auto projectFile = lna::Workspace::findProejctFile(ln::Environment::currentDirectory());

			if (!workspace->openMainProject(projectFile)) {
				return 1;
			}
			if (!workspace->restoreProject()) {
				return 1;
			}
		}
		//--------------------------------------------------------------------------------
		// fxc command
		else if (parser.has(fxcCommand)) {
			FxcCommand cmd;
			if (fxcCommand_outputArg->hasValue()) {
				cmd.outputFile = fxcCommand_outputArg->value();
			}
			if (fxcCommand_exportArg->hasValue()) {
				cmd.exportDir = fxcCommand_exportArg->value();
			}
			return cmd.execute(fxcCommand_inputArg->value());
		}
		//--------------------------------------------------------------------------------
		// new-asset command
		else if (parser.has(newAssetCommand)) {
			auto workspace = ln::makeObject<lna::Workspace>();
			auto projectFile = lna::Workspace::findProejctFile(ln::Environment::currentDirectory());
			if (!workspace->openMainProject(projectFile)) {
				return 1;
			}

			NewAssetCommand cmd;
			if (newAssetCommand_outputArg->hasValue()) {
				cmd.filePath = newAssetCommand_outputArg->value();
			}
			return cmd.execute(workspace, newAssetCommand_nameArg->value());
		}
		//--------------------------------------------------------------------------------
		// build-assets command
		//else if (parser.has(buildAssetsCommand)) {
		//    if (!workspace->openProject(ln::Environment::currentDirectory())) {
		//        return 1;
		//    }
		//    ArchiveCommand cmd;
		//    return cmd.execute(workspace->project());
		//}
		//--------------------------------------------------------------------------------
		//else if (parser.has(dev_installTools))
		//{
		//	if (!workspace->dev_installTools()) {
		//		return 1;
		//	}
		//}
		//else if (parser.has(dev_openide))
		//{
		//	if (!workspace->openMainProject(projectFile)) {
		//		return 1;
		//	}
		//	workspace->dev_openIde(dev_openide_targetArg->value());
		//}
		else
		{
			parser.printHelp();
		}
	}
	else {
		parser.printHelp();
	}

	return 1;
}

