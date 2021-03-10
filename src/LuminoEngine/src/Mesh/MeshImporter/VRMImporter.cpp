

#include "Internal.hpp"
//#define TINYGLTF_IMPLEMENTATION
////#define TINYGLTF_NO_FS
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../build/BuildCache/tinygltf/tiny_gltf.h"
//#include <LuminoEngine/Engine/Diagnostics.hpp>
//#include <LuminoEngine/Graphics/VertexBuffer.hpp>
//#include <LuminoEngine/Graphics/IndexBuffer.hpp>
//#include <LuminoEngine/Graphics/Texture.hpp>
//#include <LuminoEngine/Graphics/Bitmap.hpp>
//#include <LuminoEngine/Rendering/Material.hpp>
//#include <LuminoEngine/Asset/Assets.hpp>
//#include <LuminoEngine/Animation/AnimationCurve.hpp>
//#include <LuminoEngine/Animation/AnimationTrack.hpp>
//#include <LuminoEngine/Animation/AnimationClip.hpp>
//#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
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
	}

	return true;
}

} // namespace detail
} // namespace ln

