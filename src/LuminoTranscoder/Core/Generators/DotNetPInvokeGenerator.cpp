
#include "DotNetPInvokeGenerator.hpp"

//==============================================================================
// DotNetPInvokeGenerator

void DotNetPInvokeGenerator::generate()
{

    OutputBuffer code;
    code.AppendLine("using System;");
    code.NewLine();
    code.AppendLine(u"namespace {0}", config()->moduleName);
    code.AppendLine(u"{");
    code.IncreaseIndent();
    {
        code.AppendLines(generateEnums());
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

        code.AppendLine("public enum {0}", enumSymbol->shortName());
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

ln::String DotNetPInvokeGenerator::makeDocumentComment(DocumentInfo* doc) const
{
    OutputBuffer code;
    code.AppendLine(u"/// <summary>");
    code.AppendLine(u"/// " + doc->summary());
    code.AppendLine(u"/// </summary>");

    for (auto& param : doc->params()) {
        code.AppendLine(u"/// <param name=\"{0}\">{1}</param>", param->name(), param->description());
    }

    if (!doc->returns().isEmpty())
        code.AppendLine(u"/// <returns>{0}</returns>", doc->returns());

    if (!doc->details().isEmpty()) {
        code.AppendLine(u"/// <remakes>");
        code.AppendLine(u"/// " + doc->details());
        code.AppendLine(u"/// </remakes>");
    }

    return code.toString().trim();
}
