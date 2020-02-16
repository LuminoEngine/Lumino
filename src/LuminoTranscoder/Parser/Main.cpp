
#include "HeaderParser2.hpp"

int main(int argc, const char** argv)
{
#if _DEBUG
	if (argc == 1) {
		static const char* localArgv[] = {
			argv[0],
			"C:/Users/lriki/AppData/Local/Temp/lnCodeAnalyzerCDB-22",
		};
		argc = 2;
		argv = localArgv;
	}
#endif
	if (argc != 2) {
		std::cerr << "Invalid args." << std::endl;
		return 1;
	}

	// dump args
	for (int i = 0; i < argc; i++) {
		std::cerr << argv[i] << std::endl;
	}

	CompilationDatabase cdb;
	cdb.load(ln::String::fromCString(argv[1]));

	HeaderParser2 parser;
	for (auto& path : cdb.includeDirectories) parser.addIncludePath(path);
	for (auto& path : cdb.forceIncludeFiles) parser.addForceIncludeFile(path);

	auto pidb = ln::makeRef<PIDatabase>();
	auto diag = ln::makeObject<ln::DiagnosticsManager>();
	int result = parser.parse(cdb.inputFile, pidb, diag);
	
	std::cerr << "Internal exit code: " << result << std::endl;

	pidb->save(cdb.outputPIDB);

	// TODO: logging
	diag->dumpToLog();

	return 0;
}
