#pragma once

namespace ln {
namespace kokage {

class ShaderMetadataParser final {
public:
    struct ShaderPass {
        std::string name;
        std::string vertexEntryPoint;
        std::string fragmentEntryPoint;
        std::string computeEntryPoint;
    };

    ShaderMetadataParser();
    MaybeResult parse(const std::string& code);

    const std::vector<ShaderPass>& passes() const { return m_passes; }

private:
    std::vector<ShaderPass> m_passes;
};

} // namespace kokage
} // namespace ln

