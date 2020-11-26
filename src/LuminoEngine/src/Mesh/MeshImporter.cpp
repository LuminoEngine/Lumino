
#include "Internal.hpp"
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include "MeshManager.hpp"
#include "MeshImporter.hpp"

namespace ln {
namespace detail {

MeshImporter::MeshImporter()
	: m_applyBoneTransformationsEnabled(true)
	//, m_flipZCoordinate(true)
{
}

void MeshImporter::prepare(MeshManager* meshManager, DiagnosticsManager* diag)
{
	m_meshManager = meshManager;
	m_assetManager = m_meshManager->assetManager();
	m_diag = diag;
}

void MeshImporter::applySettings(const MeshImportSettings* settings)
{
	LN_DCHECK(settings);

	m_applyBoneTransformationsEnabled = settings->applyBoneTransformationsEnabled().valueOr(m_applyBoneTransformationsEnabled);
}

} // namespace detail
} // namespace ln
