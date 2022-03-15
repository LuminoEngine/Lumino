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

	//virtual ln::Result applyTemplates(const ln::String& templateName) = 0;
	virtual void restore() = 0;
	virtual ln::Result build(const ln::String& target);

	static ln::Result buildAssets(const ln::Path& intermediateDir, const ln::Path& inputDir, const ln::Path& outputFile);

private:
	ln::Result buildAssets() const;

	Project* m_project;
};

class CppLanguageContext
	: public LanguageContext
{
public:
	CppLanguageContext(Project* project);
	virtual ~CppLanguageContext();

	//virtual ln::Result applyTemplates(const ln::String& templateName) override;
	virtual void restore() override;
	virtual ln::Result build(const ln::String& target) override;

private:
	ln::Result build_NativeCMakeTarget() const;
	ln::Result build_WebTarget() const;
};

} // namespace lna



