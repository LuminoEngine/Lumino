
#include "DotNetPInvokeGenerator.hpp"

//==============================================================================
// DotNetPInvokeGenerator

void DotNetPInvokeGenerator::generate() {

    OutputBuffer code;
    code.AppendLine(U"using System;");
    code.AppendLine(U"using System.Runtime.InteropServices;");
    code.NewLine();
    code.AppendLine(U"namespace {0}", config()->moduleName);
    code.AppendLine(U"{");
    code.IncreaseIndent();
    {
        code.AppendLines(generateEnums());
        code.AppendLines(generateStructs());
    }
    code.DecreaseIndent();
    code.AppendLine(U"}");

    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(U"Dotnet", U"Lumino.Dotnet"));
        ln::FileSystem::createDirectory(outputDir);

        auto fileName = ln::format(U"{0}.PInvoke.generated.cs", config()->moduleName);
        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), code.toString());
    }
}

ln::String DotNetPInvokeGenerator::generateEnums() {
    OutputBuffer code;

    for (auto& enumSymbol : db()->enums()) {
        code.AppendLines(makeDocumentComment(enumSymbol->document()));

        if (enumSymbol->isFlags())
            code.AppendLine(U"[Flags]");

        code.AppendLine(U"public enum {0}", enumSymbol->shortName());
        code.AppendLine(U"{");
        code.IncreaseIndent();

        for (auto& member : enumSymbol->constants()) {
            code.AppendLine(makeDocumentComment(member->document()));
            code.AppendLine(U"{0} = {1},", member->name(), member->value()->get<int>());
            code.NewLine();
        }

        code.DecreaseIndent();
        code.AppendLine(U"}");

        code.NewLine();
    }

    return code.toString();
}

ln::String DotNetPInvokeGenerator::generateStructs() {
    OutputBuffer code;

    for (auto& structSymbol : db()->structs()) {
        code.AppendLines(makeDocumentComment(structSymbol->document()));

        code.AppendLine(U"[StructLayout(LayoutKind.Sequential)]");
        code.AppendLine(U"public struct {0}", structSymbol->shortName());
        code.AppendLine(U"{");
        code.IncreaseIndent();

        //for (auto& member : enumSymbol->constants()) {
        //    code.AppendLine(makeDocumentComment(member->document()));
        //    code.AppendLine(U"{0} = {1},", member->name(), member->value()->get<int>());
        //    code.NewLine();
        //}

        code.DecreaseIndent();
        code.AppendLine(U"}");

        code.NewLine();
    }

    return code.toString();
}

ln::String DotNetPInvokeGenerator::makeDocumentComment(DocumentInfo* doc) const {
    OutputBuffer code;
    code.AppendLine(U"/// <summary>");
    code.AppendLinesHeaderd2(U"/// ", doc->summary());
    code.AppendLine(U"/// </summary>");

    for (auto& param : doc->params()) {
        code.AppendLine(U"/// <param name=\"{0}\">{1}</param>", param->name(), param->description());
    }

    if (!doc->returns().isEmpty())
        code.AppendLine(U"/// <returns>{0}</returns>", doc->returns());

    if (!doc->details().isEmpty()) {
        code.AppendLine(U"/// <remakes>");
        code.AppendLinesHeaderd2(U"/// ", doc->details());
        code.AppendLine(U"/// </remakes>");
    }

    return code.toString().trim();
}
