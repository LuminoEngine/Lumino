﻿#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "LanguageContext.hpp"
#include "ProjectTemplateManager.hpp"
#include "Project.hpp"

namespace lna {

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

ln::Result Project::newProject(const ln::Path& projectDir, const ln::String& projectName, const ln::String& engineSource, const ln::String& templateName)
{
	m_rootDir = projectDir.canonicalize();
	m_projectName = projectName;
	m_properties = ln::makeRef<ProjectProperties>();

	ln::FileSystem::createDirectory(m_rootDir);

	setupPathes();

	m_projectFilePath = ln::Path(m_rootDir, m_projectName + ProjectFileExt);
	if (ln::FileSystem::existsFile(m_projectFilePath)) {
		CLI::error("Lumino project already exists.");
		return false;
	}

	CLI::info(u"\nCreating a new Lumino app in " + m_rootDir + u"\n");

	m_properties->language = u"cpp";
    m_properties->engine = engineSource;
    if (m_properties->engine.isEmpty()) {
        m_properties->engine = u"system";
    }

	//ln::FileSystem::createDirectory(m_engineDir);
	//ln::FileSystem::createDirectory(m_sourcesDir);
	//ln::FileSystem::createDirectory(m_assetsDir);
	//ln::FileSystem::createDirectory(projectsDir());
	//ln::FileSystem::createDirectory(m_buildDir);
	//ln::FileSystem::setAttribute(m_buildDir, ln::FileAttribute::Hidden);

	m_context = ln::makeRef<CppLanguageContext>(this);
	
	if (!m_workspace->projectTemplateManager()->applyTemplates(this, templateName)) {
		return false;
	}
    //if (!m_context->applyEngine()) {
    //    return false;
    //}
	
    ln::Result result = saveProject();
	CLI::info(u"\nSuccess! Created " + m_projectName + u" at " + m_rootDir + u"\n");

    //if (!postInitialize()) {
    //    return false;
    //}

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

ln::Result Project::openProject2(const ln::Path& projectFile)
{
    m_projectFilePath = projectFile.canonicalize();
	m_rootDir = m_projectFilePath.parent();
	m_properties = ln::makeRef<ProjectProperties>();

	setupPathes();
	if (m_projectFilePath.isEmpty()) {
		CLI::error("Lumino project file not found in current directory.");
		return false;
	}

	if (!loadProject()) {
		return false;
	}

	if (m_properties->language == "cpp") {
		m_context = ln::makeRef<CppLanguageContext>(this);
	}
	else {
		LN_NOTIMPLEMENTED();
	}

    //if (!postInitialize()) {
    //    return false;
    //}

	return true;
}

ln::Result Project::saveProject()
{
	auto json = ln::JsonSerializer::serialize(*m_properties);
	ln::FileSystem::writeAllText(m_projectFilePath, json);
	return true;
}

ln::Result Project::loadProject()
{
	ln::String json = ln::FileSystem::readAllText(m_projectFilePath);
	ln::JsonSerializer::deserialize(json, *m_properties);
	return true;
}

void Project::close()
{
    
}

void Project::restore()
{
	m_context->restore();
}

const ln::Path& Project::acquireBuildDir() const
{
	ln::FileSystem::createDirectory(m_buildDir);
	return m_buildDir;
}

bool Project::existsProjectFile(const ln::Path& dir)
{
    if (ln::FileSystem::existsDirectory(dir)) {
        auto files = ln::FileSystem::getFiles(dir, u"*" + ProjectFileExt);
        return !files.isEmpty();
    }
    else {
        return false;
    }
}

void Project::setupPathes()
{
	m_engineDir = ln::Path(m_rootDir, u"engine");
	m_sourcesDir = ln::Path(m_rootDir, u"src");
	m_assetsDir = ln::Path(m_rootDir, u"assets");
	m_buildDir = ln::Path(m_rootDir, u".ln");
	m_releaseDir = ln::Path(m_rootDir, u"release");
	
	m_intermediateAssetsDir = ln::Path(m_buildDir, u"assets");
    m_localPluginDir = ln::Path(m_buildDir, u"plugins");

	//auto files = ln::FileSystem::getFiles(m_rootDir, u"*" + ProjectFileExt);
	//if (!files.isEmpty()) {
	//	m_projectFilePath = *files.begin();
	//}
}

//ln::Result Project::postInitialize()
//{
//}
//
#if 0
//#include "EnvironmentSettings.hpp"
//#include "Workspace.hpp"
//#include "LanguageContext.hpp"
#include "AssetDatabase.hpp"
#include "Project.hpp"

//==============================================================================
// Project

const ln::String Project::ProjectFileExt = _T(".lnproj");

Project::Project()
{
}

Project::~Project()
{
}

ln::Result Project::initForNew(const ln::Path& projectDir, const ln::String& projectName, const ln::String& engineSource, const ln::String& templateName)
{
    LN_NOTIMPLEMENTED();
    return false;
}

ln::Result Project::initForOpen(const ln::Path& dir)
{
	m_rootDir = dir.canonicalize();
	m_properties = ln::makeRef<ProjectProperties>();

	setupPathes();
	if (m_projectFilePath.isEmpty()) {
		LN_LOG_ERROR << ("Lumino project file not found in current directory.");
		return false;
	}

	if (!loadProject()) {
		return false;
	}

	//if (m_properties->language == "cpp") {
	//	m_context = ln::makeRef<CppLanguageContext>(this);
	//}
	//else {
	//	LN_NOTIMPLEMENTED();
	//}

	return true;
}

ln::Result Project::saveProject()
{
	auto json = ln::JsonSerializer::serialize(*m_properties);
	ln::FileSystem::writeAllText(m_projectFilePath, json);
	return true;
}

ln::Result Project::loadProject()
{
	ln::String json = ln::FileSystem::readAllText(m_projectFilePath);
	ln::JsonSerializer::deserialize(json, *m_properties);
	return true;
}

void Project::restore()
{
    LN_NOTIMPLEMENTED();
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
	m_buildDir = ln::Path(m_rootDir, u".ln");
	m_releaseDir = ln::Path(m_rootDir, u"Release");
	
	m_intermediateAssetsDir = ln::Path(m_buildDir, u"Assets");

	auto files = ln::FileSystem::getFiles(m_rootDir, u"*" + ProjectFileExt);
	if (!files.isEmpty()) {
		m_projectFilePath = *files.begin();
	}

    m_assetDatabase = ln::makeObject<AssetDatabase>();
}

#endif

} // namespace lna

