
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

	for (auto dir : ln::FileSystem::getDirectories(APP->buildEnvironment()->projectTemplatesDirPath())) {
		auto t = ln::makeObject2<ProjectTemplate>();
		t->fullName = dir.fileName();
		if (t->fullName.indexOf(_TT("cpp-"), 0, ln::CaseSensitivity::CaseInsensitive) == 0) {
			t->lang = _TT("cpp");
		}
		else if (t->fullName.indexOf(_TT("ruby-"), 0, ln::CaseSensitivity::CaseInsensitive) == 0) {
			t->lang = _TT("ruby");
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
	auto defaultName = ln::String::concat(templateName, _TT("-default"));
	for (auto& t : m_templates) {
		if (ln::String::compare(t->fullName, defaultName, ln::CaseSensitivity::CaseInsensitive) == 0) {
			return t;
		}
	}

	// templateName omitted.
	// e.g.) lumino new
	// e.g.) lumino --lang=ruby new
	if (templateName.isEmpty()) {
		auto name = ln::String::concat(primaryLang, _TT("-default"));
		for (auto& t : m_templates) {
			if (ln::String::compare(t->fullName, name, ln::CaseSensitivity::CaseInsensitive) == 0) {
				return t;
			}
		}
	}
	else {
		auto name = ln::String::concat(primaryLang, _TT("-"), templateName);
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
		CLI::error(_TT("Invalid project template."));
		return ln::err();
	}

	auto projectTemplatesDir = project->workspace()->buildEnvironment()->projectTemplatesDirPath();
	auto dstRoot = project->rootDirPath();
	auto srcRoot = templateProject->directoryPath;

	CLI::info(_TT("Template: ") + templateName);

	// 先にフォルダを作っておく
	for (auto dir : ln::FileSystem::getDirectories(srcRoot, ln::StringView(), ln::SearchOption::Recursive)) {
		auto rel = srcRoot.makeRelative(dir);
		ln::FileSystem::createDirectory(ln::Path(dstRoot, rel));
	}

	// ファイルをコピー
	for (auto file : ln::FileSystem::getFiles(srcRoot, ln::StringView(), ln::SearchOption::Recursive)) {
		auto rel = srcRoot.makeRelative(file);
		ln::FileSystem::copyFile(file, ln::Path(dstRoot, rel));
	}

	CLI::info(_TT("Copied template."));
	return ln::ok();
}

} // namespace lna
