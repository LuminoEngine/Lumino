#pragma once

namespace lna {
class Workspace;
class Project;

class LanguageContext
	: public ln::RefObject
{
public:
	LanguageContext(Project* project);
	virtual ~LanguageContext();

	Project* project() const { return m_project; }

	virtual ln::Result applyTemplates(const ln::String& templateName) = 0;
    virtual ln::Result applyEngine() = 0;
	virtual void restore() = 0;

private:
	Project* m_project;
};

class CppLanguageContext
	: public LanguageContext
{
public:
	CppLanguageContext(Project* project);
	virtual ~CppLanguageContext();

	virtual ln::Result applyTemplates(const ln::String& templateName) override;
    virtual ln::Result applyEngine() override;
	virtual void restore() override;

private:
};

} // namespace lna



