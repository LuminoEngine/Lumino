#pragma once
#include "OutputBuffer.h"

class SymbolDatabase;

class Generator
{
public:
	static const String NewLine;

	void setup(SymbolDatabase* database, const Path& templateDir, const Path& outputDir, const String& moduleFullName, const String& moduleShortName)
	{
		m_database = database;
		m_templateDir = templateDir;
		m_outputDir = outputDir;
		m_moduleFullName = moduleFullName;
		m_moduleShortName = moduleShortName;
	}

	SymbolDatabase* db() const { return m_database; }
	Path makeTemplateFilePath(const Path& localPath) const { return Path(m_templateDir, localPath); }
	Path makeOutputFilePath(const Path& localPath) const { return Path(m_outputDir, localPath); }
	const String& moduleFullName() const { return m_moduleFullName; }
	const String& moduleShortName() const { return m_moduleShortName; }

private:
	SymbolDatabase* m_database;
	Path m_templateDir;
	Path m_outputDir;
	String m_moduleFullName;
	String m_moduleShortName;
};
