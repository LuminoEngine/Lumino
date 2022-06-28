#pragma once

namespace lna {

class AppData
    : public ln::Object
{
public:
    ln::List<ln::Path> recentProjectFiles;

    void addRecentProjectFile(const ln::Path& filePath);

public:
    static AppData* current();
	void load();
    void save();

private:
    AppData();

    //LN_SERIALIZE_CLASS_VERSION(1)
    //void serialize(ln::Archive& ar);
};

} // namespace lna
