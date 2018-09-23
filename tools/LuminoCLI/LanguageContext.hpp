#pragma once

class Workspace;
class Project;

class LanguageContext
	: public ln::RefObject
{
public:
	LanguageContext(Project* project);
	virtual ~LanguageContext();

	Project* project() const { return m_project; }

	virtual Result applyTemplates() = 0;

private:
	Project* m_project;
};

class CppLanguageContext
	: public LanguageContext
{
public:
	CppLanguageContext(Project* project);
	virtual ~CppLanguageContext();

	virtual Result applyTemplates() override;
};
