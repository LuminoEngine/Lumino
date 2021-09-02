#pragma once

namespace lna {
class Workspace;
class Project;

class ProjectTemplate
	: public ln::Object
{
public:
	ln::String fullName;
	ln::String shortName;
	ln::String lang;
	ln::Path directoryPath;
};

class ProjectTemplateManager
	: public ln::Object
{
public:
	ProjectTemplateManager();
	void search();
	ProjectTemplate* findTemplate(const ln::String& primaryLang, const ln::String& templateName) const;

	ln::Result applyTemplates(const Project* project, const ln::String& templateName) const;

private:
	ln::List<Ref<ProjectTemplate>> m_templates;
};

} // namespace lna



