
#pragma once
#include <unordered_map>
#include "../Parser/ParserIntermediates.hpp"

class CodeAnalyzer
{
public:
	// env
	ln::Path parserExecutable;
	ln::Path luminoIncludePath;
	ln::Path forceInclude_LuminoCore;
	ln::Path forceInclude_LuminoEngine;

	// input params
	//const ln::List<ln::Path>* files_LuminoCore;
	//const ln::List<ln::Path>* files_LuminoEngine;
	ln::List<CompilationDatabase> inputs;

	CodeAnalyzer();

	void analyze(PIDatabase* pidb, ln::DiagnosticsManager* diag);

	const Ref<PIDatabase>& pidb() const { return m_pidb; }

private:
	void callParser(CompilationDatabase& cdb, int index);

	ln::DiagnosticsManager* m_diag;
	PIDatabase* m_pidb;
};


