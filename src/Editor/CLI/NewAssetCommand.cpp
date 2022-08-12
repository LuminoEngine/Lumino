
#include <Project/Project.hpp>
#include "NewAssetCommand.hpp"
#include "../../../lumino/LuminoEngine/src/Engine/EngineDomain.hpp"
#include <LuminoEngine/Asset/detail/AssetManager.hpp>

int NewAssetCommand::execute(lna::Workspace* workspace, ln::String name)
{
    ln::Ref<ln::Object> obj = ln::TypeInfo::createInstance(name);
    if (!obj) {
        CLI::error(_TT("Invalid type name."));
        return 1;
    }

    if (filePath.isEmpty()) {
        filePath = name;
    }

    auto model = ln::makeObject_deprecated<ln::AssetModel>(obj);
    ln::detail::AssetManager::instance()->saveAssetModelToLocalFile(model, filePath);

    return 0;
}
