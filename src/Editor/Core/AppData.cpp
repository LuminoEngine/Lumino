
#include "AppData.hpp"

namespace lna {

AppData* AppData::current()
{
    static AppData inst;
    return &inst;
}

AppData::AppData()
{
}

void AppData::load()
{
    auto filePath = ln::Path::combine(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), _TT("Lumino"), _TT("EditorData.json"));
    if (ln::FileSystem::existsFile(filePath)) {
        LN_NOTIMPLEMENTED();
        //auto text = ln::FileSystem::readAllText(filePath);
        //ln::JsonSerializer::deserialize(text.unwrap(), *this);
    }
}

void AppData::save()
{
    auto dirPath = ln::Path(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), _TT("Lumino"));
    auto filePath = ln::Path(dirPath, _TT("EditorData.json"));
    ln::FileSystem::createDirectory(dirPath);

    LN_NOTIMPLEMENTED();
    //auto text = ln::JsonSerializer::serialize(*this);
    //ln::FileSystem::writeAllText(filePath, text);
}

//void AppData::serialize(ln::Archive& ar)
//{
//    ar & ln::makeNVP(_TT("RecentProjectFiles"), recentProjectFiles);
//}

void AppData::addRecentProjectFile(const ln::Path& filePath)
{
    int index = recentProjectFiles.indexOfIf([&](auto& x) { return ln::Path::compare(x, filePath) == 0; });
    if (index >= 0) {
        recentProjectFiles.removeAt(index);
    }
    recentProjectFiles.insert(0, filePath);

    if (recentProjectFiles.size() > 4) {
        recentProjectFiles.removeLast();
    }
}

} // namespace lna
