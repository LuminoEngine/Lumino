
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
	mesh->initializePlane(SceneGraphManager::Instance->getGraphicsManager(), size, sliceH, sliceV, flags);
	ptr->initialize(mesh);
	return ptr;
}

//------------------------------------------------------------------------------
Ref<StaticMeshComponent> StaticMeshComponent::createScreenPlane()
{
	auto ptr = Ref<StaticMeshComponent>::makeRef();
	auto mesh = Ref<StaticMeshModel>::makeRef();
	mesh->initializeScreenPlane(SceneGraphManager::Instance->getGraphicsManager(), MeshCreationFlags::None);
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
	mesh->initializeBox(SceneGraphManager::Instance->getGraphicsManager(), size, MeshCreationFlags::None);

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
	mesh->initializeSphere(SceneGraphManager::Instance->getGraphicsManager(), radius, tessellation, tessellation, MeshCreationFlags::None);

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
	mesh->initializeTeapot(SceneGraphManager::Instance->getGraphicsManager(), size, tessellation, MeshCreationFlags::None);

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
bool StaticMeshComponent::initialize(StaticMeshModel* meshModel)
{
	LN_BASE_INITIALIZE(VisualComponent);
	if (LN_CHECK_ARG(meshModel != nullptr)) return false;
	m_mesh = meshModel;

	//m_materialList->copyShared(m_mesh->m_materials, true);


	//owner->getRootNode()->addChild(this);
	setAutoRemove(true);
	return true;
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
bool Rectangle::initialize(const Rect& rect)
{
	LN_BASE_INITIALIZE(VisualComponent);
	detail::EngineDomain::getDefaultSceneGraph3D()->getRootNode()->addChild(this);
	setAutoRemove(true);

	m_rect = rect;
	return true;
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

LN_NAMESPACE_END
