
#include "Internal.hpp"
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Mesh/MeshModel.hpp>
#include <LuminoGraphics/Mesh/MeshModelFactory.hpp>
#include <LuminoEngine/Scene/Shapes/TeapotMesh.hpp>

namespace ln {

//==============================================================================
// TeapotMesh

TeapotMesh::TeapotMesh() = default;

TeapotMesh::~TeapotMesh() = default;

bool TeapotMesh::init(Material* material)
{
	auto model = detail::MeshModelFactory::createTeapot(material);
	if (!StaticMesh::init(model)) return false;
	return true;
}

//==============================================================================
// TeapotMesh::BuilderDetails

TeapotMesh::BuilderDetails::BuilderDetails()
	: material(nullptr)
{
}

Ref<Object> TeapotMesh::BuilderDetails::create() const
{
	auto p = makeObject<TeapotMesh>(material);
	apply(p);
	return p;
}

} // namespace ln

