
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include <LuminoEngine/Shader/ShaderInterfaceFramework.hpp>

namespace ln {
namespace detail {

//==============================================================================
// CameraInfo

void CameraInfo::makePerspective(const Vector3& viewPos, const Vector3& viewDir, float fovY, const Size& size, float n, float f)
{
    //dataSourceId = 0;
    viewPixelSize = size;
    viewPosition = viewPos;
    viewDirection = viewDir;
    viewMatrix = Matrix::makeLookAtLH(viewPos, viewPos + viewDir, Vector3::UnitY);
    //projMatrix = Matrix::makeOrthoLH(size.width, size.height, n, f);//Matrix::makePerspectiveFovLH(fovY, size.width / size.height, n, f);
    projMatrix = Matrix::makePerspectiveFovLH(fovY, size.width / size.height, n, f);
    viewProjMatrix = viewMatrix * projMatrix;
    viewFrustum = ViewFrustum(viewProjMatrix);
    //zSortDistanceBase = ZSortDistanceBase::CameraScreenDistance;
    nearClip = n;
    farClip = f;
}

//=============================================================================
// ShaderTechniqueSemanticsManager
//		参考:
//		Unity Builtin Shader variables
//		https://docs.unity3d.com/Manual/SL-UnityShaderVariables.html

static const std::unordered_map<String, BuiltinShaderParameters> s_BuiltinShaderParametersMap =
{
    {_LT("ln_View"), BuiltinShaderParameters_ln_View},
    {_LT("ln_Projection"), BuiltinShaderParameters_ln_Projection},
    {_LT("ln_ProjectionI"), BuiltinShaderParameters_ln_ProjectionI},
    {_LT("ln_Resolution"), BuiltinShaderParameters_ln_Resolution},
    {_LT("ln_mainLightShadowMapResolution"), BuiltinShaderParameters_ln_mainLightShadowMapResolution},
    {_LT("ln_CameraPosition"), BuiltinShaderParameters_ln_CameraPosition},
    {_LT("ln_CameraDirection"), BuiltinShaderParameters_ln_CameraDirection},
    {_LT("ln_NearClip"), BuiltinShaderParameters_ln_NearClip},
    {_LT("ln_FarClip"), BuiltinShaderParameters_ln_FarClip},

    {_LT("ln_World"), BuiltinShaderParameters_ln_World},
    {_LT("ln_WorldI"), BuiltinShaderParameters_ln_WorldI},
    {_LT("ln_WorldViewProjection"), BuiltinShaderParameters_ln_WorldViewProjection},
    {_LT("ln_WorldView"), BuiltinShaderParameters_ln_WorldView},
    {_LT("ln_WorldViewIT"), BuiltinShaderParameters_ln_WorldViewIT},
    {_LT("ln_BoneTextureReciprocalSize"), BuiltinShaderParameters_ln_BoneTextureReciprocalSize},

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
};

static const std::unordered_map<String, BuiltinShaderTextures> s_BuiltinShaderTexturesMap =
{
    {_LT("ln_MaterialTexture"), BuiltinShaderTextures_ln_MaterialTexture},
    {_LT("ln_NormalMap"), BuiltinShaderTextures_ln_NormalMap},
    {_LT("ln_MaterialRoughnessMap"), BuiltinShaderTextures_ln_MaterialRoughnessMap},
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
    assert(192 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_Resolution));
    assert(208 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_CameraPosition));
    assert(224 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_CameraDirection));
    assert(240 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_NearClip));
    assert(244 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_FarClip));
    static_assert(256 == sizeof(LNRenderViewBuffer), "Invalid sizeof(LNRenderViewBuffer)");
    static_assert(352 == sizeof(LNRenderElementBuffer), "Invalid sizeof(LNRenderViewBuffer)");
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

void ShaderTechniqueSemanticsManager::updateSceneVariables(const SceneInfo& info)
{
}

void ShaderTechniqueSemanticsManager::updateRenderViewVariables(const RenderViewInfo& info)
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
        data.ln_NearClip = c.nearClip;
        data.ln_FarClip = c.farClip;
        data.ln_CameraPosition = Vector4(c.viewPosition, 1.0f);
        data.ln_CameraDirection = Vector4(c.viewDirection, 1.0f);
        m_descriptor->setData(index, &data, sizeof(data));
    }

    index = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNShadowParameters];
    if (index >= 0) {
        LNShadowParameters data;
        data.ln_mainLightShadowMapResolution = Vector4(info.mainLightShadowMapPixelSize, 1.0f / info.mainLightShadowMapPixelSize.width, 1.0f / info.mainLightShadowMapPixelSize.height);
        data.ln_shadowDensity = Vector4(info.mainLightShadowDensity, 0, 0, 0);
        m_descriptor->setData(index, &data, sizeof(data));
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_mainLightShadowMap];
    if (index >= 0) {
        LN_DCHECK(info.mainLightShadowMap);
        m_descriptor->setTexture(index, info.mainLightShadowMap);
    }
}

void ShaderTechniqueSemanticsManager::updateElementVariables(const CameraInfo& cameraInfo, const ElementInfo& info)
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

        data.ln_objectId = info.objectId;
        m_descriptor->setData(index, &data, sizeof(data));
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_BoneTexture];
    if (index >= 0)
        m_descriptor->setTexture(index, info.boneTexture);

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_BoneLocalQuaternionTexture];
    if (index >= 0)
        m_descriptor->setTexture(index, info.boneLocalQuaternionTexture);
}

void ShaderTechniqueSemanticsManager::updateSubsetVariables(const SubsetInfo& info)
{
    int index = m_builtinUniformBuffers[BuiltinShaderUniformBuffers_LNEffectColorBuffer];
    if (index >= 0) {
        // 計算に時間がかかるものでもないため、個々のメンバの alive は確認しない
        LNEffectColorBuffer data = {
            info.colorScale.withAlpha(info.colorScale.a * info.opacity).toVector4(),
            info.blendColor.toVector4(),
            info.tone.toVector4(),
        };
        m_descriptor->setData(index, &data, sizeof(data));
    }


    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_MaterialTexture];
    if (index >= 0) {
        LN_DCHECK(info.materialTexture);
        m_descriptor->setTexture(index, info.materialTexture);
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_NormalMap];
    if (index >= 0) {
        LN_DCHECK(info.normalMap);
        m_descriptor->setTexture(index, info.normalMap);
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_MaterialRoughnessMap];
    if (index >= 0) {
        LN_DCHECK(info.roughnessMap);
        m_descriptor->setTexture(index, info.roughnessMap);
    }
}

void ShaderTechniqueSemanticsManager::updateSubsetVariables_PBR(const PbrMaterialData& materialData)
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
        m_descriptor->setData(index, &data, sizeof(data));
    }
}

void ShaderTechniqueSemanticsManager::updateClusteredShadingVariables(const ClusteredShadingRendererInfo& info) const
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
        m_descriptor->setData(index, &data, sizeof(data));
    }

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_ClustersTexture];
    if (index >= 0)
        m_descriptor->setTexture(index, info.lightClustersTexture);

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_GlobalLightInfoTexture];
    if (index >= 0)
        m_descriptor->setTexture(index, info.globalLightInfoTexture);

    index = m_builtinShaderTextures[BuiltinShaderTextures_ln_PointLightInfoTexture];
    if (index >= 0)
        m_descriptor->setTexture(index, info.localLightInfoTexture);
}

//=============================================================================
// ShaderTechniqueClass

void ShaderTechniqueClass::parseTechniqueClassString(const String& str, ShaderTechniqueClass* outClassSet)
{
    outClassSet->defaultTechnique = false;
    //outClassSet->phase = ShaderTechniqueClass_Phase::Forward;
    outClassSet->meshProcess = ShaderTechniqueClass_MeshProcess::StaticMesh;
    outClassSet->shadingModel = ShaderTechniqueClass_ShadingModel::Default;
    outClassSet->drawMode = ShaderTechniqueClass_DrawMode::Primitive;
    if (String::compare(str, u"Default", CaseSensitivity::CaseInsensitive) == 0)
    {
        outClassSet->defaultTechnique = true;
    }
	else if (String::compare(str, u"LightDisc", CaseSensitivity::CaseInsensitive) == 0)
	{
        outClassSet->phase = ShaderTechniqueClass_Phase::LightDisc;
	}
    else
    {
        // TODO: splitRef
        auto tokens = str.split(u"_", StringSplitOptions::RemoveEmptyEntries);
        for (auto& token : tokens) {
            if (String::compare(token, u"SkinnedMesh", CaseSensitivity::CaseInsensitive) == 0) {
                outClassSet->meshProcess = ShaderTechniqueClass_MeshProcess::SkinnedMesh;
            }
            else if (String::compare(token, u"UnLighting", CaseSensitivity::CaseInsensitive) == 0) {
                outClassSet->shadingModel = ShaderTechniqueClass_ShadingModel::Unlit;
            }
            else if (String::compare(token, u"Instancing", CaseSensitivity::CaseInsensitive) == 0) {
                outClassSet->drawMode = ShaderTechniqueClass_DrawMode::Instancing;
            }
        }
    }
}

bool ShaderTechniqueClass::equals(const ShaderTechniqueClass& a, const ShaderTechniqueClass& b)
{
    return a.phase == b.phase &&
           a.meshProcess == b.meshProcess &&
           a.shadingModel == b.shadingModel &&
           a.drawMode == b.drawMode &&
           a.normalClass == b.normalClass&&
           a.roughnessClass == b.roughnessClass;
}

} // namespace detail
} // namespace ln
