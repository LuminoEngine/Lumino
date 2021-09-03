
#include "Internal.hpp"
//#include <filesystem>
#include <toml.hpp>
#include "EngineManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// EngineSettings

bool EngineSettings::tryLoad()
{
    if (FileSystem::existsFile(u"lumino.ini")) {
        const auto data = toml::parse("lumino.ini");
        const auto asset_directory = toml::find<std::string>(data, "assetDirectory");
        assetDirectories.add(String::fromStdString(asset_directory));

        const auto priority_gpu_name = toml::find<std::string>(data, "priorityGpuName");
        priorityGPUName = String::fromStdString(priority_gpu_name);

        LN_LOG_INFO << "lumino.ini Loaded.";

        return true;
    }
    else {
        return false;
    }
}

} // namespace detail
} // namespace ln

