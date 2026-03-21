// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include <lumino_shader/Common.hpp>

namespace ln {
namespace shader {

class ShaderMetadataParser final {
public:
    struct ShaderPass {
        std::string name;
        std::string vertexEntryPoint;
        std::string fragmentEntryPoint;
        std::string computeEntryPoint;
    };

    ShaderMetadataParser();
    VoidResult parse(const std::string& code);

    const std::vector<ShaderPass>& passes() const { return m_passes; }

private:
    std::vector<ShaderPass> m_passes;
};

} // namespace shader
} // namespace ln
