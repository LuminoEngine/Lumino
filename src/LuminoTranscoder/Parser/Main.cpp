
#include "HeaderParser2.hpp"

int main(int argc, const char** argv)
{
	if (argc != 2) {
		std::cerr << "Invalid args." << std::endl;
		return 1;
	}

	CompilationDatabase cdb;
	cdb.load(ln::String::fromCString(argv[1]));

	HeaderParser2 parser;
	for (auto& path : cdb.includeDirectories) parser.addIncludePath(path);
	for (auto& path : cdb.forceIncludeFiles) parser.addForceIncludeFile(path);

	auto pidb = ln::makeRef<PIDatabase>();
	auto diag = ln::makeObject<ln::DiagnosticsManager>();
	int result = parser.parse(cdb.inputFile, pidb, diag);

	if (result == 0) {
		pidb->save(cdb.outputPIDB);
	}

	// TODO: logging
	diag->dumpToLog();

	return result;
}
