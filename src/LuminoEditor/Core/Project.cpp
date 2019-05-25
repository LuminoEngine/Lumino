//#include "EnvironmentSettings.hpp"
//#include "Workspace.hpp"
//#include "LanguageContext.hpp"
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
}
