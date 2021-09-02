
#pragma once
#ifdef LN_USE_FBX_IMPORTER

#include "MeshImporter.hpp"

namespace ln {
class SkinnedMeshModel;

namespace detail {

class FbxImporter
    : public MeshImporter
{
public:
    Ref<MeshModel> import(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag);
	Ref<SkinnedMeshModel> importSkinnedMesh(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag);

private:
};

} // namespace detail
} // namespace ln

#endif // LN_USE_FBX_IMPORTER
