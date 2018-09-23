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

Result Project::newProject(const ln::Path& dir, const ln::String& projectName)
{
	m_rootDir = dir.canonicalize();
	m_properties = ln::makeRef<ProjectProperties>();
	setupPathes();

	m_projectName = projectName;
	m_properties->language = u"cpp";
	ln::FileSystem::createDirectory(m_sourcesDir);
	ln::FileSystem::createDirectory(m_assetsDir);
	ln::FileSystem::createDirectory(m_buildDir);

	m_context = ln::makeRef<CppLanguageContext>(this);
	m_context->applyTemplates();
	
	return saveProject();
}

Result Project::openProject(const ln::Path& dir)
{
	m_rootDir = dir.canonicalize();
	m_properties = ln::makeRef<ProjectProperties>();
	setupPathes();
	return Result::OK;
}

Result Project::saveProject()
{
	auto json = ln::JsonSerializer::serialize(*m_properties);
	auto path = ln::Path(m_rootDir, m_projectName + ProjectFileExt);
	ln::FileSystem::writeAllText(path.c_str(), json);	// TODO: remove c_str()

	return Result::OK;
}

bool Project::existsProjectFile(const ln::Path& dir)
{
	auto files = ln::FileSystem::getFiles(dir, u"*" + ProjectFileExt);
	return !files.isEmpty();
}

void Project::setupPathes()
{
	m_sourcesDir = ln::Path(m_rootDir, _T("Sources"));
	m_assetsDir = ln::Path(m_rootDir, _T("Assets"));
	m_buildDir = ln::Path(m_rootDir, _T(".lumino"));
}
