
#include "../Internal.h"
#include <Lumino/Rendering/RenderingContext.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// StaticMeshComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(StaticMeshComponent, VisualComponent);

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::create(StaticMeshModel* staticMeshModel)
{
	auto ptr = Ref<StaticMeshComponent>::makeRef();
	ptr->initialize(staticMeshModel);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::create(const StringRef& filePath)
{
	auto ptr = Ref<StaticMeshComponent>::makeRef();
	auto mesh = SceneGraphManager::Instance->getModelManager()->createStaticMeshModel(filePath);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createPlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto ptr = Ref<StaticMeshComponent>::makeRef();
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initializePlane(size, sliceH, sliceV, flags);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createScreenPlane()
{
	auto ptr = Ref<StaticMeshComponent>::makeRef();
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initializeScreenPlane(MeshCreationFlags::None);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createBox()
{
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->addMeshResource(detail::ModelManager::getInstance()->getUnitBoxMeshResource(MeshCreationFlags::None));
	mesh->addMaterial(detail::ModelManager::getInstance()->getDefaultMaterial());

	auto ptr = Ref<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createBox(const Vector3& size)
{
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initializeBox(size, MeshCreationFlags::None);

	auto ptr = Ref<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createBox(float width, float height, float depth)
{
	return createBox(Vector3(width, height, depth));
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createSphere()
{
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->addMeshResource(detail::ModelManager::getInstance()->getUnitSphereMeshResource(MeshCreationFlags::None));
	mesh->addMaterial(detail::ModelManager::getInstance()->getDefaultMaterial());

	auto ptr = Ref<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createSphere(float radius, int tessellation)
{
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initializeSphere(radius, tessellation, tessellation, MeshCreationFlags::None);

	auto ptr = Ref<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createTeapot()
{
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->addMeshResource(detail::ModelManager::getInstance()->getUnitTeapotMeshResource());
	mesh->addMaterial(detail::ModelManager::getInstance()->getDefaultMaterial());

	auto ptr = Ref<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createTeapot(float size, int tessellation)
{
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initializeTeapot(size, tessellation, MeshCreationFlags::None);

	auto ptr = Ref<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
StaticMeshComponent::StaticMeshComponent()
	: m_mesh()
{
}

//------------------------------------------------------------------------------
StaticMeshComponent::~StaticMeshComponent()
{
}

//------------------------------------------------------------------------------
void StaticMeshComponent::initialize(StaticMeshModel* meshModel)
{
	if (LN_REQUIRE(meshModel != nullptr)) return;
	m_mesh = meshModel;

	VisualComponent::initialize();

	//m_materialList->copyShared(m_mesh->m_materials, true);


	//owner->getRootNode()->addChild(this);
	setAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMeshComponent::onRender2(RenderingContext* renderer)
{
	renderer->setTransform(getOwnerObject()->transform.getWorldMatrix());

	for (int iMesh = 0; iMesh < m_mesh->getMeshResourceCount(); iMesh++)
	{
		MeshResource* m = m_mesh->getMeshResource(iMesh);
		for (int i = 0; i < m->getSubsetCount(); i++)
		{
			MeshAttribute attr;
			m->getMeshAttribute(i, &attr);
			renderer->drawMesh(m, i, m_mesh->getMaterial(attr.MaterialIndex));
			//renderer->drawMesh(m, i, GetMaterials()->GetAt(i));
		}
	}

}

//==============================================================================
// Rectangle
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Rectangle, VisualComponent);

//------------------------------------------------------------------------------
Ref<Rectangle> Rectangle::create(const Rect& rect)
{
	return newObject<Rectangle>(rect);
}

//------------------------------------------------------------------------------
Rectangle::Rectangle()
{
}

//------------------------------------------------------------------------------
Rectangle::~Rectangle()
{
}

//------------------------------------------------------------------------------
void Rectangle::initialize(const Rect& rect)
{
	VisualComponent::initialize();
	detail::EngineDomain::getDefaultSceneGraph3D()->getRootNode()->addChild(this);
	setAutoRemove(true);

	m_rect = rect;
}

//------------------------------------------------------------------------------
void Rectangle::onRender2(RenderingContext* renderer)
{
	renderer->drawSquarePrimitive(
		Vector3(m_rect.getTopLeft(), 0), Vector2(0, 0), Color::White,
		Vector3(m_rect.getTopRight(), 0), Vector2(1, 0), Color::White,
		Vector3(m_rect.getBottomLeft(), 0), Vector2(0, 1), Color::White,
		Vector3(m_rect.getBottomRight(), 0), Vector2(1, 1), Color::White);
}

//==============================================================================
// CornellBox
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CornellBox, StaticMeshComponent);

Ref<CornellBox> CornellBox::create()
{
	return newObject<CornellBox>();
}

CornellBox::CornellBox()
{
}

CornellBox::~CornellBox()
{
}

void CornellBox::initialize()
{
	auto boxMesh = MeshResource::create(4, 6, MeshCreationFlags::None);

	// front
	boxMesh->addSquare(
		Vertex{ Vector3(-10, 20, 10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(-10, 0, 10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(10, 0, 10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(10, 20, 10), Vector2(0, 0), Vector3(0, 0, 1), Color::White });
	// up
	boxMesh->addSquare(
		Vertex{ Vector3(-10, 20, -10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(-10, 20, 10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(10, 20, 10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(10, 20, -10), Vector2(0, 0), Vector3(0, 0, 1), Color::White });
	// down
	boxMesh->addSquare(
		Vertex{ Vector3(-10, 0, 10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(-10, 0, -10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(10, 0, -10), Vector2(0, 0), Vector3(0, 0, 1), Color::White },
		Vertex{ Vector3(10, 0, 10), Vector2(0, 0), Vector3(0, 0, 1), Color::White });
	// left
	boxMesh->addSquare(
		Vertex{ Vector3(-10, 20, -10), Vector2(0, 0), Vector3::UnitX, Color::Red },
		Vertex{ Vector3(-10, 0, -10), Vector2(0, 0), Vector3::UnitX, Color::Red },
		Vertex{ Vector3(-10, 0, 10), Vector2(0, 0), Vector3::UnitX, Color::Red },
		Vertex{ Vector3(-10, 20, 10), Vector2(0, 0), Vector3::UnitX, Color::Red });
	// right
	boxMesh->addSquare(
		Vertex{ Vector3(10, 20, 10), Vector2(0, 0), -Vector3::UnitX, Color::Green },
		Vertex{ Vector3(10, 0, 10), Vector2(0, 0), -Vector3::UnitX, Color::Green },
		Vertex{ Vector3(10, 0, -10), Vector2(0, 0), -Vector3::UnitX, Color::Green },
		Vertex{ Vector3(10, 20, -10), Vector2(0, 0), -Vector3::UnitX, Color::Green });


	boxMesh->addBox(Vector3(4, 12, 4), Matrix::makeTranslation(-3, 6, 2));

	boxMesh->addSphere(3, 16, 16, Matrix::makeTranslation(4, 3, 0));

	auto mesh = newObject<StaticMeshModel>(boxMesh);

	auto material = Material::create();
	material->setMaterialTexture(Texture2D::getWhiteTexture());
	mesh->addMaterial(material);
	StaticMeshComponent::initialize(mesh);
}

LN_NAMESPACE_END
