
#include "Internal.hpp"
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/GPU/SwapChain.hpp>
#include <LuminoGraphics/GPU/Shader.hpp>
#include <LuminoGraphics/GPU/ShaderDescriptor.hpp>
#include <LuminoGraphics/GPU/ShaderInterfaceFramework.hpp>

namespace ln {
namespace detail {

//==============================================================================
// ShaderTechniqueSemanticsManager
//		参考:
//		Unity Builtin Shader variables
//		https://docs.unity3d.com/Manual/SL-UnityShaderVariables.html

static const std::unordered_map<String, BuiltinShaderParameters> s_BuiltinShaderParametersMap =
{
    {_LT("ln_View"), BuiltinShaderParameters_ln_View},
    {_LT("ln_Projection"), BuiltinShaderParameters_ln_Projection},
    {_LT("ln_ProjectionI"), BuiltinShaderParameters_ln_ProjectionI},
    {_LT("_ln_MainLightMatrix"), BuiltinShaderParameters_ln_MainLightMatrix},
    {_LT("ln_Resolution"), BuiltinShaderParameters_ln_Resolution},
    {_LT("ln_CameraPosition_"), BuiltinShaderParameters_ln_CameraPosition},
    {_LT("ln_CameraDirection_"), BuiltinShaderParameters_ln_CameraDirection},
    {_LT("ln_AmbientColor"), BuiltinShaderParameters_ln_AmbientColor},
    {_LT("ln_AmbientSkyColor"), BuiltinShaderParameters_ln_AmbientSkyColor},
    {_LT("ln_AmbientGroundColor"), BuiltinShaderParameters_ln_AmbientGroundColor},
    {_LT("_ln_MainLightColor"), BuiltinShaderParameters_ln_MainLightColor},
    {_LT("_ln_MainLightPos"), BuiltinShaderParameters_ln_MainLightPos},
    {_LT("_ln_MainLightDir"), BuiltinShaderParameters_ln_MainLightDir},

    {_LT("ln_World"), BuiltinShaderParameters_ln_World},
    {_LT("ln_WorldI"), BuiltinShaderParameters_ln_WorldI},
    {_LT("ln_WorldViewProjection"), BuiltinShaderParameters_ln_WorldViewProjection},
    {_LT("ln_WorldView"), BuiltinShaderParameters_ln_WorldView},
    {_LT("ln_WorldViewIT"), BuiltinShaderParameters_ln_WorldViewIT},
    {_LT("ln_BoneTextureReciprocalSize"), BuiltinShaderParameters_ln_BoneTextureReciprocalSize},
    {_LT("ln_MorphWeights"), BuiltinShaderParameters_ln_MorphWeights},

    {_LT("ln_ColorScale"), BuiltinShaderParameters_ln_ColorScale},
    {_LT("ln_BlendColor"), BuiltinShaderParameters_ln_BlendColor},
    {_LT("ln_ToneColor"), BuiltinShaderParameters_ln_ToneColor},

    {_LT("ln_MaterialColor"), BuiltinShaderParameters_ln_MaterialColor},
    {_LT("ln_MaterialEmissive"), BuiltinShaderParameters_ln_MaterialEmissive},
    {_LT("ln_MaterialRoughness"), BuiltinShaderParameters_ln_MaterialRoughness},
    {_LT("ln_MaterialMetallic"), BuiltinShaderParameters_ln_MaterialMetallic},
};

static const std::unordered_map<String, BuiltinShaderUniformBuffers> s_BuiltinShaderUniformBuffersMap =
{
    {_LT("LNRenderViewBuffer"), BuiltinShaderUniformBuffers_LNRenderViewBuffer},
    {_LT("LNRenderElementBuffer"), BuiltinShaderUniformBuffers_LNRenderElementBuffer},
    {_LT("LNEffectColorBuffer"), BuiltinShaderUniformBuffers_LNEffectColorBuffer},
    {_LT("LNPBRMaterialParameter"), BuiltinShaderUniformBuffers_LNPBRMaterialParameter},
    {_LT("LNClusteredShadingParameters"), BuiltinShaderUniformBuffers_LNClusteredShadingParameters},
    {_LT("LNShadowParameters"), BuiltinShaderUniformBuffers_LNShadowParameters},
    {_LT("$Global"), BuiltinShaderUniformBuffers_Global},
};

static const std::unordered_map<String, BuiltinShaderTextures> s_BuiltinShaderTexturesMap =
{
    {_LT("ln_MaterialTexture"), BuiltinShaderTextures_ln_MaterialTexture},
    {_LT("ln_NormalMap"), BuiltinShaderTextures_ln_NormalMap},
    {_LT("ln_MetallicRoughnessTexture"), BuiltinShaderTextures_ln_MetallicRoughnessTexture},
    {_LT("ln_OcclusionTexture"), BuiltinShaderTextures_ln_OcclusionTexture},
    {_LT("ln_BoneTexture"), BuiltinShaderTextures_ln_BoneTexture},
    {_LT("ln_BoneLocalQuaternionTexture"), BuiltinShaderTextures_ln_BoneLocalQuaternionTexture},
    {_LT("ln_mainLightShadowMap"), BuiltinShaderTextures_ln_mainLightShadowMap},
    
    {_LT("ln_clustersTexture"), BuiltinShaderTextures_ln_ClustersTexture},
    {_LT("ln_GlobalLightInfoTexture"), BuiltinShaderTextures_ln_GlobalLightInfoTexture},
    {_LT("ln_pointLightInfoTexture"), BuiltinShaderTextures_ln_PointLightInfoTexture},
};

ShaderTechniqueSemanticsManager::ShaderTechniqueSemanticsManager()
    : m_descriptor(nullptr)
    , m_hasBuiltinShaderParameters(0)
    , m_builtinUniformBuffers({})
    , m_builtinShaderTextures({})
{
    //auto a = LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_NearClip);
    // メモリレイアウトそのまま ConstantBuffer に転送するため、オフセットを検証しておく
    static_assert(256 == offsetof(LNRenderViewBuffer, ln_Resolution), "Invalid offsetof(LNRenderViewBuffer, ln_Resolution)");
    static_assert(272 == offsetof(LNRenderViewBuffer, ln_CameraPosition), "Invalid offsetof(LNRenderViewBuffer, ln_CameraPosition)");
    static_assert(288 == offsetof(LNRenderViewBuffer, ln_CameraDirection), "Invalid offsetof(LNRenderViewBuffer, ln_CameraDirection)");
    static_assert(400 == sizeof(LNRenderViewBuffer), "Invalid sizeof(LNRenderViewBuffer)");
    static_assert(368 == sizeof(LNRenderElementBuffer), "Invalid sizeof(LNRenderViewBuffer)");
    static_assert(48 == sizeof(LNEffectColorBuffer), "Invalid sizeof(LNRenderViewBuffer)");
    static_assert(BuiltinShaderParameters__Count < 64, "Invalid BuiltinShaderParameters__Count");

    reset();
}

void ShaderTechniqueSemanticsManager::init(ShaderTechnique* technique)
{
    m_descriptor = technique->shader()->descriptor();
    const auto& globalLayout = technique->shader()->descriptorLayout();

    for (const auto& pass : technique->m_passes) {

        const auto& localLayout = pass->descriptorLayout();

        // UniformBuffers
        for (const auto& localInfo : localLayout.m_buffers) {
            const auto& globalInfo = globalLayout->m_buffers[localInfo.dataIndex];

            auto itr = s_BuiltinShaderUniformBuffersMap.find(globalInfo.name);
            if (itr != s_BuiltinShaderUniformBuffersMap.end()) {
                m_builtinUniformBuffers[itr->second] = localInfo.dataIndex;
            }
        }
        for (const auto& memberInfo : globalLayout->m_members) {
            auto itr = s_BuiltinShaderParametersMap.find(memberInfo.name);
            if (itr != s_BuiltinShaderParametersMap.end()) {
                m_hasBuiltinShaderParameters |= (1 << itr->second);
            }
        }

        // Textues
        for (const auto& localInfo : localLayout.m_textures) {
            const auto& globalInfo = globalLayout->m_textures[localInfo.dataIndex];

            auto itr = s_BuiltinShaderTexturesMap.find(globalInfo.name);
            if (itr != s_BuiltinShaderTexturesMap.end()) {
                m_builtinShaderTextures[itr->second] = localInfo.dataIndex;
            }
        }

        //// Samplers
        //for (const auto& localInfo : localLayout.m_samplers) {
        //    const auto& globalInfo = globalLayout->m_samplers[localInfo.dataIndex];

        //    auto itr = s_BuiltinShaderParametersMap.find(globalInfo.name);
        //    if (itr != s_BuiltinShaderParametersMap.end()) {
        //        m_hasBuiltinShaderParameters |= (1 << itr->second);
        //    }
        //}

    }

}

void ShaderTechniqueSemanticsManager::reset()
{
    //m_hasBuiltinShaderParameters = 0;
    for (auto& i : m_builtinUniformBuffers) i = -1;
    for (auto& i : m_builtinShaderTextures) i = -1;
}

void ShaderTechniqueSemanticsManager::updateRenderViewVariables(ShaderSecondaryDescriptor* descriptor, const RenderViewInfo& info, const SceneInfo& sceneInfo) const
{
    int index = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNRenderViewBuffer];
    if (index >= 0) {
        const CameraInfo& c = info.cameraInfo;

        LNRenderViewBuffer data;
        data.ln_View = c.viewMatrix;
        data.ln_Projection = c.projMatrix;
        if (hasParameter(BuiltinShaderParameters_ln_ProjectionI))
            data.ln_ProjectionI = Matrix::makeInverse(c.projMatrix);
        data.ln_Resolution = Vector4(c.viewPixelSize, 1.0f / c.viewPixelSize.width, 1.0f / c.viewPixelSize.height);
        data.ln_CameraPosition = Vector4(c.viewPosition, c.nearClip);
        data.ln_CameraDirection = Vector4(c.viewDirection, c.farClip);
        data.ln_AmbientColor = sceneInfo.ambientColor.toVector4();
        data.ln_AmbientSkyColor = sceneInfo.ambientSkyColor.toVector4();
        data.ln_AmbientGroundColor = sceneInfo.ambientGroundColor.toVector4();
        data._ln_MainLightColor = sceneInfo.mainLightColor.toVector4();
        data._ln_MainLightPos = sceneInfo.mainLightPos;
        data._ln_MainLightDir = sceneInfo.mainLightDir;
        descriptor->setUniformBufferData(index, &data, sizeof(data));
    }

    index = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNShadowParameters];
    if (index >= 0) {
        LNShadowParameters data;
        data.ln_mainLightShadowMapResolution = Vector4(info.mainLightShadowMapPixelSize, 1.0f / info.mainLightShadowMapPixelSize.width, 1.0f / info.mainLightShadowMapPixelSize.height);
        data.ln_shadowDensity = Vector4(info.mainLightShadowDensity, 0, 0, 0);
        descriptor->setUniformBufferData(index, &data, sizeof(data));
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_mainLightShadowMap];
    if (index >= 0) {
        LN_DCHECK(info.mainLightShadowMap);
        descriptor->setTexture(index, info.mainLightShadowMap);
    }
}

void ShaderTechniqueSemanticsManager::updateElementVariables(ShaderSecondaryDescriptor* descriptor, const CameraInfo& cameraInfo, const ElementInfo& info) const
{
    int index = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNRenderElementBuffer];
    if (index >= 0) {
        LNRenderElementBuffer data;
        if (hasParameter(BuiltinShaderParameters_ln_World))
            data.ln_World = info.WorldMatrix;
        if (hasParameter(BuiltinShaderParameters_ln_WorldI))
            data.ln_WorldI = Matrix::makeInverse(info.WorldMatrix);
        if (hasParameter(BuiltinShaderParameters_ln_WorldViewProjection))
            data.ln_WorldViewProjection = info.WorldViewProjectionMatrix;
        if (hasParameter(BuiltinShaderParameters_ln_WorldView))
            data.ln_WorldView = info.WorldMatrix * cameraInfo.viewMatrix;
        if (hasParameter(BuiltinShaderParameters_ln_WorldViewIT))
            data.ln_WorldViewIT = Matrix::makeTranspose(Matrix::makeInverse(info.WorldMatrix * cameraInfo.viewMatrix));
        if (hasParameter(BuiltinShaderParameters_ln_BoneTextureReciprocalSize) && info.boneTexture)
            data.ln_BoneTextureReciprocalSize = Vector4(1.0f / info.boneTexture->width(), 1.0f / info.boneTexture->height(), 0, 0);
        if (hasParameter(BuiltinShaderParameters_ln_MorphWeights))
            data.ln_MorphWeights.set(info.morphWeights[0], info.morphWeights[1], info.morphWeights[2], info.morphWeights[3]);

        data.ln_objectId = info.objectId;
        descriptor->setUniformBufferData(index, &data, sizeof(data));
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_BoneTexture];
    if (index >= 0) {
        descriptor->setTexture(index, info.boneTexture);
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_BoneLocalQuaternionTexture];
    if (index >= 0) {
        descriptor->setTexture(index, info.boneLocalQuaternionTexture);
    }
}

void ShaderTechniqueSemanticsManager::updateSubsetVariables(ShaderSecondaryDescriptor* descriptor, const SubsetInfo& info) const
{
    int index = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNEffectColorBuffer];
    if (index >= 0) {
        // 計算に時間がかかるものでもないため、個々のメンバの alive は確認しない
        LNEffectColorBuffer data = {
            info.colorScale.withAlpha(info.colorScale.a * info.opacity).toVector4(),
            info.blendColor.toVector4(),
            info.tone.toVector4(),
        };
        descriptor->setUniformBufferData(index, &data, sizeof(data));
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_MaterialTexture];
    if (index >= 0) {
        LN_DCHECK(info.materialTexture);
        descriptor->setTexture(index, info.materialTexture);
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_NormalMap];
    if (index >= 0) {
        LN_DCHECK(info.normalMap);
        descriptor->setTexture(index, info.normalMap);
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_MetallicRoughnessTexture];
    if (index >= 0) {
        LN_DCHECK(info.metallicRoughnessTexture);
        descriptor->setTexture(index, info.metallicRoughnessTexture);
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_OcclusionTexture];
    if (index >= 0) {
        LN_DCHECK(info.occlusionTexture);
        descriptor->setTexture(index, info.occlusionTexture);
    }
}

void ShaderTechniqueSemanticsManager::updateSubsetVariables_PBR(ShaderSecondaryDescriptor* descriptor, const PbrMaterialData& materialData) const
{
    int index = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNPBRMaterialParameter];
    if (index >= 0) {
        // 計算に時間がかかるものでもないため、個々のメンバの alive は確認しない
        LNPBRMaterialParameter data = {
            materialData.color.toVector4(),
            materialData.emissive.toVector4(),
            materialData.roughness,
            materialData.metallic,
        };
        descriptor->setUniformBufferData(index, &data, sizeof(data));
    }
}

void ShaderTechniqueSemanticsManager::updateClusteredShadingVariables(ShaderSecondaryDescriptor* descriptor, const ClusteredShadingRendererInfo& info) const
{
    int index = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNClusteredShadingParameters];
    if (index >= 0) {
        // 計算に時間がかかるものでもないため、個々のメンバの alive は確認しない
        LNClusteredShadingParameters data = {
            info.fogParams,
            info.fogColorAndDensity,
            info.mainLightDirection,
            info.nearClip,
            info.farClip,
        };
        descriptor->setUniformBufferData(index, &data, sizeof(data));
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_ClustersTexture];
    if (index >= 0) {
        descriptor->setTexture(index, info.lightClustersTexture);
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_GlobalLightInfoTexture];
    if (index >= 0) {
        descriptor->setTexture(index, info.globalLightInfoTexture);
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_PointLightInfoTexture];
    if (index >= 0) {
        descriptor->setTexture(index, info.localLightInfoTexture);
    }
}

//==============================================================================
// ShaderPassSemanticsManager

ShaderPassSemanticsManager::ShaderPassSemanticsManager()
    : m_hasBuiltinShaderParameters(0)
    , m_builtinUniformBuffers({})
    , m_builtinShaderTextures({}) {
    // auto a = LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_NearClip);
    //  メモリレイアウトそのまま ConstantBuffer に転送するため、オフセットを検証しておく
    static_assert(256 == offsetof(LNRenderViewBuffer, ln_Resolution), "Invalid offsetof(LNRenderViewBuffer, ln_Resolution)");
    static_assert(272 == offsetof(LNRenderViewBuffer, ln_CameraPosition), "Invalid offsetof(LNRenderViewBuffer, ln_CameraPosition)");
    static_assert(288 == offsetof(LNRenderViewBuffer, ln_CameraDirection), "Invalid offsetof(LNRenderViewBuffer, ln_CameraDirection)");
    static_assert(400 == sizeof(LNRenderViewBuffer), "Invalid sizeof(LNRenderViewBuffer)");
    static_assert(368 == sizeof(LNRenderElementBuffer), "Invalid sizeof(LNRenderViewBuffer)");
    static_assert(48 == sizeof(LNEffectColorBuffer), "Invalid sizeof(LNRenderViewBuffer)");
    static_assert(BuiltinShaderParameters__Count < 64, "Invalid BuiltinShaderParameters__Count");

    reset();
}

void ShaderPassSemanticsManager::init(const ShaderPass* shaderPass, const kokage::DescriptorLayout& layout) {

    // Buffers
    for (int iSlot = 0; iSlot < layout.bufferSlots().size(); iSlot++) {
        const auto& slot = layout.bufferSlots()[iSlot];
        auto itr = s_BuiltinShaderUniformBuffersMap.find(ln::String::fromUtf8(slot.name)); // TODO: 文字コード変換がオーバーヘッドになるかも
        if (itr != s_BuiltinShaderUniformBuffersMap.end()) {
            m_builtinUniformBuffers[itr->second] = iSlot;
        }

        for (const auto& member : slot.members) {
            auto itr = s_BuiltinShaderParametersMap.find(ln::String::fromUtf8(member.name));    // TODO: 文字コード変換がオーバーヘッドになるかも
            if (itr != s_BuiltinShaderParametersMap.end()) {
                m_hasBuiltinShaderParameters |= (1 << itr->second);
            }
        }
    }

    // Resources
    for (int iSlot = 0; iSlot < layout.resourceSlots().size(); iSlot++) {
        const auto& slot = layout.resourceSlots()[iSlot];
        auto itr = s_BuiltinShaderTexturesMap.find(ln::String::fromUtf8(slot.name)); // TODO: 文字コード変換がオーバーヘッドになるかも
        if (itr != s_BuiltinShaderTexturesMap.end()) {
            m_builtinShaderTextures[itr->second] = iSlot;
        }
    }
}

void ShaderPassSemanticsManager::reset() {
    // m_hasBuiltinShaderParameters = 0;
    for (auto& i : m_builtinUniformBuffers)
        i = -1;
    for (auto& i : m_builtinShaderTextures)
        i = -1;
}

void ShaderPassSemanticsManager::updateRenderViewVariables(ShaderDescriptor* descriptor, const RenderViewInfo& info, const SceneInfo& sceneInfo) const {
    int slotIndex = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNRenderViewBuffer];
    if (slotIndex >= 0) {
        const CameraInfo& c = info.cameraInfo;

        LNRenderViewBuffer data;
        data.ln_View = c.viewMatrix;
        data.ln_Projection = c.projMatrix;
        if (hasParameter(BuiltinShaderParameters_ln_ProjectionI))
            data.ln_ProjectionI = Matrix::makeInverse(c.projMatrix);
        data.ln_Resolution = Vector4(c.viewPixelSize, 1.0f / c.viewPixelSize.width, 1.0f / c.viewPixelSize.height);
        data.ln_CameraPosition = Vector4(c.viewPosition, c.nearClip);
        data.ln_CameraDirection = Vector4(c.viewDirection, c.farClip);
        data.ln_AmbientColor = sceneInfo.ambientColor.toVector4();
        data.ln_AmbientSkyColor = sceneInfo.ambientSkyColor.toVector4();
        data.ln_AmbientGroundColor = sceneInfo.ambientGroundColor.toVector4();
        data._ln_MainLightColor = sceneInfo.mainLightColor.toVector4();
        data._ln_MainLightPos = sceneInfo.mainLightPos;
        data._ln_MainLightDir = sceneInfo.mainLightDir;
        descriptor->setUniformBufferData(slotIndex, &data, sizeof(data));
    }

    slotIndex = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNShadowParameters];
    if (slotIndex >= 0) {
        LNShadowParameters data;
        data.ln_mainLightShadowMapResolution = Vector4(info.mainLightShadowMapPixelSize, 1.0f / info.mainLightShadowMapPixelSize.width, 1.0f / info.mainLightShadowMapPixelSize.height);
        data.ln_shadowDensity = Vector4(info.mainLightShadowDensity, 0, 0, 0);
        descriptor->setUniformBufferData(slotIndex, &data, sizeof(data));
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_mainLightShadowMap];
    if (slotIndex >= 0) {
        LN_DCHECK(info.mainLightShadowMap);
        descriptor->setTexture(slotIndex, info.mainLightShadowMap);
    }
}

void ShaderPassSemanticsManager::updateElementVariables(ShaderDescriptor* descriptor, const CameraInfo& cameraInfo, const ElementInfo& info) const {
    int slotIndex = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNRenderElementBuffer];
    if (slotIndex >= 0) {
        LNRenderElementBuffer data;
        if (hasParameter(BuiltinShaderParameters_ln_World))
            data.ln_World = info.WorldMatrix;
        if (hasParameter(BuiltinShaderParameters_ln_WorldI))
            data.ln_WorldI = Matrix::makeInverse(info.WorldMatrix);
        if (hasParameter(BuiltinShaderParameters_ln_WorldViewProjection))
            data.ln_WorldViewProjection = info.WorldViewProjectionMatrix;
        if (hasParameter(BuiltinShaderParameters_ln_WorldView))
            data.ln_WorldView = info.WorldMatrix * cameraInfo.viewMatrix;
        if (hasParameter(BuiltinShaderParameters_ln_WorldViewIT))
            data.ln_WorldViewIT = Matrix::makeTranspose(Matrix::makeInverse(info.WorldMatrix * cameraInfo.viewMatrix));
        if (hasParameter(BuiltinShaderParameters_ln_BoneTextureReciprocalSize) && info.boneTexture)
            data.ln_BoneTextureReciprocalSize = Vector4(1.0f / info.boneTexture->width(), 1.0f / info.boneTexture->height(), 0, 0);
        if (hasParameter(BuiltinShaderParameters_ln_MorphWeights))
            data.ln_MorphWeights.set(info.morphWeights[0], info.morphWeights[1], info.morphWeights[2], info.morphWeights[3]);

        data.ln_objectId = info.objectId;
        descriptor->setUniformBufferData(slotIndex, &data, sizeof(data));
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_BoneTexture];
    if (slotIndex >= 0) {
        descriptor->setTexture(slotIndex, info.boneTexture);
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_BoneLocalQuaternionTexture];
    if (slotIndex >= 0) {
        descriptor->setTexture(slotIndex, info.boneLocalQuaternionTexture);
    }
}

void ShaderPassSemanticsManager::updateSubsetVariables(ShaderDescriptor* descriptor, const SubsetInfo& info) const {
    int slotIndex = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNEffectColorBuffer];
    if (slotIndex >= 0) {
        // 計算に時間がかかるものでもないため、個々のメンバの alive は確認しない
        LNEffectColorBuffer data = {
            info.colorScale.withAlpha(info.colorScale.a * info.opacity).toVector4(),
            info.blendColor.toVector4(),
            info.tone.toVector4(),
        };
        descriptor->setUniformBufferData(slotIndex, &data, sizeof(data));
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_MaterialTexture];
    if (slotIndex >= 0) {
        LN_DCHECK(info.materialTexture);
        descriptor->setTexture(slotIndex, info.materialTexture);
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_NormalMap];
    if (slotIndex >= 0) {
        LN_DCHECK(info.normalMap);
        descriptor->setTexture(slotIndex, info.normalMap);
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_MetallicRoughnessTexture];
    if (slotIndex >= 0) {
        LN_DCHECK(info.metallicRoughnessTexture);
        descriptor->setTexture(slotIndex, info.metallicRoughnessTexture);
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_OcclusionTexture];
    if (slotIndex >= 0) {
        LN_DCHECK(info.occlusionTexture);
        descriptor->setTexture(slotIndex, info.occlusionTexture);
    }
}

void ShaderPassSemanticsManager::updateSubsetVariables_PBR(ShaderDescriptor* descriptor, const PbrMaterialData& materialData) const {
    int slotIndex = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNPBRMaterialParameter];
    if (slotIndex >= 0) {
        // 計算に時間がかかるものでもないため、個々のメンバの alive は確認しない
        LNPBRMaterialParameter data = {
            materialData.color.toVector4(),
            materialData.emissive.toVector4(),
            materialData.roughness,
            materialData.metallic,
        };
        descriptor->setUniformBufferData(slotIndex, &data, sizeof(data));
    }
}

void ShaderPassSemanticsManager::updateClusteredShadingVariables(ShaderDescriptor* descriptor, const ClusteredShadingRendererInfo& info) const {
    int slotIndex = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNClusteredShadingParameters];
    if (slotIndex >= 0) {
        // 計算に時間がかかるものでもないため、個々のメンバの alive は確認しない
        LNClusteredShadingParameters data = {
            info.fogParams,
            info.fogColorAndDensity,
            info.mainLightDirection,
            info.nearClip,
            info.farClip,
        };
        descriptor->setUniformBufferData(slotIndex, &data, sizeof(data));
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_ClustersTexture];
    if (slotIndex >= 0) {
        descriptor->setTexture(slotIndex, info.lightClustersTexture);
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_GlobalLightInfoTexture];
    if (slotIndex >= 0) {
        descriptor->setTexture(slotIndex, info.globalLightInfoTexture);
    }

    slotIndex = m_builtinShaderTextures[BuiltinShaderTextures_ln_PointLightInfoTexture];
    if (slotIndex >= 0) {
        descriptor->setTexture(slotIndex, info.localLightInfoTexture);
    }
}

} // namespace detail
} // namespace ln
