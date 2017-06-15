
#include "../Internal.h"
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// StaticMeshComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(StaticMeshComponent, VisualComponent);

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::create(StaticMeshModel* staticMeshModel)
{
	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(staticMeshModel);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::create(const StringRef& filePath)
{
	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	auto mesh = SceneGraphManager::Instance->getModelManager()->createStaticMeshModel(filePath);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createPlane(const Vector2& size, int sliceH, int sliceV, MeshCreationFlags flags)
{
	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initializePlane(SceneGraphManager::Instance->getGraphicsManager(), size, sliceH, sliceV, flags);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createScreenPlane()
{
	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initializeScreenPlane(SceneGraphManager::Instance->getGraphicsManager(), MeshCreationFlags::None);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createBox()
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->addMeshResource(detail::ModelManager::getInstance()->getUnitBoxMeshResource(MeshCreationFlags::None));
	mesh->addMaterial(detail::ModelManager::getInstance()->getDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createBox(const Vector3& size)
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initializeBox(SceneGraphManager::Instance->getGraphicsManager(), size, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createBox(float width, float height, float depth)
{
	return createBox(Vector3(width, height, depth));
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createSphere()
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->addMeshResource(detail::ModelManager::getInstance()->getUnitSphereMeshResource(MeshCreationFlags::None));
	mesh->addMaterial(detail::ModelManager::getInstance()->getDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createSphere(float radius, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initializeSphere(SceneGraphManager::Instance->getGraphicsManager(), radius, tessellation, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createTeapot()
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initialize(SceneGraphManager::Instance->getGraphicsManager());
	mesh->addMeshResource(detail::ModelManager::getInstance()->getUnitTeapotMeshResource());
	mesh->addMaterial(detail::ModelManager::getInstance()->getDefaultMaterial());

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
RefPtr<StaticMeshComponent> StaticMeshComponent::createTeapot(float size, int tessellation)
{
	auto mesh = RefPtr<StaticMeshModel>::makeRef();
	mesh->initializeTeapot(SceneGraphManager::Instance->getGraphicsManager(), size, tessellation, MeshCreationFlags::None);

	auto ptr = RefPtr<StaticMeshComponent>::makeRef();
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
	if (LN_CHECK_ARG(meshModel != nullptr)) return;
	m_mesh = meshModel;

	VisualComponent::initialize();

	//m_materialList->copyShared(m_mesh->m_materials, true);


	//owner->getRootNode()->addChild(this);
	setAutoRemove(true);
}

//------------------------------------------------------------------------------
void StaticMeshComponent::onRender2(DrawList* renderer)
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
RefPtr<Rectangle> Rectangle::create(const Rect& rect)
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
void Rectangle::onRender2(DrawList* renderer)
{
	renderer->drawSquarePrimitive(
		Vector3(m_rect.getTopLeft(), 0), Vector2(0, 0), Color::White,
		Vector3(m_rect.getTopRight(), 0), Vector2(1, 0), Color::White,
		Vector3(m_rect.getBottomLeft(), 0), Vector2(0, 1), Color::White,
		Vector3(m_rect.getBottomRight(), 0), Vector2(1, 1), Color::White);
}

LN_NAMESPACE_END
