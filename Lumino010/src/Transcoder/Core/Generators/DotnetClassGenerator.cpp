
#include "DotnetClassGenerator.hpp"

//==============================================================================
// DotnetClassGenerator

void DotnetClassGenerator::generate()
{
    //for (auto& structSymbol : db()->structs()) {

    //    code.AppendLine("//==============================================================================");
    //    code.AppendLine("// {0}", structSymbol->fullName());
    //    code.NewLine();

    //    code.AppendLine(u"VALUE {0};", makeRubyClassInfoVariableName(structSymbol));
    //    code.NewLine();

    //    code.AppendLine(m_RubyRequiredStructMethodsTemplate
    //        .replace(u"%%FlatStructName%%", makeFlatClassName(structSymbol)));
    //}


    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(u"Dotnet", u"Lumino.Dotnet"));
        ln::FileSystem::createDirectory(outputDir);

        auto src = u"";

        auto fileName = ln::String::format("{0}.Class.generated.cs", config()->moduleName);
        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), src);
    }
}
