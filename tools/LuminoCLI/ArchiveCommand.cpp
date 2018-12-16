
#include "../../src/LuminoEngine/src/Asset/AssetArchive.hpp"
#include "../../src/LuminoEngine/src/Shader/UnifiedShader.hpp"
#include "Project.hpp"
#include "ArchiveCommand.hpp"
#include "FxcCommand.hpp"

int ArchiveCommand::execute(Project* project)
{
    ln::detail::CryptedAssetArchiveWriter writer;
    auto outputFilePath = ln::Path(project->buildDir(), u"Assets.lca");
    writer.open(outputFilePath, ln::detail::CryptedArchiveHelper::DefaultPassword);

    for (auto& file : ln::FileSystem::getFiles(project->assetsDir(), ln::StringRef(), ln::SearchOption::Recursive)) {
        if (file.hasExtension(".fx")) {
            auto workFile = ln::Path(project->buildDir(), file.fileName().replaceExtension(ln::detail::UnifiedShader::FileExt));

            FxcCommand cmd;
            cmd.outputFile = workFile;
            cmd.execute(file);

            writer.addFile(workFile, project->assetsDir().makeRelative(file).replaceExtension(ln::detail::UnifiedShader::FileExt));
            CLI::info(file);
        }
        else {
            writer.addFile(file, project->assetsDir().makeRelative(file));
            CLI::info(file);
        }
    }

    writer.close();

    // Android
    {
        auto dst = ln::Path::combine(project->androidProjectDir(), u"app", u"src", u"main", u"assets", u"Assets.lca");
        ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
        CLI::info(u"Copy to " + dst);
    }

    // macOS
    {
        auto dst = ln::Path::combine(project->macOSProjectDir(), u"LuminoApp.macOS", u"Assets.lca");
        ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
        CLI::info(u"Copy to " + dst);
    }

    // iOS
    {
        auto dst = ln::Path::combine(project->macOSProjectDir(), u"LuminoApp.iOS", u"Assets.lca");
        ln::FileSystem::copyFile(outputFilePath, dst, ln::FileCopyOption::Overwrite);
        CLI::info(u"Copy to " + dst);
    }

    CLI::info(u"Compilation succeeded; see " + outputFilePath);

    return 0;
}
