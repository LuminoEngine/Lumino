// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <LuminoShader/UnifiedShader2.hpp>

namespace ln {
namespace shader {

UnifiedShader2::UnifiedShader2() {
}

const std::vector<ParameterBlockLayout2>& UnifiedShader2::parameterBlocks() const {
    return m_parameterBlocks;
}

const std::vector<std::unique_ptr<GlobalShaderPass2>>& UnifiedShader2::globalShaderPasses() const {
    return m_globalShaderPasses;
}

const std::vector<std::unique_ptr<TargetShaderPass2>>& UnifiedShader2::targetShaderPasses() const {
    return m_targetShaderPasses;
}

const std::vector<std::unique_ptr<TargetEntryPoint2>>& UnifiedShader2::targetEntryPoints() const {
    return m_targetEntryPoints;
}

TargetShaderPass2* UnifiedShader2::targetShaderPass(TargetShaderPassId2 id) const {
    return m_targetShaderPasses[id].get();
}

TargetEntryPoint2* UnifiedShader2::targetEntryPoint(TargetEntryPointId2 id) const {
    return m_targetEntryPoints[id].get();
}

Blob* UnifiedShader2::blob(BlobId2 id) const {
    return m_blobs[id].get();
}

ParameterBlockLayout2* UnifiedShader2::addParameterBlock(ParameterBlockLayout2&& layout) {
    m_parameterBlocks.push_back(std::move(layout));
    return &m_parameterBlocks.back();
}

GlobalShaderPass2* UnifiedShader2::createGlobalShaderPass() {
    auto pass = std::make_unique<GlobalShaderPass2>(this);
    pass->id = static_cast<GlobalShaderPassId2>(m_globalShaderPasses.size());
    m_globalShaderPasses.push_back(std::move(pass));
    return m_globalShaderPasses.back().get();
}

TargetShaderPass2* UnifiedShader2::createTargetShaderPass() {
    auto pass = std::make_unique<TargetShaderPass2>();
    pass->id = static_cast<TargetShaderPassId2>(m_targetShaderPasses.size());
    m_targetShaderPasses.push_back(std::move(pass));
    return m_targetShaderPasses.back().get();
}

TargetEntryPoint2* UnifiedShader2::createEntryPoint() {
    auto entryPoint = std::make_unique<TargetEntryPoint2>();
    entryPoint->id = static_cast<TargetEntryPointId2>(m_targetEntryPoints.size());
    m_targetEntryPoints.push_back(std::move(entryPoint));
    return m_targetEntryPoints.back().get();
}

Blob* UnifiedShader2::createBlob() {
    auto b = std::make_unique<Blob>();
    b->id = static_cast<BlobId>(m_blobs.size());
    m_blobs.push_back(std::move(b));
    return m_blobs.back().get();
}

Result<TargetEntryPoint2*> UnifiedShader2::getTargetEntryPoint(
    ShaderTarget target, const std::string& name) const {
    auto itr = std::find_if(
        m_targetEntryPoints.begin(),
        m_targetEntryPoints.end(),
        [&target, &name](const std::unique_ptr<TargetEntryPoint2>& entryPoint) {
            return entryPoint->target == target && entryPoint->name == name;
        });
    if (itr == m_targetEntryPoints.end()) {
        return LNSHADER_MAKE_ERROR("TargetEntryPoint2 not found. (" + name + ")");
    }
    return (*itr).get();
}

VoidResult UnifiedShader2::mergeTargetBindingLayout(
    TargetBindingLayout2& target, const TargetBindingLayout2& other, bool reset) {
    if (reset) {
        target.bindings.clear();
        target.combinedSamplers.clear();
    }
    for (const TargetBinding2& otherBinding : other.bindings) {
        auto itr = std::find_if(
            target.bindings.begin(),
            target.bindings.end(),
            [&otherBinding](const TargetBinding2& b) {
                return b.name == otherBinding.name &&
                       b.kind == otherBinding.kind &&
                       b.setIndex == otherBinding.setIndex;
            });
        if (itr != target.bindings.end()) {
            TargetBinding2& existing = *itr;

            if (otherBinding.bindingIndex != existing.bindingIndex) {
                return LNSHADER_MAKE_ERROR(
                    "TargetBinding2 already exists with different bindingIndex (" + otherBinding.name + ")");
            }
            if (otherBinding.size != existing.size) {
                return LNSHADER_MAKE_ERROR(
                    "TargetBinding2 already exists with different size (" + otherBinding.name + ")");
            }

            existing.used = static_cast<ShaderStageFlags>(existing.used | otherBinding.used);
        }
        else {
            target.bindings.push_back(otherBinding);
        }
    }

    // combined sampler は同じ (テクスチャ, サンプラー) の組が頂点とフラグメントの両方に
    // 現れるため、名前で重複を除きながら結合する。GL のテクスチャユニットはプログラム単位で
    // 共有されるので、ここで確定した添字がそのままユニット番号になる。
    for (const CombinedSamplerBinding2& otherCombined : other.combinedSamplers) {
        auto itr = std::find_if(
            target.combinedSamplers.begin(),
            target.combinedSamplers.end(),
            [&otherCombined](const CombinedSamplerBinding2& c) { return c.name == otherCombined.name; });
        if (itr == target.combinedSamplers.end()) {
            target.combinedSamplers.push_back(otherCombined);
        }
    }

    return LNSHADER_OK();
}

} // namespace shader
} // namespace ln
