#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"

//==============================================================================
// ProjectLanguageContext

ProjectLanguageContext::ProjectLanguageContext(Project* project)
	: m_project(project)
{
}

ProjectLanguageContext::~ProjectLanguageContext()
{
}

//==============================================================================
// CppProjectLanguageContext

CppProjectLanguageContext::CppProjectLanguageContext(Project* project)
	: ProjectLanguageContext(project)
{
}

CppProjectLanguageContext::~CppProjectLanguageContext()
{
}

Result CppProjectLanguageContext::applyTemplates()
{
	auto projectTemplatesDir = project()->workspace()->environmentSettings()->projectTemplatesDirPath();

	// Common
	{
		ln::String files[] = {
			"CMakeLists.txt",
			"LuminoCppTemplate.sln",
			"Sources/Application.cpp",
			"Sources/Application.h",
			"Sources/LuminoCppTemplate.vcxproj",
			"Sources/LuminoCppTemplate.vcxproj.filters",
		};

		auto destRoot = project()->rootDirPath();
		auto srcRoot = ln::Path(projectTemplatesDir, _T("LuminoCppTemplate"));

		for (auto& file : files)
		{
			ln::FileSystem::copyFile(ln::Path(srcRoot, file), ln::Path(destRoot, file), ln::FileCopyOption::Overwrite);
		}
	}

	// Win32
	if (0)
	{
		ln::String files[] = {
			"LuminoCppTemplate.cpp",
			"LuminoCppTemplate.h",
			"LuminoCppTemplate.ico",
			"LuminoCppTemplate.rc",
			"LuminoCppTemplate.vcxproj",
			"LuminoCppTemplate.vcxproj.filters",
			"Resource.h",
			"small.ico",
			"stdafx.cpp",
			"stdafx.h",
			"targetver.h",
		};

		auto destRoot = ln::Path(project()->buildDir(), _T("Win32"));
		auto srcRoot = ln::Path(projectTemplatesDir, _T("LuminoCppTemplate/LuminoCppTemplate"));
		ln::FileSystem::createDirectory(destRoot);

		for (auto& file : files)
		{
			ln::FileSystem::copyFile(ln::Path(srcRoot, file), ln::Path(destRoot, file), ln::FileCopyOption::Overwrite);
		}
	}

	return Result::OK;
}

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
	m_properties->language = _T("cpp");
	ln::FileSystem::createDirectory(m_sourcesDir);
	ln::FileSystem::createDirectory(m_assetsDir);
	ln::FileSystem::createDirectory(m_buildDir);

	m_context = ln::makeRef<CppProjectLanguageContext>(this);
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

void Project::setupPathes()
{
	m_sourcesDir = ln::Path(m_rootDir, _T("Sources"));
	m_assetsDir = ln::Path(m_rootDir, _T("Assets"));
	m_buildDir = ln::Path(m_rootDir, _T("Build"));
}
