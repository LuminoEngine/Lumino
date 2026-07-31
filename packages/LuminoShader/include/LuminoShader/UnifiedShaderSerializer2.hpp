// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"
#include <filesystem>

namespace ln {
namespace shader {

class UnifiedShader2;

class UnifiedShaderSerializer2 final {
public:
    enum FileVersion {
        FileVersion_1 = 1,
        FileVersion_2 = 2,   // Added GlobalMemberInfo to ParameterBlockLayout2
        FileVersion_3 = 3,   // Added UnifiedShader2::sourceName
        FileVersion_Last,
        FileVersion_Current = FileVersion_Last - 1,
    };
    static constexpr const char* FileExt = "lcsh";

    static VoidResult saveToFile(const UnifiedShader2* shader, const std::filesystem::path& filePath);
    static Result<Ref<UnifiedShader2>> loadFromData(const void* data, size_t length);
};

} // namespace shader
} // namespace ln
