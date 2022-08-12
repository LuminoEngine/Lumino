
#include "Internal.hpp"
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Mesh/MeshModel.hpp>
#include <LuminoGraphics/Mesh/MeshModelFactory.hpp>
#include <LuminoEngine/Scene/Shapes/CylinderMesh.hpp>

namespace ln {

//==============================================================================
// CylinderMesh

CylinderMesh::CylinderMesh() = default;

CylinderMesh::~CylinderMesh() = default;

bool CylinderMesh::init(float radiusTop, float radiusBottom, float height, float segments, Material* material)
{
	auto model = detail::MeshModelFactory::createCylinder(radiusTop, radiusBottom, height, segments, material);
	if (!StaticMesh::init(model)) return false;
	return true;
}

//==============================================================================
// CylinderMesh::BuilderDetails

CylinderMesh::BuilderDetails::BuilderDetails()
	: radiusTop(0.5f)
	, radiusBottom(0.5f)
	, height(1.0f)
	, segments(16)
	, material(nullptr)
{
}

Ref<Object> CylinderMesh::BuilderDetails::create() const
{
	auto p = makeObject_deprecated<CylinderMesh>(radiusTop, radiusBottom, height, segments, material);
	apply(p);
	return p;
}

} // namespace ln

