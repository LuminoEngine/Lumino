
#include "CodeAnalyzer.hpp"

CodeAnalyzer::CodeAnalyzer() {
}

void CodeAnalyzer::analyze(PIDatabase* pidb, ln::DiagnosticsManager* diag) {
    m_pidb = pidb;

    for (int i = 0; i < inputs.size(); i++) {
        callParser(inputs[i], i);
    }

    ////if (!ln::FileSystem::existsFile(U"pidb.json"))
    ////{
    //	if (files_LuminoCore) {
    //		for (auto& file : (*files_LuminoCore)) {
    //			CompilationDatabase cdb;
    //			cdb.inputFile = file;
    //			cdb.includeDirectories.add(luminoIncludePath);
    //			cdb.forceIncludeFiles.add(forceInclude_LuminoCore);
    //			callParser(cdb);

    //			//HeaderParser2 parser;
    //			//parser.addIncludePath(TEST_ROOT "include");
    //			//parser.addForceIncludeFile(TEST_ROOT "src/LuminoCore/src/LuminoCore.PCH.h");
    //			//parser.parse(file, pidb, diag);
    //		}
    //	}

    //	if (files_LuminoEngine) {
    //		for (auto& file : (*files_LuminoEngine)) {
    //			CompilationDatabase cdb;
    //			cdb.inputFile = file;
    //			cdb.includeDirectories.add(luminoIncludePath);
    //			cdb.forceIncludeFiles.add(forceInclude_LuminoEngine);
    //			callParser(cdb);

    //			//HeaderParser2 parser;
    //			//parser.addIncludePath(TEST_ROOT "include");
    //			//parser.addForceIncludeFile(TEST_ROOT "src/LuminoEngine/src/LuminoEngine.PCH.h");
    //			//parser.parse(file, pidb, diag);
    //		}
    //	}

    ////	pidb->save(U"pidb.json");
    ////}
    ////else
    ////{
    ////	pidb->load(U"pidb.json");
    ////}
}

void CodeAnalyzer::callParser(CompilationDatabase& cdb, int index) {
    auto cdbFile = ln::Path(ln::Environment::specialFolderPath(ln::SpecialFolder::Temporary), U"lnCodeAnalyzerCDB-" + ln::String::fromNumber(index)); // ln::Uuid::generate().toString());
    auto pidbFile = ln::Path(ln::Environment::specialFolderPath(ln::SpecialFolder::Temporary), U"lnCodeAnalyzerPIDB-" + ln::String::fromNumber(index));

    cdb.outputPIDB = pidbFile;
    cdb.save(cdbFile);

    {
        ln::Process p;
        p.setProgram(parserExecutable);
        p.setArguments({ cdbFile });
        p.setUseShellExecute(false);
        auto stderrReader = p.openStderr();
        p.start();

        ln::String line;
        while (stderrReader->readLine(&line)) {
            std::cerr << line.toStdString() << std::endl;
        }
        p.wait(); // ↑の while を抜けた時点で Process は終了しているはずだが、念のため

        // TODO: ログ出しちゃんと考える
        int exitCode = p.exitCode();
        std::cerr << "finish analysis (" << exitCode << ") : " << cdb.inputFile << std::endl;
        if (exitCode != 0) {
            std::cout << "Error occurred. (" << exitCode << ")" << std::endl;
            // TODO: Record ファイルができていなければ FatalError.
            return;
        }
    }

    // import PIDB
    auto localPIDB = ln::makeRef<PIDatabase>();
    localPIDB->load(pidbFile);
    m_pidb->mergeFrom(localPIDB);
}
