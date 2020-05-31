
#ifdef LN_USE_FBX_IMPORTER
#include "Internal.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "../Asset/AssetManager.hpp"
#include "FbxImporter.hpp"

namespace ln {
namespace detail {

Ref<StaticMeshModel> FbxImporter::import(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag)
{
	return nullptr;
}

Ref<SkinnedMeshModel> FbxImporter::importSkinnedMesh(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag)
{
	return nullptr;
}

} // namespace detail
} // namespace ln

#endif // LN_USE_FBX_IMPORTER
