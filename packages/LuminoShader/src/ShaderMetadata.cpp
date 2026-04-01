// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <nlohmann/json.hpp>
#include "ShaderMetadata.hpp"

using json = nlohmann::json;

namespace ln {
namespace shader {

ShaderMetadata::ShaderMetadata() {
}

VoidResult ShaderMetadata::parse(const std::string& code) {
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
        return LNSHADER_MAKE_ERROR("#ifdef LUMINO_METADATA not found.");
    }

    // Find `#endif`
    size_t end = code.find("#endif", metadataPos);
    if (end == std::string::npos) {
        return LNSHADER_MAKE_ERROR("Missing #endif after LUMINO_METADATA.");
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
            shaderPass.computeEntryPoint = pass.value("compute", "");
            m_passes.push_back(shaderPass);
        }
    }
    catch (const std::exception& e) {
        return LNSHADER_MAKE_ERROR(e.what());
    }
    return LNSHADER_OK();
}

} // namespace shader
} // namespace ln
