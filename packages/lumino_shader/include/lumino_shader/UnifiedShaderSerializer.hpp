// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"
#include <filesystem>

namespace lumino {
namespace shader {

class UnifiedShader;

class UnifiedShaderSerializer final {
public:
    enum FileVersion {
        FileVersion_1 = 1,
        FileVersion_Last,
        FileVersion_Current = FileVersion_Last - 1,
    };
    static constexpr const char* FileExt = "lcsh";

    static VoidResult saveToFile(const UnifiedShader* shader, const std::filesystem::path& filePath);
    static Result<Ref<UnifiedShader>> loadFromData(const void* data, size_t length);
};

} // namespace shader
} // namespace lumino
