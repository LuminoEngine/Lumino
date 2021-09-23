// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include <LuminoEngine/Graphics/ColorStructs.hpp>

namespace ln {
class Texture;
class Texture2D;
class ShaderDefaultDescriptor;
namespace detail {

// cbuffer LNRenderViewBuffer
struct alignas(16) LNRenderViewBuffer
{
    alignas(16) Matrix ln_View;
    alignas(16) Matrix ln_Projection;
    alignas(16) Matrix ln_ProjectionI;
    alignas(16) Matrix _ln_MainLightMatrix;
    alignas(16) Vector4 ln_Resolution;
    alignas(16) Vector4 ln_CameraPosition;      // .w = ln_NearClip
    alignas(16) Vector4 ln_CameraDirection;     // .w = ln_FarClip
    alignas(16) Vector4 ln_AmbientColor;
    alignas(16) Vector4 ln_AmbientSkyColor;
    alignas(16) Vector4 ln_AmbientGroundColor;
    alignas(16) Vector4 _ln_MainLightColor;
    alignas(16) Vector4 _ln_MainLightPos;
    alignas(16) Vector4 _ln_MainLightDir;
};

// cbuffer LNRenderElementBuffer
struct alignas(16) LNRenderElementBuffer
{
    alignas(16) Matrix ln_World;
    alignas(16) Matrix ln_WorldI;
    alignas(16) Matrix ln_WorldViewProjection;
    alignas(16) Matrix ln_WorldView;
    alignas(16) Matrix ln_WorldViewIT;
    alignas(16) Vector4 ln_BoneTextureReciprocalSize;   // .zw is not use. ([0, 0])
    alignas(16) Vector4 ln_MorphWeights;
    alignas(4) int32_t ln_objectId;
};

// cbuffer LNEffectColorBuffer
struct alignas(16) LNEffectColorBuffer
{
    alignas(16) Vector4 ln_ColorScale;
    alignas(16) Vector4 ln_BlendColor;
    alignas(16) Vector4 ln_ToneColor;
};

// cbuffer LNPBRMaterialParameter
struct alignas(16) LNPBRMaterialParameter
{
    alignas(16) Vector4 ln_MaterialColor;
    alignas(16) Vector4 ln_MaterialEmissive;
    alignas(4) float ln_MaterialRoughness;
    alignas(4) float ln_MaterialMetallic;
};

// cbuffer LNClusteredShadingParameters
struct alignas(16) LNClusteredShadingParameters
{
    alignas(16) Vector4 ln_FogParams;
    alignas(16) Vector4 ln_FogColorAndDensity;
    alignas(16) Vector3 ln_MainLightDirection;
    alignas(4) float ln_NearClip;
    alignas(4) float ln_FarClip;
};

// cbuffer LNShadowParameters
struct alignas(16) LNShadowParameters
{
    alignas(16) Vector4 ln_mainLightShadowMapResolution;
    alignas(16) Vector4 ln_shadowDensity;
};

// Note: specular は簡単のため無し。
// Three.js では無し。
// https://threejs.org/docs/#api/en/materials/MeshStandardMaterial
// UE4 では有るが、「0.5のままで十分」としている。
// https://docs.unrealengine.com/ja/Engine/Rendering/Materials/HowTo/ShinyMaterials/index.html
// http://envgameartist.blogspot.com/2014/12/pbr.html
struct PbrMaterialData
{
    Color color;
    float roughness;
    float metallic;
    //float specular;
    Color emissive;
};

struct PhongMaterialData
{
    Color diffuse;
    Color ambient;
    Color specular;
    Color emissive;
    float power;
};

// シーン単位のデータに関する情報
struct SceneInfo
{
    Color ambientColor;
    Color ambientSkyColor;
    Color ambientGroundColor;
    Color mainLightColor;
    Vector4 mainLightPos;
    Vector4 mainLightDir;
    Matrix mainLightMatrix;
};

// カメラ単位のデータに関する情報
struct CameraInfo
{
    //intptr_t	dataSourceId;
    Size viewPixelSize;
    Vector3 viewPosition;
    Vector3 viewDirection;
    Matrix viewMatrix;
    Matrix projMatrix;
    Matrix viewProjMatrix;
    ViewFrustum viewFrustum;

    float nearClip = 0;
    float farClip = 0;

	float dpiScale = 1.0f;

    void makeUnproject(const Size& size)
    {
        viewPixelSize = size;
        viewPosition = Vector3::Zero;
        viewDirection = Vector3::UnitZ;
        viewMatrix = Matrix::Identity;
        projMatrix = Matrix::Identity;
        viewProjMatrix = Matrix::Identity;
        viewFrustum = ViewFrustum();
        nearClip = 0.0f;
        farClip = 1.0f;
    }
    void makePerspective(const Vector3& viewPos, const Vector3& viewDir, float fovY, const Size& size, float n, float f);
};

struct RenderViewInfo
{
    CameraInfo cameraInfo;
    Size mainLightShadowMapPixelSize;
    Texture* mainLightShadowMap = nullptr;
    Matrix mainLightViewProjection;
    float mainLightShadowDensity = 0.0f;
};

// 描画要素単位のデータに関する情報
struct ElementInfo
{
    const Matrix* viewProjMatrix;

    Matrix WorldMatrix;
    Matrix WorldViewProjectionMatrix;

    Texture2D* boneTexture = nullptr;
    Texture2D* boneLocalQuaternionTexture = nullptr;

    std::array<float, 4> morphWeights;

    int objectId = 0;
};

// サブセット単位のデータに関する情報
// TODO: descripterInfo (PipelineStateやRenderPass,Framebufferではなく、Descripterを差し替えれば済む系のパラメータ)
struct SubsetInfo
{
    // common material
    Texture* materialTexture;
    Texture* normalMap;
    Texture* metallicRoughnessTexture;
    Texture* occlusionTexture;

    // builtin effect
    float opacity;
    Color colorScale;
    Color blendColor;
	ColorTone tone;

	void clear()
	{
		materialTexture = nullptr;
        normalMap = nullptr;
        metallicRoughnessTexture = nullptr;
		opacity = 0.0f;
		colorScale = Color();
		blendColor = Color();
		tone = ColorTone();
	}

	static bool equals(const SubsetInfo& lhs, const SubsetInfo& rhs)
	{
		return
			lhs.materialTexture == rhs.materialTexture &&
            lhs.normalMap == rhs.normalMap &&
            lhs.metallicRoughnessTexture == rhs.metallicRoughnessTexture &&
			Math::nearEqual(lhs.opacity, rhs.opacity) &&
			Color::nearEqual(lhs.colorScale, rhs.colorScale) &&
			Color::nearEqual(lhs.blendColor, rhs.blendColor) &&
			ColorTone::nearEqual(lhs.tone, rhs.tone);
	}
};

struct ClusteredShadingRendererInfo
{
    Texture2D* globalLightInfoTexture;
    Texture2D* localLightInfoTexture;
    Texture2D* lightClustersTexture;

    Vector3 mainLightDirection;
    Vector4 fogParams;
    Vector4 fogColorAndDensity;
    float nearClip;
    float farClip;
};

enum BuiltinShaderParameters
{
    // LNRenderViewBuffer
    BuiltinShaderParameters_ln_View,
    BuiltinShaderParameters_ln_Projection,
    BuiltinShaderParameters_ln_ProjectionI,
    BuiltinShaderParameters_ln_Resolution,
    BuiltinShaderParameters_ln_MainLightMatrix,
    BuiltinShaderParameters_ln_CameraPosition,
    BuiltinShaderParameters_ln_CameraDirection,
    BuiltinShaderParameters_ln_AmbientColor,
    BuiltinShaderParameters_ln_AmbientSkyColor,
    BuiltinShaderParameters_ln_AmbientGroundColor,
    BuiltinShaderParameters_ln_MainLightColor,
    BuiltinShaderParameters_ln_MainLightPos,
    BuiltinShaderParameters_ln_MainLightDir,

    // LNRenderElementBuffer
    BuiltinShaderParameters_ln_World,
    BuiltinShaderParameters_ln_WorldI,
    BuiltinShaderParameters_ln_WorldViewProjection,
    BuiltinShaderParameters_ln_WorldView,
    BuiltinShaderParameters_ln_WorldViewIT,
    BuiltinShaderParameters_ln_BoneTextureReciprocalSize,
    BuiltinShaderParameters_ln_MorphWeights,

    // LNEffectColorBuffer
    BuiltinShaderParameters_ln_ColorScale,
    BuiltinShaderParameters_ln_BlendColor,
    BuiltinShaderParameters_ln_ToneColor,

    // LNPBRMaterialParameter
    BuiltinShaderParameters_ln_MaterialColor,
    BuiltinShaderParameters_ln_MaterialEmissive,
    BuiltinShaderParameters_ln_MaterialRoughness,
    BuiltinShaderParameters_ln_MaterialMetallic,

    BuiltinShaderParameters__Count,
};

enum BuiltinShaderUniformBuffers
{
    BuiltinShaderUniformBuffers_LNRenderViewBuffer,
    BuiltinShaderUniformBuffers_LNRenderElementBuffer,
    BuiltinShaderUniformBuffers_LNEffectColorBuffer,
    BuiltinShaderUniformBuffers_LNPBRMaterialParameter,
    BuiltinShaderUniformBuffers_LNClusteredShadingParameters,
    BuiltinShaderUniformBuffers_LNShadowParameters,
    BuiltinShaderUniformBuffers_Global, // for Material user params
    
    BuiltinShaderUniformBuffers__Count,
};

enum BuiltinShaderTextures
{
    BuiltinShaderTextures_ln_MaterialTexture, // TODO: MaterialMainTexture
    BuiltinShaderTextures_ln_NormalMap, // TODO: MaterialNormalMap
    BuiltinShaderTextures_ln_MetallicRoughnessTexture,
    BuiltinShaderTextures_ln_OcclusionTexture,
    BuiltinShaderTextures_ln_mainLightShadowMap,
    BuiltinShaderTextures_ln_BoneTexture,
    BuiltinShaderTextures_ln_BoneLocalQuaternionTexture,

    // ForwardRendering
    BuiltinShaderTextures_ln_ClustersTexture,
    BuiltinShaderTextures_ln_GlobalLightInfoTexture,
    BuiltinShaderTextures_ln_PointLightInfoTexture,

    BuiltinShaderTextures__Count,
};

// セマンティクスが関係するシェーダ変数の管理
class ShaderTechniqueSemanticsManager
{
public:
    ShaderTechniqueSemanticsManager();
    void init(ShaderTechnique* technique);
    void reset();

    int getBuiltinShaderUniformBufferIndex(BuiltinShaderUniformBuffers buffer) const { return m_builtinUniformBuffers[static_cast<int>(buffer)]; }

    // call by rendering time.
    void updateRenderViewVariables(ShaderSecondaryDescriptor* descriptor, const RenderViewInfo& info, const SceneInfo& sceneInfo) const;
    void updateElementVariables(ShaderSecondaryDescriptor* descriptor, const CameraInfo& cameraInfo, const ElementInfo& info) const;
    void updateSubsetVariables(ShaderSecondaryDescriptor* descriptor, const SubsetInfo& info) const;
    void updateSubsetVariables_PBR(ShaderSecondaryDescriptor* descriptor, const PbrMaterialData& materialData) const;
    void updateClusteredShadingVariables(ShaderSecondaryDescriptor* descriptor, const ClusteredShadingRendererInfo& info) const;

private:
    //struct VariableKindPair
    //{
    //    int index;  // member or texture or sampler index.
    //    BuiltinSemantics kind;
    //};

    bool hasParameter(BuiltinShaderParameters v) const { return (m_hasBuiltinShaderParameters & (1ULL << v)) != 0; }

    ShaderDefaultDescriptor* m_descriptor;

    // Boolean flags BuiltinShaderParameters
    uint64_t m_hasBuiltinShaderParameters;

    // Index of ShaderDescriptorLayout::m_buffers
    std::array<int, BuiltinShaderUniformBuffers__Count> m_builtinUniformBuffers;

    // Index of ShaderDescriptorLayout::m_textures
    std::array<int, BuiltinShaderTextures__Count> m_builtinShaderTextures;
};



} // namespace detail
} // namespace ln
