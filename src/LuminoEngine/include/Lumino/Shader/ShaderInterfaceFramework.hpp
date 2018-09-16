
#pragma once
#include "Common.hpp"
#include "../Graphics/GeometryStructs.hpp"
#include "../Graphics/ColorStructs.hpp"

namespace ln {
class Texture;
class Texture2D;
class ShaderParameter;
namespace detail {

// シェーダ変数セマンティクス
enum class BuiltinSemantics
{
	//--------------------
	// Scene unit
	Dummy,

	//--------------------
	// Camera unit
	View,
	Projection,
	ViewportPixelSize,
	NearClip,
	FarClip,
	CameraPosition,
	CameraDirection,

	//--------------------
	// Element unit
	WorldViewProjection,
	World,
	WorldView,
	WorldViewIT,			// transpose(inverse(WorldView));

	LightEnables,			// [Mark only] bool[]
	LightWVPMatrices,		// [Mark only] matrix[]
	LightDirections,		// [Mark only] vector[]
	LightPositions,			// [Mark only] vector[]
	LightZFars,				// [Mark only] float[]
	LightDiffuses,			// [Mark only] vector[]
	LightAmbients,			// [Mark only] vector[]
	LightSpeculars,			// [Mark only] vector[]

	BoneTextureReciprocalSize,		// internal
	BoneTexture,					// internal
	BoneLocalQuaternionTexture,		// internal

	//--------------------
	// Subset unit
	MaterialTexture,

	MaterialColor,		// [PBR] vector
	MaterialRoughness,	// [PBR] float
	MaterialMetallic,		// [PBR] float
	MaterialSpecular,		// [PBR] float

	MaterialDiffuse,		// [Phong] vector
	MaterialAmbient,		// [Phong] vector
	MaterialEmmisive,		// [Phong] vector
	MaterialSpecularColor,	// [Phong] vector
	MaterialSpecularPower,	// [Phong] float

	ColorScale,				// vector (Built-in effect)
	BlendColor,				// vector (Built-in effect)
	ToneColor,				// vector (Built-in effect)

	_Count,
};

struct PbrMaterialData
{
	Color color;
	float roughness;
	float metallic;
	float specular;
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
	intptr_t	dataSourceId;
	Size		viewPixelSize;
	Vector3		viewPosition;
	Vector3		viewDirection;
	Matrix		viewMatrix;
	Matrix		projMatrix;
	Matrix		viewProjMatrix;
	ViewFrustum	viewFrustum;

	float		nearClip = 0;
	float		farClip = 0;

	// POD

	void makePerspective(const Vector3& viewPos, const Vector3& viewDir, float fovY, const Size& size, float n, float f);
};

// 描画要素単位のデータに関する情報
struct ElementInfo
{
	const Matrix*		viewProjMatrix;

	Matrix				WorldMatrix;
	Matrix				WorldViewProjectionMatrix;

	Texture2D*			boneTexture = nullptr;
	Texture2D*			boneLocalQuaternionTexture = nullptr;
};

// サブセット単位のデータに関する情報
struct SubsetInfo
{
	// common material
	Texture* materialTexture;

	// builtin effect
	float opacity;
	Color colorScale;
	Color blendColor;
	ToneF tone;
};

// セマンティクスが関係するシェーダ変数の管理
class ShaderSemanticsManager
{
public:
	ShaderSemanticsManager();

	// call by shader creation time.
	void prepareParameter(ShaderParameter* var);

	// call by rendering time.
	void updateSceneVariables(const SceneInfo& info);
	void updateCameraVariables(const CameraInfo& info);
	void updateElementVariables(const CameraInfo& cameraInfo, const ElementInfo& info);
	void updateSubsetVariables(const SubsetInfo& info);
	void updateSubsetVariables_PBR(const PbrMaterialData& materialData);
	void updateSubsetVariables_Phong(const PhongMaterialData& materialData);

private:
	struct VariableKindPair
	{
		ShaderParameter* variable;
		BuiltinSemantics kind;
	};

	List<VariableKindPair>	m_sceneVariables;
	List<VariableKindPair>	m_cameraVariables;
	List<VariableKindPair>	m_elementVariables;
	List<VariableKindPair>	m_subsetVariables;
	//intptr_t				m_lastCameraInfoId;

	//MemoryStream			m_tempBuffer;
	//BinaryWriter			m_tempBufferWriter;
};


// LigitingModel
enum class ShaderTechniqueClass_Ligiting : uint8_t
{
	Forward,		// default
	// TODO: Differd
};

enum class ShaderTechniqueClass_Phase : uint8_t
{
	Geometry,		// default
	// TODO: ShadowCaster
	// TODO: DepthPrepass
};

// VertexFactory
enum class ShaderTechniqueClass_MeshProcess : uint8_t
{
	StaticMesh,		// default
	SkinnedMesh,
};

// PixelShader
enum class ShaderTechniqueClass_ShadingModel : uint8_t
{
	Default,
	UnLighting,
};

// Rendering モジュールで、Pass が ShaderTechnique を検索するときに使うデータ
struct ShaderTechniqueClass
{
	ShaderTechniqueClass_Ligiting ligiting;
	ShaderTechniqueClass_Phase phase;
	ShaderTechniqueClass_MeshProcess meshProcess;
	ShaderTechniqueClass_ShadingModel shadingModel;

	static void parseTechniqueClassString(const String& str, ShaderTechniqueClass* outClassSet);
	static bool equals(const ShaderTechniqueClass& a, const ShaderTechniqueClass& b);
};

} // namespace detail
} // namespace ln
