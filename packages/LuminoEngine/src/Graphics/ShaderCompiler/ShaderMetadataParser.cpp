#include <nlohmann/json.hpp>
#include "Internal.hpp"
#include "ShaderMetadataParser.hpp"
using json = nlohmann::json;

namespace ln {
namespace kokage {

ShaderMetadataParser::ShaderMetadataParser() {

}

MaybeResult ShaderMetadataParser::parse(const std::string& code) {
    // Find `#ifdef LUMINO_METADATA`
    size_t start = code.find("#ifdef");
    size_t metadataPos = std::string::npos;
    while (start != std::string::npos) {
        metadataPos = code.find("LUMINO_METADATA", start);
        if (metadataPos != std::string::npos) {
            break;
        }
        start = code.find("#ifdef", start + 1);
    }
    if (metadataPos == std::string::npos) {
        return LN_MAKE_ERROR("#ifdef LUMINO_METADATA not found.");
    }

    // Find `#endif`
    size_t end = code.find("#endif", metadataPos);
    if (metadataPos == std::string::npos) {
        return LN_MAKE_ERROR("Missing #endif after LUMINO_METADATA.");
    }

    // Extract the code from `#ifdef LUMINO_METADATA` to `#endif`
    const size_t metadataLength = strlen("LUMINO_METADATA");
    std::string metadataCode = code.substr(
        metadataPos + metadataLength,
        end - metadataPos - metadataLength);

    // Parse the metadata JSON.
    try {
        json j = json::parse(metadataCode, nullptr, true, true);
        size_t count = j["passes"].size();
        for (size_t i = 0; i < count; i++) {
            json pass = j["passes"][i];
            ShaderPass shaderPass;
            shaderPass.name = pass.value("name", "Pass." + std::to_string(i));
            shaderPass.vertexEntryPoint = pass.value("vertex", "");
            shaderPass.fragmentEntryPoint = pass.value("fragment", "");
            m_passes.push_back(shaderPass);
        }
    }
    catch (const std::exception& e) {
        return LN_MAKE_ERROR(e.what());
    }
	return LN_MAKE_SUCCESS();
}

} // namespace kokage
} // namespace ln

