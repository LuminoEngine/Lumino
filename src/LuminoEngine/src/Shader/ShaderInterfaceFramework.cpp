
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

//==============================================================================
// ShaderSemanticsManager
//		参考:
//		Unity Builtin Shader variables
//		https://docs.unity3d.com/Manual/SL-UnityShaderVariables.html

static std::unordered_map<String, BuiltinSemantics> g_builtinNameMap_CameraUnit =
    {
        {_LT("ln_View"), BuiltinSemantics::View},
        {_LT("ln_Projection"), BuiltinSemantics::Projection},
        {_LT("ln_ViewportPixelSize"), BuiltinSemantics::ViewportPixelSize},
        {_LT("ln_NearClip"), BuiltinSemantics::NearClip},
        {_LT("ln_FarClip"), BuiltinSemantics::FarClip},
        {_LT("ln_CameraPosition"), BuiltinSemantics::CameraPosition},
        {_LT("ln_CameraDirection"), BuiltinSemantics::CameraDirection},
};

static std::unordered_map<String, BuiltinSemantics> g_builtinNameMap_ElementUnit =
    {
        {_LT("ln_WorldViewProjection"), BuiltinSemantics::WorldViewProjection},
        {_LT("ln_World"), BuiltinSemantics::World},
        {_LT("ln_WorldView"), BuiltinSemantics::WorldView},
        {_LT("ln_WorldViewIT"), BuiltinSemantics::WorldViewIT},
        {_LT("ln_LightEnables"), BuiltinSemantics::LightEnables},
        {_LT("ln_LightWVPMatrices"), BuiltinSemantics::LightWVPMatrices},
        {_LT("ln_LightDirections"), BuiltinSemantics::LightDirections},
        {_LT("ln_LightPositions"), BuiltinSemantics::LightPositions},
        {_LT("ln_LightZFars"), BuiltinSemantics::LightZFars},
        {_LT("ln_LightDiffuses"), BuiltinSemantics::LightDiffuses},
        {_LT("ln_LightAmbients"), BuiltinSemantics::LightAmbients},
        {_LT("ln_LightSpeculars"), BuiltinSemantics::LightSpeculars},
        {_LT("ln_BoneTextureReciprocalSize"), BuiltinSemantics::BoneTextureReciprocalSize},
        {_LT("ln_BoneTexture"), BuiltinSemantics::BoneTexture},
        {_LT("ln_BoneLocalQuaternionTexture"), BuiltinSemantics::BoneLocalQuaternionTexture},
};

static std::unordered_map<String, BuiltinSemantics> g_builtinNameMap_SubsetUnit =
    {
        {_LT("ln_MaterialTexture"), BuiltinSemantics::MaterialTexture},
        {_LT("ln_MaterialColor"), BuiltinSemantics::MaterialColor},
        {_LT("ln_MaterialRoughness"), BuiltinSemantics::MaterialRoughness},
        {_LT("ln_MaterialMetallic"), BuiltinSemantics::MaterialMetallic},
        //{ _LT("ln_MaterialSpecular"), BuiltinSemantics::MaterialSpecular },
        {_LT("ln_MaterialEmissive"), BuiltinSemantics::MaterialEmissive},
        {_LT("ln_PhongMaterialDiffuse"), BuiltinSemantics::PhongMaterialDiffuse},
        {_LT("ln_PhongMaterialAmbient"), BuiltinSemantics::PhongMaterialAmbient},
        {_LT("ln_PhongMaterialEmmisive"), BuiltinSemantics::PhongMaterialEmmisive},
        {_LT("ln_PhongMaterialSpecularColor"), BuiltinSemantics::PhongMaterialSpecularColor},
        {_LT("ln_PhongMaterialSpecularPower"), BuiltinSemantics::PhongMaterialSpecularPower},
        {_LT("ln_ColorScale"), BuiltinSemantics::ColorScale},
        {_LT("ln_BlendColor"), BuiltinSemantics::BlendColor},
        {_LT("ln_ToneColor"), BuiltinSemantics::ToneColor},
};

struct BuiltinSemanticsNamePair
{
    String name;
    BuiltinSemantics semantics;
};
static BuiltinSemanticsNamePair g_builtinSemanticsNamePairMap[] =
    {
        {_LT("ln_GlobalLightInfoTexture"), BuiltinSemantics::GlobalLightInfoTexture},
        {_LT("ln_pointLightInfoTexture"), BuiltinSemantics::LocalLightInfoTexture},
        {_LT("ln_clustersTexture"), BuiltinSemantics::LightClustersTexture},
        {_LT("ln_nearClip"), BuiltinSemantics::NearClip2},
        {_LT("ln_farClip"), BuiltinSemantics::FarClip2},
        {_LT("ln_cameraPos"), BuiltinSemantics::CameraPosition2},
		{_LT("ln_FogColorAndDensity"), BuiltinSemantics::FogColorAndDensity},
		{_LT("ln_FogParams"), BuiltinSemantics::FogParams},
};

ShaderSemanticsManager::ShaderSemanticsManager()
    : m_sceneVariables()
    , m_elementVariables()
    , m_subsetVariables()
    , m_variablesTable{}
//, m_lastCameraInfoId(0)
//, m_tempBufferWriter(&m_tempBuffer)
{
    // メモリレイアウトそのまま ConstantBuffer に転送するため、オフセットを検証しておく
    assert(128 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_CameraPosition));
    assert(144 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_CameraDirection));
    assert(160 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_ViewportPixelSize));
    assert(168 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_NearClip));
    assert(172 == LN_MEMBER_OFFSETOF(LNRenderViewBuffer, ln_FarClip));
    assert(176 == sizeof(LNRenderViewBuffer));
    std::cout << sizeof(LNRenderViewBuffer) << std::endl;
}

void ShaderSemanticsManager::prepareParameter(ShaderParameter* var)
{
    const String& name = var->name();

    // try element unit
    {
        auto itr = g_builtinNameMap_ElementUnit.find(name);
        if (itr != g_builtinNameMap_ElementUnit.end()) {
            m_elementVariables.add({var, itr->second});
            return;
        }
    }

    // try subset unit
    {
        auto itr = g_builtinNameMap_SubsetUnit.find(name);
        if (itr != g_builtinNameMap_SubsetUnit.end()) {
            m_subsetVariables.add({var, itr->second});
            return;
        }
    }

    {
        for (auto& pair : g_builtinSemanticsNamePairMap) {
            if (name == pair.name) {
                m_variablesTable[(int)pair.semantics] = var;
            }
        }
    }
}

void ShaderSemanticsManager::prepareConstantBuffer(ShaderConstantBuffer* buffer)
{
    if (buffer->name() == u"LNRenderViewBuffer") {
        m_renderViewBuffer = buffer;
    }
}

void ShaderSemanticsManager::updateSceneVariables(const SceneInfo& info)
{
}

void ShaderSemanticsManager::updateCameraVariables(const CameraInfo& info)
{
    if (m_renderViewBuffer) {
        LNRenderViewBuffer data;
        data.ln_View = info.viewMatrix;
        data.ln_Projection = info.projMatrix;
        data.ln_ViewportPixelSize = info.viewPixelSize;
        data.ln_NearClip = info.nearClip;
        data.ln_FarClip = info.farClip;
        data.ln_CameraPosition = info.viewPosition;
        data.ln_CameraDirection = info.viewDirection;
        m_renderViewBuffer->setData(&data, sizeof(data));
    }
}

void ShaderSemanticsManager::updateElementVariables(const CameraInfo& cameraInfo, const ElementInfo& info)
{
    for (const VariableKindPair& varInfo : m_elementVariables) {
        switch (varInfo.kind) {
            case BuiltinSemantics::WorldViewProjection:
                varInfo.variable->setMatrix(info.WorldViewProjectionMatrix);
                break;
            case BuiltinSemantics::World:
                varInfo.variable->setMatrix(info.WorldMatrix);
                break;
            case BuiltinSemantics::WorldView:
                varInfo.variable->setMatrix(info.WorldMatrix * cameraInfo.viewMatrix);
                break;
            case BuiltinSemantics::WorldViewIT:
                varInfo.variable->setMatrix(Matrix::makeTranspose(Matrix::makeInverse(info.WorldMatrix * cameraInfo.viewMatrix)));
                break;

#if 0
			// TODO: 以下、ライト列挙時に確定したい。何回もこんな計算するのはちょっと・・
		case BuiltinSemantics::LightEnables:
			m_tempBufferWriter.seek(0, SeekOrigin_Begin);
			for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
			{
				// TODO: WriteBool() がほしい
				m_tempBufferWriter.writeUInt8((lights == nullptr) ? false : lights[i] != nullptr);
			}
			varInfo.variable->setBoolArray((const bool*)m_tempBuffer.getBuffer(), DynamicLightInfo::MaxLights);
			break;
		case BuiltinSemantics::LightWVPMatrices:
			LN_NOTIMPLEMENTED();
			//if (lights != nullptr)
			//{
			//	m_tempBufferWriter.Seek(0, SeekOrigin_Begin);
			//	for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
			//	{
			//		Matrix m = (lights[i] != nullptr) ? (lights[i]->transform * (*info.viewProjMatrix)) : Matrix::Identity;
			//		m_tempBufferWriter.Write(&m, sizeof(Matrix));
			//	}
			//	varInfo.variable->setMatrixArray((const Matrix*)m_tempBuffer.getBuffer(), DynamicLightInfo::MaxLights);
			//}
			break;
		case BuiltinSemantics::LightDirections:
			if (lights != nullptr)
			{
				m_tempBufferWriter.seek(0, SeekOrigin_Begin);
				for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
				{
					// TODO: Vector4::Zero がほしい
					Vector4 v = (lights[i] != nullptr) ? Vector4(lights[i]->m_direction, 0) : Vector4(0, 0, 0, 0);
					m_tempBufferWriter.write(&v, sizeof(Vector4));
				}
				varInfo.variable->setVectorArray((const Vector4*)m_tempBuffer.getBuffer(), DynamicLightInfo::MaxLights);
			}
			break;
		case BuiltinSemantics::LightPositions:
			if (lights != nullptr)
			{
				for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
				{
					// TODO: Vector4::Zero がほしい
					Vector4 v = (lights[i] != nullptr) ? Vector4(lights[i]->m_position, 0) : Vector4(0, 0, 0, 0);
					m_tempBufferWriter.write(&v, sizeof(Vector4));
				}
				varInfo.variable->setVectorArray((const Vector4*)m_tempBuffer.getBuffer(), DynamicLightInfo::MaxLights);
			}
			break;
		case BuiltinSemantics::LightZFars:
			if (lights != nullptr)
			{
				m_tempBufferWriter.seek(0, SeekOrigin_Begin);
				for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
				{
					m_tempBufferWriter.writeFloat((lights[i] != nullptr) ? lights[i]->m_shadowZFar : 0.0f);
				}
				varInfo.variable->setFloatArray((const float*)m_tempBuffer.getBuffer(), DynamicLightInfo::MaxLights);
			}
			break;
		case BuiltinSemantics::LightDiffuses:
			if (lights != nullptr)
			{
				m_tempBufferWriter.seek(0, SeekOrigin_Begin);
				for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
				{
					auto& v = (lights[i] != nullptr) ? lights[i]->m_diffuse : Color::Black;
					m_tempBufferWriter.write(&v, sizeof(Color));
				}
				varInfo.variable->setVectorArray((const Vector4*)m_tempBuffer.getBuffer(), DynamicLightInfo::MaxLights);
			}
			break;
		case BuiltinSemantics::LightAmbients:
			if (lights != nullptr)
			{
				m_tempBufferWriter.seek(0, SeekOrigin_Begin);
				for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
				{
					auto& v = (lights[i] != nullptr) ? lights[i]->m_ambient : Color::Transparency;		// TODO: デフォルト値は？
					m_tempBufferWriter.write(&v, sizeof(Color));
				}
				varInfo.variable->setVectorArray((const Vector4*)m_tempBuffer.getBuffer(), DynamicLightInfo::MaxLights);
			}
			break;
		case BuiltinSemantics::LightSpeculars:
			if (lights != nullptr)
			{
				m_tempBufferWriter.seek(0, SeekOrigin_Begin);
				for (int i = 0; i < DynamicLightInfo::MaxLights; i++)
				{
					auto& v = (lights[i] != nullptr) ? lights[i]->m_specular : Color::Black;		// TODO: デフォルト値は？
					m_tempBufferWriter.write(&v, sizeof(Color));
				}
				varInfo.variable->setVectorArray((const Vector4*)m_tempBuffer.getBuffer(), DynamicLightInfo::MaxLights);
			}
			break;
#endif

            case BuiltinSemantics::BoneTextureReciprocalSize:
				if (info.boneTexture)
					varInfo.variable->setVector(Vector4(1.0f / info.boneTexture->width(), 1.0f / info.boneTexture->height(), 0, 0));
				else
					varInfo.variable->setVector(Vector4::Zero);
                break;
            case BuiltinSemantics::BoneTexture:
                varInfo.variable->setTexture(info.boneTexture);
                break;
            case BuiltinSemantics::BoneLocalQuaternionTexture:
                varInfo.variable->setTexture(info.boneLocalQuaternionTexture);
                break;
            default:
                break;
        }
    }
}

void ShaderSemanticsManager::updateSubsetVariables(const SubsetInfo& info)
{
    for (const VariableKindPair& varInfo : m_subsetVariables) {
        switch (varInfo.kind) {
            case BuiltinSemantics::MaterialTexture:
                varInfo.variable->setTexture(info.materialTexture);
                break;

                // TODO: 以下、グループ化して 別の .fxh に分けておけば、include しないシェーダは全部この for 回さずパフォーマンス上げられそう
            case BuiltinSemantics::ColorScale: {
                Color c = info.colorScale;
                c.a *= info.opacity;
                varInfo.variable->setVector(c.toVector4());
                break;
            }
            case BuiltinSemantics::BlendColor:
                varInfo.variable->setVector(info.blendColor.toVector4());
                break;
            case BuiltinSemantics::ToneColor:
                varInfo.variable->setVector(info.tone.toVector4());
                break;
            default:
                break;
        }
    }
}

void ShaderSemanticsManager::updateSubsetVariables_PBR(const PbrMaterialData& materialData)
{
    for (const VariableKindPair& varInfo : m_subsetVariables) {
        switch (varInfo.kind) {
            case BuiltinSemantics::MaterialColor:
                varInfo.variable->setVector(materialData.color.toVector4());
                break;
            case BuiltinSemantics::MaterialRoughness:
                varInfo.variable->setFloat(materialData.roughness);
                break;
            case BuiltinSemantics::MaterialMetallic:
                varInfo.variable->setFloat(materialData.metallic);
                break;
                //case BuiltinSemantics::MaterialSpecular:
                //	varInfo.variable->setFloat(materialData.specular);
                //	break;
            case BuiltinSemantics::MaterialEmissive:
                varInfo.variable->setVector(materialData.emissive.toVector4());
                break;
        }
    }
}

void ShaderSemanticsManager::updateSubsetVariables_Phong(const PhongMaterialData& materialData)
{
    for (const VariableKindPair& varInfo : m_subsetVariables) {
        switch (varInfo.kind) {
            case BuiltinSemantics::PhongMaterialDiffuse:
                varInfo.variable->setVector(materialData.diffuse.toVector4());
                break;
            case BuiltinSemantics::PhongMaterialAmbient:
                varInfo.variable->setVector(materialData.ambient.toVector4());
                break;
            case BuiltinSemantics::PhongMaterialEmmisive:
                varInfo.variable->setVector(materialData.emissive.toVector4());
                break;
            case BuiltinSemantics::PhongMaterialSpecularColor:
                varInfo.variable->setVector(materialData.specular.toVector4());
                break;
            case BuiltinSemantics::PhongMaterialSpecularPower:
                varInfo.variable->setFloat(materialData.power);
                break;
        }
    }
}

ShaderParameter* ShaderSemanticsManager::getParameterBySemantics(BuiltinSemantics semantics) const
{
    return m_variablesTable[(int)semantics];
}

//=============================================================================
// ShaderTechniqueClass

void ShaderTechniqueClass::parseTechniqueClassString(const String& str, ShaderTechniqueClass* outClassSet)
{
    outClassSet->defaultTechnique = false;
    outClassSet->ligiting = ShaderTechniqueClass_Ligiting::Forward;
    outClassSet->phase = ShaderTechniqueClass_Phase::Geometry;
    outClassSet->meshProcess = ShaderTechniqueClass_MeshProcess::StaticMesh;
    outClassSet->shadingModel = ShaderTechniqueClass_ShadingModel::Default;
    outClassSet->drawMode = ShaderTechniqueClass_DrawMode::Primitive;
    if (String::compare(str, u"Default", CaseSensitivity::CaseInsensitive) == 0)
    {
        outClassSet->defaultTechnique = true;
    }
	else if (String::compare(str, u"LightDisc", CaseSensitivity::CaseInsensitive) == 0)
	{
		outClassSet->ligiting = ShaderTechniqueClass_Ligiting::LightDisc;
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
    return a.ligiting == b.ligiting &&
           a.phase == b.phase &&
           a.meshProcess == b.meshProcess &&
           a.shadingModel == b.shadingModel &&
           a.drawMode == b.drawMode;
}

} // namespace detail
} // namespace ln
