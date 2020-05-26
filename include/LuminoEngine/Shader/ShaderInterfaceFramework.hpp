// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "../Graphics/GeometryStructs.hpp"
#include "../Graphics/ColorStructs.hpp"

namespace ln {
class Texture;
class Texture2D;
class ShaderParameter;
class ShaderConstantBuffer;
namespace detail {

// cbuffer LNRenderViewBuffer
struct alignas(16) LNRenderViewBuffer
{
    alignas(16) Matrix ln_View;
    alignas(16) Matrix ln_Projection;
    alignas(16) Vector3 ln_CameraPosition;
    alignas(16) Vector3 ln_CameraDirection;
    alignas(8) Vector2 ln_ViewportPixelSize;
    alignas(4) float ln_NearClip;
    alignas(4) float ln_FarClip;
};

// cbuffer LNRenderElementBuffer
struct alignas(16) LNRenderElementBuffer
{
    alignas(16) Matrix ln_World;
    alignas(16) Matrix ln_WorldViewProjection;
    alignas(16) Matrix ln_WorldView;
    alignas(16) Matrix ln_WorldViewIT;
    alignas(8) Vector4 ln_BoneTextureReciprocalSize;
};

// シェーダ変数セマンティクス
enum class BuiltinSemantics
{
    //--------------------
    // Scene unit
    Dummy,

    //--------------------
    // Element unit
    WorldViewProjection,
    World,
    WorldView,
    WorldViewIT, // transpose(inverse(WorldView));

    //LightEnables,     // [Mark only] bool[]
    //LightWVPMatrices, // [Mark only] matrix[]
    //LightDirections,  // [Mark only] vector[]
    //LightPositions,   // [Mark only] vector[]
    //LightZFars,       // [Mark only] float[]
    //LightDiffuses,    // [Mark only] vector[]
    //LightAmbients,    // [Mark only] vector[]
    //LightSpeculars,   // [Mark only] vector[]

    //BoneTextureReciprocalSize,  // internal
    BoneTexture,                // internal
    BoneLocalQuaternionTexture, // internal

    //--------------------
    // Subset unit
    MaterialTexture, // glslang の HLSL Parser が texture をサポートしていないため Texture2D 固定。

    MaterialColor,     // [PBR] vector
    MaterialRoughness, // [PBR] float
    MaterialMetallic,  // [PBR] float
                       //MaterialSpecular,		// [PBR] float
    MaterialEmissive,  // [PBR] vector

    PhongMaterialDiffuse,       // [Phong] vector
    PhongMaterialAmbient,       // [Phong] vector
    PhongMaterialEmmisive,      // [Phong] vector
    PhongMaterialSpecularColor, // [Phong] vector
    PhongMaterialSpecularPower, // [Phong] float

    ColorScale, // vector (Built-in effect)
    BlendColor, // vector (Built-in effect)
    ToneColor,  // vector (Built-in effect)

    //--------------------
    // SceneRenderer internal
    GlobalLightInfoTexture,
    LocalLightInfoTexture,
    LightClustersTexture,
    NearClip2,
    FarClip2,
    CameraPosition2,
	FogColorAndDensity,
    FogParams,

    _Count,
};

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
    //Matrix	ViewProjectionMatrix;
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

// 描画要素単位のデータに関する情報
struct ElementInfo
{
    const Matrix* viewProjMatrix;

    Matrix WorldMatrix;
    Matrix WorldViewProjectionMatrix;

    Texture2D* boneTexture = nullptr;
    Texture2D* boneLocalQuaternionTexture = nullptr;
};

// サブセット単位のデータに関する情報
// TODO: descripterInfo (PipelineStateやRenderPass,Framebufferではなく、Descripterを差し替えれば済む系のパラメータ)
struct SubsetInfo
{
    // common material
    Texture* materialTexture;

    // builtin effect
    float opacity;
    Color colorScale;
    Color blendColor;
	ColorTone tone;

	void clear()
	{
		materialTexture = nullptr;
		opacity = 0.0f;
		colorScale = Color();
		blendColor = Color();
		tone = ColorTone();
	}

	static bool equals(const SubsetInfo& lhs, const SubsetInfo& rhs)
	{
		return
			lhs.materialTexture == rhs.materialTexture &&
			Math::nearEqual(lhs.opacity, rhs.opacity) &&
			Color::nearEqual(lhs.colorScale, rhs.colorScale) &&
			Color::nearEqual(lhs.blendColor, rhs.blendColor) &&
			ColorTone::nearEqual(lhs.tone, rhs.tone);
	}
};

// セマンティクスが関係するシェーダ変数の管理
class ShaderSemanticsManager
{
public:
    ShaderSemanticsManager();

    // call by shader creation time.
    void prepareParameter(ShaderParameter* var);    // deprecated
    void prepareConstantBuffer(ShaderConstantBuffer* buffer);

    // call by rendering time.
    void updateSceneVariables(const SceneInfo& info);
    void updateCameraVariables(const CameraInfo& info);
    void updateElementVariables(const CameraInfo& cameraInfo, const ElementInfo& info);
    void updateSubsetVariables(const SubsetInfo& info);
    void updateSubsetVariables_PBR(const PbrMaterialData& materialData);
    void updateSubsetVariables_Phong(const PhongMaterialData& materialData);
    ShaderParameter* getParameterBySemantics(BuiltinSemantics semantics) const;

private:
    struct VariableKindPair
    {
        ShaderParameter* variable;
        BuiltinSemantics kind;
    };

    List<VariableKindPair> m_sceneVariables;
    //List<VariableKindPair> m_cameraVariables;
    ShaderConstantBuffer* m_renderViewBuffer = nullptr;
    List<VariableKindPair> m_elementVariables;
    ShaderConstantBuffer* m_renderElementBuffer = nullptr;
    List<VariableKindPair> m_subsetVariables;
    // TODO: 実質↑のほとんどの変数は使うので、リストを分けるとかえって変にメモリ使ってしまうかも。
    // ↓のような全体でひとつのテーブル使う方がかえって検索効率もよくなる。後でこっちにする。
    std::array<ShaderParameter*, (int)BuiltinSemantics::_Count> m_variablesTable;
};



enum BuiltinShaderParameters
{
    // LNRenderViewBuffer
    BuiltinShaderParameters_ln_View,
    BuiltinShaderParameters_ln_Projection,
    BuiltinShaderParameters_ln_CameraPosition,
    BuiltinShaderParameters_ln_CameraDirection,
    BuiltinShaderParameters_ln_ViewportPixelSize,
    BuiltinShaderParameters_ln_NearClip,
    BuiltinShaderParameters_ln_FarClip,

    // LNRenderElementBuffer
    BuiltinShaderParameters_ln_World,
    BuiltinShaderParameters_ln_WorldViewProjection,
    BuiltinShaderParameters_ln_WorldView,
    BuiltinShaderParameters_ln_WorldViewIT,
    BuiltinShaderParameters_ln_BoneTextureReciprocalSize,

    // LNEffectColorBuffer
    BuiltinShaderParameters_ln_ColorScale,
    BuiltinShaderParameters_ln_BlendColor,
    BuiltinShaderParameters_ln_ToneColor,

    BuiltinShaderParameters_ln_MaterialTexture,
    BuiltinShaderParameters_ln_BoneTexture,
    BuiltinShaderParameters_ln_BoneLocalQuaternionTexture,

    BuiltinShaderParameters__Count,
};

enum BuiltinShaderUniformBuffers
{
    BuiltinShaderUniformBuffers_LNRenderViewBuffer,
    BuiltinShaderUniformBuffers_LNRenderElementBuffer,
    BuiltinShaderUniformBuffers_LNEffectColorBuffer,
    BuiltinShaderUniformBuffers__Count,
};

// セマンティクスが関係するシェーダ変数の管理
class ShaderPassSemanticsManager
{
public:
    ShaderPassSemanticsManager();
    void init(ShaderPass* shaderPass);
    void reset();

    // call by rendering time.
    void updateSceneVariables(const SceneInfo& info);
    void updateCameraVariables(const CameraInfo& info);
    void updateElementVariables(const CameraInfo& cameraInfo, const ElementInfo& info);
    void updateSubsetVariables(const SubsetInfo& info);
    void updateSubsetVariables_PBR(const PbrMaterialData& materialData);
    void updateSubsetVariables_Phong(const PhongMaterialData& materialData);
    ShaderParameter* getParameterBySemantics(BuiltinSemantics semantics) const;

private:
    struct VariableKindPair
    {
        int index;  // member or texture or sampler index.
        BuiltinSemantics kind;
    };

    // Boolean flags BuiltinShaderParameters
    uint64_t m_hasBuiltinShaderParameters;

    // Index of ShaderDescriptorLayout::m_buffers,m_textures,m_samplers
    std::array<int, BuiltinShaderUniformBuffers__Count> m_builtinUniformBuffers;
};

// LigitingModel
enum class ShaderTechniqueClass_Ligiting : uint8_t
{
    Forward, // default
             // TODO: Differd
	LightDisc,
};

enum class ShaderTechniqueClass_Phase : uint8_t
{
    Geometry, // default
              // TODO: ShadowCaster
              // TODO: DepthPrepass
};

// VertexFactory
enum class ShaderTechniqueClass_MeshProcess : uint8_t
{
    StaticMesh, // default
    SkinnedMesh,
};

// PixelShader
enum class ShaderTechniqueClass_ShadingModel : uint8_t
{
    Default,
    Unlit,
};

enum class ShaderTechniqueClass_DrawMode : uint8_t
{
    Primitive, // default
    Instancing,
};

// Rendering モジュールで、Pass が ShaderTechnique を検索するときに使うデータ
struct ShaderTechniqueClass
{
    bool defaultTechnique;  // "Default" とだけ書かれた tech は　SceneRederer のデフォルトよりも優先。主に DebugGrid など、ライトがないことなどでシェーディングモデルが切り替わっても同じ tech を使いたいケース用。
    ShaderTechniqueClass_Ligiting ligiting;
    ShaderTechniqueClass_Phase phase;
    ShaderTechniqueClass_MeshProcess meshProcess;
    ShaderTechniqueClass_ShadingModel shadingModel;
    ShaderTechniqueClass_DrawMode drawMode;

    static void parseTechniqueClassString(const String& str, ShaderTechniqueClass* outClassSet);
    static bool equals(const ShaderTechniqueClass& a, const ShaderTechniqueClass& b);
};

} // namespace detail
} // namespace ln
