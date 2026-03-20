#pragma once

namespace ln {
namespace kokage {

// Build Sequence の終盤の処理。
// GlobalShaderPass が持つ GlobalLayout (カテゴライズ済み) と、
// TargetShaderPass が持つ BindingLayout (シーケンシャル) から、
// その対応表 (GlobalShaderPass::descriptorLayout) を作成する。
class DescriptorLayoutBuilder final {
public:
    DescriptorLayoutBuilder();
    MaybeResult buildDescriptorLayout(UnifiedShader2* shader);

private:
    MaybeResult getOrCreateDescriptorLayoutEntry(
        UnifiedShader2* shader,
        GlobalShaderPass* globalShaderPass,
        const TargetBindingInfo& bindingInfo,
        kokage::RegisterCategory* outCategory,
        int16_t* outIndex);
};

} // namespace kokage
} // namespace ln
