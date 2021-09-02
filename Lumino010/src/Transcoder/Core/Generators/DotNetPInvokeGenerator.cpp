
#include "DotNetPInvokeGenerator.hpp"

//==============================================================================
// DotNetPInvokeGenerator

void DotNetPInvokeGenerator::generate()
{

    OutputBuffer code;
    code.AppendLine("using System;");
    code.AppendLine("using System.Runtime.InteropServices;");
    code.NewLine();
    code.AppendLine(u"namespace {0}", config()->moduleName);
    code.AppendLine(u"{");
    code.IncreaseIndent();
    {
        code.AppendLines(generateEnums());
        code.AppendLines(generateStructs());
    }
    code.DecreaseIndent();
    code.AppendLine(u"}");

    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(u"Dotnet", u"Lumino.Dotnet"));
        ln::FileSystem::createDirectory(outputDir);

        auto fileName = ln::String::format("{0}.PInvoke.generated.cs", config()->moduleName);
        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), code.toString());
    }
}

ln::String DotNetPInvokeGenerator::generateEnums()
{
    OutputBuffer code;

    for (auto& enumSymbol : db()->enums()) {
        code.AppendLines(makeDocumentComment(enumSymbol->document()));

        if (enumSymbol->isFlags())
            code.AppendLine(u"[Flags]");

        code.AppendLine(u"public enum {0}", enumSymbol->shortName());
        code.AppendLine(u"{");
        code.IncreaseIndent();

        for (auto& member : enumSymbol->constants()) {
            code.AppendLine(makeDocumentComment(member->document()));
            code.AppendLine(u"{0} = {1},", member->name(), member->value()->get<int>());
            code.NewLine();
        }

        code.DecreaseIndent();
        code.AppendLine(u"}");

        code.NewLine();
    }

    return code.toString();
}

ln::String DotNetPInvokeGenerator::generateStructs()
{
    OutputBuffer code;

    for (auto& structSymbol : db()->structs()) {
        code.AppendLines(makeDocumentComment(structSymbol->document()));

        code.AppendLine(u"[StructLayout(LayoutKind.Sequential)]");
        code.AppendLine(u"public struct {0}", structSymbol->shortName());
        code.AppendLine(u"{");
        code.IncreaseIndent();

        //for (auto& member : enumSymbol->constants()) {
        //    code.AppendLine(makeDocumentComment(member->document()));
        //    code.AppendLine(u"{0} = {1},", member->name(), member->value()->get<int>());
        //    code.NewLine();
        //}

        code.DecreaseIndent();
        code.AppendLine(u"}");

        code.NewLine();
    }

    return code.toString();
}

ln::String DotNetPInvokeGenerator::makeDocumentComment(DocumentInfo* doc) const
{
    OutputBuffer code;
    code.AppendLine(u"/// <summary>");
    code.AppendLinesHeaderd2(u"/// ", doc->summary());
    code.AppendLine(u"/// </summary>");

    for (auto& param : doc->params()) {
        code.AppendLine(u"/// <param name=\"{0}\">{1}</param>", param->name(), param->description());
    }

    if (!doc->returns().isEmpty())
        code.AppendLine(u"/// <returns>{0}</returns>", doc->returns());

    if (!doc->details().isEmpty()) {
        code.AppendLine(u"/// <remakes>");
        code.AppendLinesHeaderd2(u"/// ", doc->details());
        code.AppendLine(u"/// </remakes>");
    }

    return code.toString().trim();
}
