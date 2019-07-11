#pragma once

namespace lna {
class Project;


class AssetDatabase
	: public ln::Object
{
public:
    Ref<ln::AssetModel> openAsset(const ln::Path& filePath);

public:// TODO:
   // LN_INTERNAL_NEW_OBJECT;
    AssetDatabase();
    ln::Result init(Project* owner);

    Project* m_ownerProject;
};

} // namespace lna
