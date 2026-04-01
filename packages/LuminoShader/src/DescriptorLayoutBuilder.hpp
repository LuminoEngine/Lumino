// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include <LuminoShader/Common.hpp>

namespace ln {
namespace shader {

class UnifiedShader;
class GlobalShaderPass;
struct TargetBindingInfo;

// Build Sequence の終盤の処理。
// GlobalShaderPass が持つ GlobalLayout (カテゴライズ済み) と、
// TargetShaderPass が持つ BindingLayout (シーケンシャル) から、
// その対応表 (GlobalShaderPass::descriptorLayout) を作成する。
class DescriptorLayoutBuilder final {
public:
    DescriptorLayoutBuilder();
    VoidResult buildDescriptorLayout(UnifiedShader* shader);

private:
    VoidResult getOrCreateDescriptorLayoutEntry(
        UnifiedShader* shader,
        GlobalShaderPass* globalShaderPass,
        const TargetBindingInfo& bindingInfo,
        RegisterCategory* outCategory,
        int16_t* outIndex);
};

} // namespace shader
} // namespace ln
