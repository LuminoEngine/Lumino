
#include "../../src/LuminoEngine/src/Asset/AssetArchive.hpp"
#include "Project.hpp"
#include "ArchiveCommand.hpp"

int ArchiveCommand::execute(Project* project)
{
    ln::detail::CryptedAssetArchiveWriter writer;
    writer.open(ln::Path(project->buildDir(), u"Assets.lca"), u"pass");

    for (auto& file : ln::FileSystem::getFiles(project->assetsDir(), ln::StringRef(), ln::SearchOption::Recursive)) {
        writer.addFile(file, project->assetsDir().makeRelative(file));
    }

    writer.close();

    return 0;
}
