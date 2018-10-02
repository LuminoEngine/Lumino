#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "LanguageContext.hpp"
#include "Project.hpp"

//==============================================================================
// Project

const ln::String Project::ProjectFileExt = _T(".lnproj");

Project::Project(Workspace* owner)
	: m_workspace(owner)
{
}

Project::~Project()
{
}

Result Project::newProject(const ln::Path& projectDir, const ln::String& projectName)
{
	m_rootDir = projectDir.canonicalize();
	m_properties = ln::makeRef<ProjectProperties>();
	setupPathes();

	CLI::info(u"\nCreating a new Lumino app in " + m_rootDir + u"\n");

	m_projectName = projectName;
	m_properties->language = u"cpp";
	ln::FileSystem::createDirectory(m_engineDir);
	ln::FileSystem::createDirectory(m_sourcesDir);
	ln::FileSystem::createDirectory(m_assetsDir);
	ln::FileSystem::createDirectory(projectsDir());
	ln::FileSystem::createDirectory(m_buildDir);

	m_context = ln::makeRef<CppLanguageContext>(this);
	m_context->applyTemplates();
	
	Result result = saveProject();
	CLI::info(u"\nSuccess! Created " + m_projectName + u" at " + m_rootDir + u"\n");
	return result;
	/* TODO: もうちょっと詳しく出力したい
		例：
		Success! Created my-app at C:\LocalProj\tmp\my-app
		Inside that directory, you can run several commands:

		  npm start
			Starts the development server.

		  npm run build
			Bundles the app into static files for production.

		  npm test
			Starts the test runner.

		  npm run eject
			Removes this tool and copies build dependencies, configuration files
			and scripts into the app directory. If you do this, you can’t go back!

		We suggest that you begin by typing:

		  cd my-app
		  npm start

		Happy hacking!
	*/
}

Result Project::openProject(const ln::Path& dir)
{
	m_rootDir = dir.canonicalize();
	m_properties = ln::makeRef<ProjectProperties>();


	setupPathes();


	ln::String json = ln::FileSystem::readAllText(m_projectFilePath);
	ln::JsonSerializer::deserialize(json, *m_properties);

	if (m_properties->language == "cpp") {
		m_context = ln::makeRef<CppLanguageContext>(this);
	}
	else {
		LN_NOTIMPLEMENTED();
	}

	return Result::OK;
}

Result Project::saveProject()
{
	auto json = ln::JsonSerializer::serialize(*m_properties);
	auto path = ln::Path(m_rootDir, m_projectName + ProjectFileExt);
	ln::FileSystem::writeAllText(path.c_str(), json);	// TODO: remove c_str()

	return Result::OK;
}

void Project::restore()
{
	m_context->restore();
}

bool Project::existsProjectFile(const ln::Path& dir)
{
	auto files = ln::FileSystem::getFiles(dir, u"*" + ProjectFileExt);
	return !files.isEmpty();
}

void Project::setupPathes()
{
	m_engineDir = ln::Path(m_rootDir, u"Engine");
	m_sourcesDir = ln::Path(m_rootDir, u"Sources");
	m_assetsDir = ln::Path(m_rootDir, u"Assets");
	m_buildDir = ln::Path(m_rootDir, u".lumino");

	auto files = ln::FileSystem::getFiles(m_rootDir, u"*" + ProjectFileExt);
	if (!files.isEmpty()) {
		m_projectFilePath = *files.begin();
	}
	else {
		LN_NOTIMPLEMENTED();
	}
}
