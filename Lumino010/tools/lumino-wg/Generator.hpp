#pragma once
#include "OutputBuffer.hpp"

class SymbolDatabase;

class Generator
{
public:
	static const ln::String NewLine;

	void setup(SymbolDatabase* database, const ln::Path& templateDir, const ln::Path& outputDir, const ln::String& moduleFullName, const ln::String& moduleShortName)
	{
		m_database = database;
		m_templateDir = templateDir;
		m_outputDir = outputDir;
		m_moduleFullName = moduleFullName;
		m_moduleShortName = moduleShortName;
	}

	SymbolDatabase* db() const { return m_database; }
	ln::Path makeTemplateFilePath(const ln::Path& localPath) const { return ln::Path(m_templateDir, localPath); }
	ln::Path makeOutputFilePath(const ln::Path& localPath) const { return ln::Path(m_outputDir, localPath); }
	const ln::String& moduleFullName() const { return m_moduleFullName; }
	const ln::String& moduleShortName() const { return m_moduleShortName; }

private:
	SymbolDatabase* m_database;
	ln::Path m_templateDir;
	ln::Path m_outputDir;
	ln::String m_moduleFullName;
	ln::String m_moduleShortName;
};
