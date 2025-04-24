#pragma once
#include "Common.hpp"

namespace ln {
namespace kokage {

class UnifiedShaderSerializer final {
public:
    enum FileVersion {
        FileVersion_1 = 1,
        FileVersion_Last,
        FileVersion_Current = FileVersion_Last - 1,
    };
    static constexpr std::string_view FileExt = "lcsh"; // Lumino Compiled Shader

    static MaybeResult saveToFile(const UnifiedShader2* shader, const std::filesystem::path& filePath);
    static Result<Ref<UnifiedShader2>> loadFromData(const void* data, int32_t length);

private:
    static MaybeResult checkSignature(BinaryReader* r, const char* sig, size_t len);
};

} // namespace kokage
} // namespace ln
