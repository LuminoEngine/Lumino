
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Mesh/MeshModelFactory.hpp>
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

