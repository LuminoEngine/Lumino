#pragma once
#include "Common.hpp"

namespace ln {
namespace kokage {

class UnifiedShaderSerializer final {
public:
    static MaybeResult saveToFile(const UnifiedShader2* shader, const std::filesystem::path& filePath);
    static Result<Ref<UnifiedShader2>> loadFromFile(const std::filesystem::path& filePath);
};

} // namespace kokage
} // namespace ln
