#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShaderSerializer.hpp>

namespace ln {
namespace kokage {

MaybeResult UnifiedShaderSerializer::saveToFile(const UnifiedShader2* shader, const std::filesystem::path& filePath) {
    auto stream = FileStream::create(Path::fromStdPath(filePath), FileOpenMode::Write | FileOpenMode::Truncate);
    auto writer = makeRef<BinaryWriter>(stream);

    return LN_MAKE_SUCCESS();
}

Result<Ref<UnifiedShader2>> UnifiedShaderSerializer::loadFromFile(const std::filesystem::path& filePath) {
    auto stream = FileStream::create(Path::fromStdPath(filePath), FileOpenMode::Read);
    auto reader = makeRef<BinaryReader>(stream);
    Ref<UnifiedShader2> shader = makeRef<UnifiedShader2>();


    return shader;
}

} // namespace kokage
} // namespace ln
