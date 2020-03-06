
#include "Internal.hpp"
#include <LuminoEngine/Scene/MeshTilemap/MeshTilemapModel.hpp>

#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>

namespace ln {

//==============================================================================
// MeshTilemapModel

LN_OBJECT_IMPLEMENT(MeshTilemapModel, Object) {}

MeshTilemapModel::MeshTilemapModel()
{
}

void MeshTilemapModel::init()
{
    Object::init();

	m_mesh = makeObject<Mesh>(4, 6);
	m_mesh->setVertex(0, Vertex{ Vector3(0, 0.5, 0), Vector3::UnitZ, Vector2(0, 0), Color::White });
	m_mesh->setVertex(1, Vertex{ Vector3(0.5, 0.5, 0), Vector3::UnitZ, Vector2(1, 0), Color::White });
	m_mesh->setVertex(2, Vertex{ Vector3(0, 0, 0), Vector3::UnitZ, Vector2(0, 1), Color::White });
	m_mesh->setVertex(3, Vertex{ Vector3(0.5, 0, 0), Vector3::UnitZ, Vector2(1, 1), Color::White });
	m_mesh->setIndex(0, 0);
	m_mesh->setIndex(1, 1);
	m_mesh->setIndex(2, 2);
	m_mesh->setIndex(3, 2);
	m_mesh->setIndex(4, 1);
	m_mesh->setIndex(5, 3);
	m_mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);

	auto tex = Texture2D::load(u"autotile1");
	m_material = Material::create(tex);
}

void MeshTilemapModel::draw(RenderingContext* context, const Matrix& transform)
{
    context->setMaterial(m_material);
    context->drawMesh(m_mesh, 0);
}

} // namespace ln

