﻿
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
    auto filePath = ln::Path::combine(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), u"Lumino", u"EditorData.json");
    if (ln::FileSystem::existsFile(filePath)) {
        auto text = ln::FileSystem::readAllText(filePath);
        ln::JsonSerializer::deserialize(text, *this);
    }
}

void AppData::save()
{
    auto dirPath = ln::Path(ln::Environment::specialFolderPath(ln::SpecialFolder::ApplicationData), u"Lumino");
    auto filePath = ln::Path(dirPath, u"EditorData.json");
    ln::FileSystem::createDirectory(dirPath);

    auto text = ln::JsonSerializer::serialize(*this);
    ln::FileSystem::writeAllText(filePath, text);
}

void AppData::serialize(ln::Archive& ar)
{
    ar & ln::makeNVP(u"RecentProjectFiles", recentProjectFiles);
}

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
