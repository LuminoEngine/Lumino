
#include "../../src/LuminoEngine/src/Asset/AssetArchive.hpp"
#include "Project.hpp"
#include "ArchiveCommand.hpp"

int ArchiveCommand::execute(Project* project)
{
    ln::detail::CryptedAssetArchiveWriter writer;
    auto outputFilePath = ln::Path(project->buildDir(), u"Assets.lca");
    writer.open(outputFilePath, u"j7OeF8Hh");

    for (auto& file : ln::FileSystem::getFiles(project->assetsDir(), ln::StringRef(), ln::SearchOption::Recursive)) {
        if (file.hasExtension(".fx")) {
            // ignore
        }
        else {
            writer.addFile(file, project->assetsDir().makeRelative(file));
        }
    }

    writer.close();

    CLI::info(u"Compilation succeeded; see " + outputFilePath);

    return 0;
}
