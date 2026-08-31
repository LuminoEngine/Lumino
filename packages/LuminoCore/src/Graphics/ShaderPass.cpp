#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include "ShaderUtils.hpp"
#include <algorithm>

namespace ln {

int32_t ShaderPass::s_liveCount = 0;

ShaderPass::ShaderPass() {
    ++s_liveCount;
}

ShaderPass::~ShaderPass() {
    --s_liveCount;
}

static rhi::ShaderCodeFormat backendToCodeFormat(rhi::Backend backend) {
    switch (backend) {
        case rhi::Backend::Vulkan: return rhi::ShaderCodeFormat::SPIRV;
        case rhi::Backend::WebGPU: return rhi::ShaderCodeFormat::WGSL;
        case rhi::Backend::WebGL2: return rhi::ShaderCodeFormat::GLSL;
    }
    return rhi::ShaderCodeFormat::SPIRV;
}

//------------------------------------------------------------------------------
// Sampler バインディングと、それが担当するテクスチャバインディングを対応付ける。
//
// Lumino のシェーダ規約ではテクスチャとサンプラーをペアで宣言するが、命名は 2 通り
// 存在する。
//   (a) <テクスチャ名> + "Sampler"   例: u_gbufferA   / u_gbufferASampler
//   (b) 共通接頭辞 + 役割サフィックス 例: u_baseTexture / u_baseSampler
// そのため、次の順で対応先を探す。
//   1. "Sampler" を除いた名前と完全一致するテクスチャ  -> (a)
//   2. "Sampler" を除いた名前を接頭辞に持つテクスチャが 1 つだけある -> (b)
//   3. 宣言順で直前にあるテクスチャ (規約から外れた命名へのフォールバック)
// いずれも見つからない場合は空文字列とし、マテリアル単位のサンプラー設定だけが効く。
static std::vector<std::string> resolveSamplerTextureNames(
    const rhi::BindGroupLayoutDesc& layoutDesc,
    const std::vector<std::string>& bindingNames) {

    static const std::string kSamplerSuffix = "Sampler";
    std::vector<std::string> result(layoutDesc.entries.size());

    for (size_t i = 0; i < layoutDesc.entries.size(); ++i) {
        if (layoutDesc.entries[i].type != rhi::BindingType::Sampler) continue;
        if (i >= bindingNames.size()) continue;
        const std::string& samplerName = bindingNames[i];

        if (samplerName.size() > kSamplerSuffix.size() &&
            samplerName.compare(samplerName.size() - kSamplerSuffix.size(),
                                kSamplerSuffix.size(), kSamplerSuffix) == 0) {
            std::string base = samplerName.substr(0, samplerName.size() - kSamplerSuffix.size());

            // 1. 完全一致
            bool matched = false;
            for (size_t j = 0; j < layoutDesc.entries.size() && j < bindingNames.size(); ++j) {
                if (layoutDesc.entries[j].type != rhi::BindingType::SampledTexture) continue;
                if (bindingNames[j] == base) {
                    result[i] = base;
                    matched = true;
                    break;
                }
            }
            if (matched) continue;

            // 2. 接頭辞一致 (候補が 1 つに絞れる場合のみ)
            const std::string* prefixMatch = nullptr;
            bool ambiguous = false;
            for (size_t j = 0; j < layoutDesc.entries.size() && j < bindingNames.size(); ++j) {
                if (layoutDesc.entries[j].type != rhi::BindingType::SampledTexture) continue;
                if (bindingNames[j].compare(0, base.size(), base) != 0) continue;
                if (prefixMatch) { ambiguous = true; break; }
                prefixMatch = &bindingNames[j];
            }
            if (prefixMatch && !ambiguous) {
                result[i] = *prefixMatch;
                continue;
            }
        }

        // 3. 直前のテクスチャへフォールバック
        for (size_t j = i; j > 0; --j) {
            if (layoutDesc.entries[j - 1].type != rhi::BindingType::SampledTexture) continue;
            if (j - 1 < bindingNames.size()) result[i] = bindingNames[j - 1];
            break;
        }
    }

    return result;
}

//------------------------------------------------------------------------------
// 内部用: createFromCompiledShader と createFromUnifiedShader が共有する実装
Result<Ref<ShaderPass>> ShaderPass::buildFromUnifiedShader(
    shader::UnifiedShader2* unifiedShader,
    rhi::Device* device,
    size_t passIndex,
    const std::string& shaderName) {

    // シェーダの識別名。明示指定が無ければ .lcsh に記録されたソースファイル名を使う。
    // 実行時のエラーメッセージと GPU オブジェクトのラベルに使うため、
    // 「どのシェーダで失敗したのか」がここから先すべてに伝播する。
    std::string resolvedName = shaderName.empty() ? unifiedShader->sourceName() : shaderName;
    if (resolvedName.empty()) {
        resolvedName = "(unnamed shader)";
    }

    // バックエンドのシェーダターゲットに対応する、要求されたパスを選択する。
    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return LN_MAKE_ERROR("No shader passes found (shader: %s)", resolvedName.c_str());
    }
    if (passIndex >= globalPasses.size()) {
        return LN_MAKE_ERROR(
            "Shader pass index out of range (shader: %s, index: %d, passCount: %d)",
            resolvedName.c_str(), static_cast<int>(passIndex), static_cast<int>(globalPasses.size()));
    }

    auto shaderTarget = detail::backendToShaderTarget(device->backend());
    auto codeFormat = backendToCodeFormat(device->backend());

    auto* globalPass = globalPasses[passIndex].get();
    auto targetPassId = globalPass->getTargetShaderPassId(shaderTarget);
    auto* targetPass = unifiedShader->targetShaderPass(targetPassId);
    if (!targetPass) {
        return LN_MAKE_ERROR(
            "No target pass found for the current backend (shader: %s, pass: %s)",
            resolvedName.c_str(), globalPass->name.c_str());
    }

    auto* vertEP = unifiedShader->targetEntryPoint(targetPass->vertEntryPointId);
    auto* fragEP = unifiedShader->targetEntryPoint(targetPass->fragEntryPointId);
    if (!vertEP || !fragEP) {
        return LN_MAKE_ERROR(
            "Missing entry points (shader: %s, pass: %s)",
            resolvedName.c_str(), globalPass->name.c_str());
    }

    auto* vertBlob = unifiedShader->blob(vertEP->codeBlobId);
    auto* fragBlob = unifiedShader->blob(fragEP->codeBlobId);

    // シェーダモジュールを作成する
    // debugName は WebGPU オブジェクトのラベルになる。ブラウザ / Dawn は不正なシェーダを
    // `[Invalid ShaderModule "<label>"]` の形で報告するため、ここに名前を載せておくと
    // 実行時ログから「どのシェーダのどのエントリポイントか」が分かる。
    rhi::ShaderModuleDesc vsDesc;
    vsDesc.format = codeFormat;
    vsDesc.code = vertBlob->data.data();
    vsDesc.codeSizeBytes = vertBlob->data.size();
    vsDesc.debugName = resolvedName + ":" + globalPass->name + ":" + vertEP->name;
    auto vsResult = device->createShaderModule(vsDesc);
    if (!vsResult) {
        return LN_MAKE_ERROR(
            "Failed to create vertex shader module. (shader: %s, pass: %s, entryPoint: %s) %s",
            resolvedName.c_str(), globalPass->name.c_str(), vertEP->name.c_str(),
            vsResult.error().message.c_str());
    }

    rhi::ShaderModuleDesc fsDesc;
    fsDesc.format = codeFormat;
    fsDesc.code = fragBlob->data.data();
    fsDesc.codeSizeBytes = fragBlob->data.size();
    fsDesc.debugName = resolvedName + ":" + globalPass->name + ":" + fragEP->name;
    auto fsResult = device->createShaderModule(fsDesc);
    if (!fsResult) {
        return LN_MAKE_ERROR(
            "Failed to create fragment shader module. (shader: %s, pass: %s, entryPoint: %s) %s",
            resolvedName.c_str(), globalPass->name.c_str(), fragEP->name.c_str(),
            fsResult.error().message.c_str());
    }

    // 任意の "$Material" ParameterBlock からマテリアルの BindGroupLayout を構築する。
    //
    // $Material は「素の uniform / リソース」をまとめた合成ブロック。
    // フルスクリーン blit のような最小シェーダではマテリアル定数バッファを持たない
    // (テクスチャ+サンプラだけ、あるいは素のパラメータが何も無い) ことがある。
    // そのため $Material ブロックの有無も、その中の定数バッファの有無もどちらも任意とし、
    // 無い場合は materialSetIdx = -1 / cbSize = 0 として扱う。
    rhi::BindGroupLayoutDesc materialLayoutDesc;
    int16_t materialSetIdx = -1;
    int16_t cbSize = 0;

    auto* materialBlock = detail::findParameterBlock(unifiedShader, "$Material");
    if (materialBlock) {
        materialLayoutDesc = detail::buildBindGroupLayoutFromReflection(*materialBlock, targetPass->bindingLayout);
        materialSetIdx = materialBlock->setIndex;

        // 定数バッファは任意。見つからない場合は 0 (= マテリアル UBO 無し) とする。
        int16_t foundCbSize = detail::findConstantBufferSize(*materialBlock);
        if (foundCbSize > 0) {
            cbSize = foundCbSize;
        }
    }

    // リフレクションから view/scene/object ブロックを見つける
    auto* viewBlock = detail::findParameterBlock(unifiedShader, "viewData");
    auto* sceneBlock = detail::findParameterBlock(unifiedShader, "sceneData");
    auto* objectBlock = detail::findParameterBlock(unifiedShader, "objectData");
    if (!viewBlock || !sceneBlock || !objectBlock) {
        return LN_MAKE_ERROR(
            "Missing required ParameterBlocks (viewData, sceneData, objectData) (shader: %s, pass: %s)",
            resolvedName.c_str(), globalPass->name.c_str());
    }

    int16_t viewSetIndex = viewBlock->setIndex;
    int16_t sceneSetIndex = sceneBlock->setIndex;
    int16_t objectSetIndex = objectBlock->setIndex;

    // リフレクションから BindGroupLayoutDesc を構築する (UBO には動的オフセットを付ける)
    auto viewLayoutDesc = detail::buildBindGroupLayoutFromReflection(*viewBlock, targetPass->bindingLayout);
    for (auto& entry : viewLayoutDesc.entries) {
        if (entry.type == rhi::BindingType::UniformBuffer) entry.hasDynamicOffset = true;
    }

    auto sceneLayoutDesc = detail::buildBindGroupLayoutFromReflection(*sceneBlock, targetPass->bindingLayout);
    for (auto& entry : sceneLayoutDesc.entries) {
        if (entry.type == rhi::BindingType::UniformBuffer) entry.hasDynamicOffset = true;
    }

    auto objectLayoutDesc = detail::buildBindGroupLayoutFromReflection(*objectBlock, targetPass->bindingLayout);
    for (auto& entry : objectLayoutDesc.entries) {
        if (entry.type == rhi::BindingType::UniformBuffer) entry.hasDynamicOffset = true;
    }

    int16_t objectCBSize = detail::findConstantBufferSize(*objectBlock);
    if (objectCBSize <= 0) {
        return LN_MAKE_ERROR(
            "Invalid object constant buffer size in reflection (shader: %s, pass: %s)",
            resolvedName.c_str(), globalPass->name.c_str());
    }

    // マテリアルセットのバインディング名を集める (materialLayoutDesc の entries と並行)
    std::vector<std::string> materialBindingNames;
    for (const auto& binding : targetPass->bindingLayout.bindings) {
        if (binding.setIndex == materialSetIdx) {
            materialBindingNames.push_back(binding.name);
        }
    }

    // PipelineLayoutDesc のサイズを正しく決めるため、最大のセットインデックスを求める
    int16_t maxSet = std::max({materialSetIdx, viewSetIndex, sceneSetIndex, objectSetIndex});

    // リフレクションから得たセットインデックスで PipelineLayoutDesc を組み立てる
    rhi::PipelineLayoutDesc plDesc;
    plDesc.setLayouts.resize(static_cast<size_t>(maxSet) + 1);
    if (materialSetIdx >= 0) {
        plDesc.setLayouts[materialSetIdx] = materialLayoutDesc;
    }
    plDesc.setLayouts[viewSetIndex] = viewLayoutDesc;
    plDesc.setLayouts[sceneSetIndex] = sceneLayoutDesc;
    plDesc.setLayouts[objectSetIndex] = objectLayoutDesc;

    // GLSL ES 300 のバックエンド向けの combined sampler の対応表。
    // 他のターゲットの .lcsh では空なので、そのまま空で渡る。
    plDesc.combinedSamplers.reserve(targetPass->bindingLayout.combinedSamplers.size());
    for (const auto& c : targetPass->bindingLayout.combinedSamplers) {
        plDesc.combinedSamplers.push_back({
            c.name,
            static_cast<uint32_t>(c.textureSetIndex),
            static_cast<uint32_t>(c.textureBindingIndex),
            static_cast<uint32_t>(c.samplerSetIndex),
            static_cast<uint32_t>(c.samplerBindingIndex)});
    }

    // GlobalMemberInfo を MaterialMemberInfo へ変換する
    std::vector<MaterialMemberInfo> members;
    if (materialBlock) {
        members.reserve(materialBlock->members.size());
        for (const auto& gm : materialBlock->members) {
            members.push_back({gm.name, gm.offset, gm.size});
        }
    }

    // インスタンスを作成する
    auto plResult = device->createPipelineLayout(plDesc);
    if (!plResult) {
        return LN_MAKE_ERROR(
            "Failed to create pipeline layout. (shader: %s, pass: %s) %s",
            resolvedName.c_str(), globalPass->name.c_str(), plResult.error().message.c_str());
    }

    auto sp = Ref<ShaderPass>::adopt(new ShaderPass());
    sp->m_shaderName = resolvedName;
    sp->m_passName = globalPass->name;
    sp->m_vertShader = std::move(*vsResult);
    sp->m_fragShader = std::move(*fsResult);
    // NOTE: エントリポイント名はコピーする。同一の UnifiedShader2 から複数のパスを
    // 構築する場合 (Shader が全パスを一度に構築する経路) に move してしまうと、
    // 2 つ目以降のパスがエントリポイント名を失うため。
    sp->m_vertEntry = vertEP->name;
    sp->m_fragEntry = fragEP->name;
    sp->m_pipelineLayout = std::move(*plResult);
    sp->m_materialParamBufferSize = static_cast<uint64_t>(cbSize);
    sp->m_materialSetIndex = materialSetIdx;
    sp->m_materialMembers = std::move(members);
    sp->m_materialLayoutDesc = materialLayoutDesc;
    sp->m_materialSamplerTextureNames =
        resolveSamplerTextureNames(materialLayoutDesc, materialBindingNames);
    sp->m_materialBindingNames = std::move(materialBindingNames);
    sp->m_viewSetIndex = viewSetIndex;
    sp->m_sceneSetIndex = sceneSetIndex;
    sp->m_objectSetIndex = objectSetIndex;
    sp->m_viewLayoutDesc = std::move(viewLayoutDesc);
    sp->m_sceneLayoutDesc = std::move(sceneLayoutDesc);
    sp->m_objectLayoutDesc = std::move(objectLayoutDesc);
    sp->m_objectUBOSize = static_cast<uint64_t>(objectCBSize);

    return sp;
}

//------------------------------------------------------------------------------
Result<Ref<ShaderPass>> ShaderPass::createFromCompiledShader(
    const void* data, size_t size,
    rhi::Device* device,
    size_t passIndex,
    const std::string& shaderName) {

    // バイナリ blob から UnifiedShader をデシリアライズする。
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(
        data, size, detail::backendToShaderTarget(device->backend()));
    if (!loadResult) {
        return LN_FORWARD_ERROR(loadResult);
    }
    auto unifiedShader = std::move(*loadResult);

    return buildFromUnifiedShader(unifiedShader.get(), device, passIndex, shaderName);
}

#ifdef LUMINO_USE_SLANG
//------------------------------------------------------------------------------
Result<Ref<ShaderPass>> ShaderPass::createFromUnifiedShader(
    shader::UnifiedShader2* unifiedShader,
    rhi::Device* device,
    size_t passIndex,
    const std::string& shaderName) {
    return buildFromUnifiedShader(unifiedShader, device, passIndex, shaderName);
}
#endif // LUMINO_USE_SLANG

} // namespace ln
