

#include "Internal.hpp"
#include <tiny_gltf.h>
//#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "../../Rendering/RenderingManager.hpp"
#include "../MeshManager.hpp"
#include "VRMImporter.hpp"

namespace ln {
namespace detail {

#define STR(x) String::fromStdString(x)

//==============================================================================
// VRMImporter

VRMImporter::VRMImporter()
	: m_vrm(makeRef<VRM_Extensions>())
{}

bool VRMImporter::onImportAsSkinnedMesh(SkinnedMeshModel* model, const AssetPath& assetPath)
{
	if (!GLTFImporter::onImportAsSkinnedMesh(model, assetPath)) return false;
	
	for (int iMaterial = 0; iMaterial < m_vrm->materialProperties.size(); iMaterial++) {
		const VRM_Material* vrmMaterial = m_vrm->materialProperties[iMaterial];
		Material* material = model->material(iMaterial);
		material->setShader(getShader(vrmMaterial->shader));


		for (const auto& pair : vrmMaterial->floatProperties) {
			if (pair.first == u"_BlendMode") {
				switch ((int)pair.second)
				{
				case 0:// RenderMode::Opaque:
					break;
				case 1:// RenderMode::Cutout:
					break;
				case 2:// RenderMode::Transparent:
					break;
				case 3:// RenderMode::TransparentWithZWrite:
					break;
				default:
					LN_NOTIMPLEMENTED();
					break;
				}
			}
			else if (pair.first == u"_CullMode") {
				switch ((int)pair.second)
				{
				case 0:// CullMode::Off:
					break;
				case 1:// CullMode::Front:
					break;
				case 2:// CullMode::Back:
					break;
				default:
					LN_NOTIMPLEMENTED();
					break;
				}
			}
			else if (pair.first == u"_DebugMode") {
				switch ((int)pair.second)
				{
				case 0:// DebugMode::None:
					break;
				case 1:// DebugMode::Normal:
					break;
				case 2:// DebugMode::LitShadeRate:
					break;
				default:
					LN_NOTIMPLEMENTED();
					break;
				}
			}
			else if (pair.first == u"_DstBlend") {
				switch ((int)pair.second)
				{
				case 0:// UnityEngine.Rendering.BlendMode.Zero
					break;
				case 1:// UnityEngine.Rendering.BlendMode.One
					break;
				case 2:// UnityEngine.Rendering.BlendMode.DstColor
					break;
				case 3:// UnityEngine.Rendering.BlendMode.SrcColor
					break;
				case 4:// UnityEngine.Rendering.BlendMode.OneMinusDstColor
					break;
				case 5:// UnityEngine.Rendering.BlendMode.SrcAlpha
					break;
				case 6:// UnityEngine.Rendering.BlendMode.OneMinusSrcColor
					break;
				case 7:// UnityEngine.Rendering.BlendMode.DstAlpha
					break;
				case 8:// UnityEngine.Rendering.BlendMode.OneMinusDstAlpha
					break;
				case 9:// UnityEngine.Rendering.BlendMode.SrcAlphaSaturate
					break;
				case 10:// UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha
					break;
				default:
					LN_NOTIMPLEMENTED();
					break;
				}
			}
			else if (pair.first == u"_SrcBlend") {
				switch ((int)pair.second)
				{
				case 0:// UnityEngine.Rendering.BlendMode.Zero
					break;
				case 1:// UnityEngine.Rendering.BlendMode.One
					break;
				case 2:// UnityEngine.Rendering.BlendMode.DstColor:
					break;
				case 3:// UnityEngine.Rendering.BlendMode.SrcColor:
					break;
				case 4:// UnityEngine.Rendering.BlendMode.OneMinusDstColor:
					break;
				case 5:// UnityEngine.Rendering.BlendMode.SrcAlpha:
					break;
				case 6:// UnityEngine.Rendering.BlendMode.OneMinusSrcColor:
					break;
				case 7:// UnityEngine.Rendering.BlendMode.DstAlpha:
					break;
				case 8:// UnityEngine.Rendering.BlendMode.OneMinusDstAlpha:
					break;
				case 9:// UnityEngine.Rendering.BlendMode.SrcAlphaSaturate:
					break;
				case 10:// UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha:
					break;
				default:
					LN_NOTIMPLEMENTED();
					break;
				}
			}
			else if (pair.first == u"_OutlineColorMode") {
				// _OutlineLightingMix に影響する
				switch ((int)pair.second)
				{
				case 0:// OutlineColorMode::FixedColor:
					break;
				case 1:// OutlineColorMode::MixedLighting:
					break;
				default:
					LN_NOTIMPLEMENTED();
					break;
				}
			}
			else if (pair.first == u"_OutlineCullMode") {
				// Outline 描画パスでの Cull に影響する
				switch ((int)pair.second)
				{
				case 0:// CullMode::Off:
					break;
				case 1:// CullMode::Front:
					break;
				case 2:// CullMode::Back:
					break;
				default:
					LN_NOTIMPLEMENTED();
					break;
				}
			}
			else if (pair.first == u"_OutlineWidthMode") {
				switch ((int)pair.second)
				{
				case 0:// OutlineWidthMode::None:
					break;
				case 1:// OutlineWidthMode::WorldCoordinates:
					break;
				case 2:// OutlineWidthMode::ScreenCoordinates:
					break;
				default:
					LN_NOTIMPLEMENTED();
					break;
				}
			}
			else if (pair.first == u"_ZWrite") {
				material->setDepthWriteEnabled(pair.second != 0.0f);
			}
			else {
				material->setFloat(pair.first, pair.second);
			}
		}

		for (const auto& pair : vrmMaterial->vectorProperties) {
			if (
				pair.first == u"_EmissionMap" ||
				pair.first == u"_BumpMap" ||
				pair.first == u"_OutlineWidthTexture" ||
				pair.first == u"_MainTex" ||
				pair.first == u"_ReceiveShadowTexture" ||
				pair.first == u"_ShadeTexture" ||
				pair.first == u"_ShadingGradeTexture" ||
				pair.first == u"_SphereAdd" ||
				pair.first == u"_RimTexture" ||
				pair.first == u"_UvAnimMaskTexture") {
				// これらは MToon が要求しているテクスチャで、Vector プロパティはそれぞれ Unity の
				// - Material.SetTextureOffset();
				// - Material.SetTextureScale();
				// に対応する。
				material->setTexture(pair.first, Texture2D::blackTexture());
			}
			else {
				const auto itr = vrmMaterial->textureProperties.find(pair.first);
				if (itr != vrmMaterial->textureProperties.end()) {
					// textureProperties に含まれる名前と同じ VectorProperty は、
					// - Material.SetTextureOffset();
					// - Material.SetTextureScale();
					// に対応する。
					// see: UniVRM/Assets/VRM/Runtime/IO/VRMMaterialImporter.cs
				}
				else {
					const auto& v = pair.second;
					material->setVector(pair.first, Vector4(v[0], v[1], v[2], v[3]));
				}
			}
		}

		for (const auto& pair : vrmMaterial->textureProperties) {
			if (
				pair.first == u"_BumpMap" ||
				pair.first == u"_MetallicGlossMap" ||
				pair.first == u"_OcclusionMap") {
				// これらは検索方法が特殊
				// see: UniVRM/Assets/UniGLTF/Runtime/UniGLTF/IO/TextureLoader/GetTextureParam.cs
				LN_NOTIMPLEMENTED();
			}
			else {
				const auto& te = gltfModel()->textures[pair.second];

				//material->setTexture(pair.first, loadedTextures(te.source));
				material->setTexture(pair.first, loadTexture(te));
				
			}
		}
	}

	return true;
}

bool VRMImporter::onReadMetadata()
{
	const tinygltf::Model* gltfModel = GLTFImporter::gltfModel();

	// VRM
	const auto vrmItr = gltfModel->extensions.find("VRM");
	if (vrmItr != gltfModel->extensions.end())
	{
		const auto& vrm = vrmItr->second;

		// vrm.blendshape
		{
			const auto& blendShapeMaster = vrm.Get("blendShapeMaster");
			const auto& blendShapeGroups = blendShapeMaster.Get("blendShapeGroups");
			const size_t len = blendShapeGroups.ArrayLen();
			for (size_t i = 0; i < len; i++) {
				const auto& group = blendShapeGroups.Get(i);

				auto vrmBlendShapeGroup = makeRef<VRM_BlendShapeGroup>();
				vrmBlendShapeGroup->name = STR(group.Get("name").Get<std::string>());
				vrmBlendShapeGroup->presetName = STR(group.Get("presetName").Get<std::string>());
				vrmBlendShapeGroup->isBinary = group.Get("isBinary").Get<bool>();
				m_vrm->blendShapeMaster->blendShapeGroups.add(vrmBlendShapeGroup);

				{
					for (const auto& bind : group.Get("binds").Get<tinygltf::Value::Array>()) {
						auto vrmBind = makeRef<VRM_BlendShapeBind>();
						const auto& obj = bind.Get<tinygltf::Value::Object>();
						for (const auto& pair : obj) {
							if (pair.first == "mesh") vrmBind->mesh = pair.second.Get<int>();
							else if (pair.first == "index") vrmBind->index = pair.second.Get<int>();
							else if (pair.first == "weight") vrmBind->weight = pair.second.Get<double>();
						}
						vrmBlendShapeGroup->binds.add(vrmBind);
					}
				}

				{
					const auto& values = group.Get("materialValues");
					for (const auto& value : values.Get<tinygltf::Value::Array>()) {
						auto vrmBind = makeRef<VRM_MaterialValueBind>();
						const auto& obj = value.Get<tinygltf::Value::Object>();
						for (const auto& pair : obj) {
							if (pair.first == "materialName") vrmBind->materialName = STR(pair.second.Get<std::string>());
							else if (pair.first == "propertyName") vrmBind->propertyName = STR(pair.second.Get<std::string>());
							else if (pair.first == "targetValues") {
								for (const auto& value : pair.second.Get<tinygltf::Value::Array>()) {
									vrmBind->targetValues.push_back(value.Get<double>());
								}
							}
						}
						vrmBlendShapeGroup->materialValues.add(vrmBind);
					}
				}
			}
		}

		for (const auto& itr : vrm.Get<tinygltf::Value::Object>()) {
			const auto& key = itr.first;
			const auto& value = itr.second;
			if (key == "exporterVersion") {
				m_vrm->exporterVersion = STR(value.Get<std::string>());
			}
			else if (key == "specVersion") {
				m_vrm->specVersion = STR(value.Get<std::string>());
			}
			else if (key == "meta") {
			}
			else if (key == "humanoid") {
			}
			else if (key == "firstPerson") {
			}
			else if (key == "blendShapeMaster") {
			}
			else if (key == "secondaryAnimation") {
			}
			else if (key == "materialProperties") {
				m_vrm->materialProperties = parseMaterialProperties(value);
			}
		}

		
	}

	return true;
}

List<Ref<VRM_Material>> VRMImporter::parseMaterialProperties(const tinygltf::Value& value)
{
	List<Ref<VRM_Material>> result;
	for (const auto& v : value.Get<tinygltf::Value::Array>()) {
		result.add(parseMaterial(v));
	}
	return std::move(result);
}

Ref<VRM_Material> VRMImporter::parseMaterial(const tinygltf::Value& value)
{
	auto result = makeRef<VRM_Material>();
	for (const auto& itr : value.Get<tinygltf::Value::Object>()) {
		const auto& key = itr.first;
		const auto& value = itr.second;

		if (key == "name") {
			result->name = STR(value.Get<std::string>());
		}
		else if (key == "shader") {
			result->shader = STR(value.Get<std::string>());
		}
		else if (key == "renderQueue") {
			result->renderQueue = value.Get<int>();
		}
		else if (key == "floatProperties") {
			for (const auto& pair : value.Get<tinygltf::Value::Object>()) {
				if (pair.second.IsInt())
					result->floatProperties.insert({ STR(pair.first), pair.second.Get<int>() });
				else if (pair.second.IsNumber())
					result->floatProperties.insert({ STR(pair.first), static_cast<float>(pair.second.Get<double>()) });
				else {
					LN_UNREACHABLE();
					return nullptr;
				}
			}
		}
		else if (key == "vectorProperties") {
			for (const auto& pair : value.Get<tinygltf::Value::Object>()) {
				std::vector<float> vec;
				for (const auto& elm : pair.second.Get<tinygltf::Value::Array>()) {
					if (elm.IsInt())
						vec.push_back(static_cast<float>(elm.Get<int>()));
					else if (elm.IsNumber())
						vec.push_back(static_cast<float>(elm.Get<double>()));
					else {
						LN_UNREACHABLE();
						return nullptr;
					}
				}
				result->vectorProperties.insert({ STR(pair.first), std::move(vec) });
			}
		}
		else if (key == "textureProperties") {
			for (const auto& pair : value.Get<tinygltf::Value::Object>()) {
				result->textureProperties.insert({ STR(pair.first), pair.second.Get<int>() });
			}
		}
		else if (key == "keywordMap") {
			for (const auto& pair : value.Get<tinygltf::Value::Object>()) {
				result->keywordMap.insert({ STR(pair.first), pair.second.Get<bool>() });
			}
		}
		else if (key == "tagMap") {
			for (const auto& pair : value.Get<tinygltf::Value::Object>()) {
				result->tagMap.insert({ STR(pair.first),STR(pair.second.Get<std::string>()) });
			}
		}
	}
	return result;
}

Shader* VRMImporter::getShader(const String& name) const
{
	if (name == u"VRM/MToon") {
		return EngineDomain::renderingManager()->builtinShader(BuiltinShader::MToon);
	}
	else {
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

} // namespace detail
} // namespace ln

