// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include <lumino_shader/Common.hpp>

namespace ln {
namespace shader {

/**
 * シェーダファイルに含まれるメタデータを解析します。
 * 現在は `#ifdef LUMINO_METADATA` と `#endif` の間に JSON 形式で記述されたパス情報を解析するために使用されます。
 */
class ShaderMetadata final {
public:
    struct ShaderPass {
        std::string name;
        std::string vertexEntryPoint;
        std::string fragmentEntryPoint;
        std::string computeEntryPoint;
    };

    ShaderMetadata();
    VoidResult parse(const std::string& code);

    const std::vector<ShaderPass>& passes() const { return m_passes; }

private:
    std::vector<ShaderPass> m_passes;
};

} // namespace shader
} // namespace ln
