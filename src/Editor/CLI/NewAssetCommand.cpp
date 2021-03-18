
#include <Project/Project.hpp>
#include "NewAssetCommand.hpp"
#include "../../Engine/src/Engine/EngineDomain.hpp"
#include "../../Engine/src/Asset/AssetManager.hpp"

int NewAssetCommand::execute(lna::Workspace* workspace, ln::String name)
{
    ln::Ref<ln::Object> obj = ln::TypeInfo::createInstance(name);
    if (!obj) {
        CLI::error(u"Invalid type name.");
        return 1;
    }

    if (filePath.isEmpty()) {
        filePath = name;
    }

    auto model = ln::makeObject<ln::AssetModel>(obj);
    ln::detail::EngineDomain::assetManager()->saveAssetModelToLocalFile(model, filePath);

    return 0;
}
