﻿
#include "EnvironmentSettings.hpp"
#include "Workspace.hpp"
#include "Project.hpp"
#include "ProjectTemplateManager.hpp"

namespace lna {

//==============================================================================
// ProjectTemplateManager

ProjectTemplateManager::ProjectTemplateManager()
{
}

void ProjectTemplateManager::search()
{
	m_templates.clear();

	for (auto dir : ln::FileSystem::getDirectories(Workspace::instance()->buildEnvironment()->projectTemplatesDirPath())) {
		auto t = ln::makeObject2<ProjectTemplate>();
		t->fullName = dir.fileName();
		if (t->fullName.indexOf(u"cpp-", 0, ln::CaseSensitivity::CaseInsensitive) == 0) {
			t->lang = u"cpp";
		}
		else if (t->fullName.indexOf(u"ruby-", 0, ln::CaseSensitivity::CaseInsensitive) == 0) {
			t->lang = u"ruby";
		}
		else {
			// invalid proj dir.
		}

		if (!t->lang.isEmpty()) {
			t->shortName = t->fullName.substr(t->lang.length() + 1);
			t->directoryPath = dir;
			m_templates.add(t);
		}
	}
}

ProjectTemplate* ProjectTemplateManager::findTemplate(const ln::String& primaryLang, const ln::String& templateName) const
{
	// First, Find Perfect matching.
	// e.g.) lumino new -t cpp-cmake
	for (auto& t : m_templates) {
		if (ln::String::compare(t->fullName, templateName, ln::CaseSensitivity::CaseInsensitive) == 0) {
			return t;
		}
	}

	// When language is specified in templateName.
	// e.g.) lumino new -t=cpp
	auto defaultName = ln::String::concat(templateName, u"-default");
	for (auto& t : m_templates) {
		if (ln::String::compare(t->fullName, defaultName, ln::CaseSensitivity::CaseInsensitive) == 0) {
			return t;
		}
	}

	// templateName omitted.
	// e.g.) lumino new
	// e.g.) lumino --lang=ruby new
	if (templateName.isEmpty()) {
		auto name = ln::String::concat(primaryLang, u"-default");
		for (auto& t : m_templates) {
			if (ln::String::compare(t->fullName, name, ln::CaseSensitivity::CaseInsensitive) == 0) {
				return t;
			}
		}
	}
	else {
		auto name = ln::String::concat(primaryLang, u"-", templateName);
		for (auto& t : m_templates) {
			if (ln::String::compare(t->fullName, name, ln::CaseSensitivity::CaseInsensitive) == 0) {
				return t;
			}
		}
	}

	return nullptr;
}

ln::Result ProjectTemplateManager::applyTemplates(const Project* project, const ln::String& templateName) const
{
	auto templateProject = findTemplate(project->workspace()->primaryLang(), templateName);
	if (!templateProject) {
		CLI::error(u"Invalid project template.");
		return false;
	}

	auto projectTemplatesDir = project->workspace()->buildEnvironment()->projectTemplatesDirPath();
	auto dstRoot = project->rootDirPath();
	auto srcRoot = templateProject->directoryPath;

	CLI::info(u"Template: " + templateName);

	// 先にフォルダを作っておく
	for (auto dir : ln::FileSystem::getDirectories(srcRoot, ln::StringRef(), ln::SearchOption::Recursive)) {
		auto rel = srcRoot.makeRelative(dir);
		ln::FileSystem::createDirectory(ln::Path(dstRoot, rel));
	}

	// ファイルをコピー
	for (auto file : ln::FileSystem::getFiles(srcRoot, ln::StringRef(), ln::SearchOption::Recursive)) {
		auto rel = srcRoot.makeRelative(file);
		ln::FileSystem::copyFile(file, ln::Path(dstRoot, rel));
	}

	CLI::info("Copied template.");
	return true;
}

} // namespace lna
