
#include "DotnetClassGenerator.hpp"

//==============================================================================
// DotnetClassGenerator

void DotnetClassGenerator::generate() {
    //for (auto& structSymbol : db()->structs()) {

    //    code.AppendLine("//==============================================================================");
    //    code.AppendLine("// {0}", structSymbol->fullName());
    //    code.NewLine();

    //    code.AppendLine(U"VALUE {0};", makeRubyClassInfoVariableName(structSymbol));
    //    code.NewLine();

    //    code.AppendLine(m_RubyRequiredStructMethodsTemplate
    //        .replace(U"%%FlatStructName%%", makeFlatClassName(structSymbol)));
    //}

    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(U"Dotnet", U"Lumino.Dotnet"));
        ln::FileSystem::createDirectory(outputDir);

        auto src = U"";

        auto fileName = ln::format(U"{0}.Class.generated.cs", config()->moduleName);
        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), src);
    }
}
