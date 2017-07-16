
#include "../Internal.h"
#include <float.h>
#include "SceneGraphManager.h"
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/SceneNode.h>
#include <Lumino/Scene/WorldObject.h>
#include <Lumino/Framework/GameScene.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// ListObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ListObject, Object);


//==============================================================================
// SceneNodeRefList
//==============================================================================
//LN_REF_OBJECT_LIST_IMPL(SceneNodeRefList, SceneNode);
//
//
//SceneNodeRefList::iterator::reference SceneNodeRefList::iterator::operator*() const { return static_cast<reference>(*m_internalItr); }
//SceneNodeRefList::iterator::reference SceneNodeRefList::iterator::operator[](difference_type offset) const { return static_cast<reference>(m_internalItr[offset]); }

//==============================================================================
// SceneNode
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SceneNode, Component);

//------------------------------------------------------------------------------
SceneNode::SceneNode()
	: m_manager(nullptr)
	, m_name()
	//, m_localMatrix()
	//, m_transform()
	//, m_transformCenter()
	//, m_rotOrder(RotationOrder::XYZ)
	, m_priority(DepthPriority::Normal)
	, m_billboardType(BillboardType::None)
	, m_renderingMode(SceneNodeRenderingMode::Visible)
	//, m_transformModified(true)
	, m_isAutoUpdate(false)
	, m_isAutoRemove(false)
	, m_children(Ref<SceneNodeList>::makeRef())
	, m_parentNode(NULL)
	, m_isVisible(true)
{
//	memset(m_renderingPassClientDataList, 0, sizeof(m_renderingPassClientDataList));
}

//------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
	//if (m_ownerSceneGraph != nullptr) {
	//	m_ownerSceneGraph->RemoveNode(this);
	//}
	//LN_SAFE_RELEASE(m_manager);
}

//------------------------------------------------------------------------------
void SceneNode::initialize()
{
	m_manager = detail::EngineDomain::getSceneGraphManager();
}

//------------------------------------------------------------------------------
void SceneNode::setName(const String& name)
{
	if (m_name != name)
	{
		m_manager->onNodeRename(this, m_name, name);
		m_name = name;
	}
}

//------------------------------------------------------------------------------
void SceneNode::addChild(SceneNode* child)
{
	if (LN_CHECK_ARG(child != nullptr)) return;

	// 別のノードの子であれば外す
	// ※ WPF などでは既に別ノードの子であれば例外するが、この SceneGraph ではしない。
	//    ノードは作成と同時にツリーに追加されるため、別ノードに addChild したいときは一度 Remove しなければならないがさすがに煩わしい。
	if (child->m_parentNode != nullptr) {
		removeChild(child);
	}

	// 子として追加
	m_children->add(child);
	child->m_parentNode = this;
}

//------------------------------------------------------------------------------
void SceneNode::removeChild(SceneNode* child)
{
	if (LN_CHECK_ARG(child != nullptr)) return;
	if (child->m_parentNode == this)
	{
		m_children->remove(child);
		child->m_parentNode = nullptr;
	}
}

//------------------------------------------------------------------------------
void SceneNode::updateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	// ワールド行列の更新が必要な場合は再計算
	//if (m_transformModified)
	//{
	//	m_localMatrix = Matrix::Identity;
	//	m_localMatrix.translate(-m_transformCenter.x, -m_transformCenter.y, -m_transformCenter.z);
	//	m_localMatrix.scale(m_transform.scale);
	//	m_localMatrix.rotateQuaternion(m_transform.rotation);
	//	m_localMatrix.translate(m_transform.translation);
	//	m_transformModified = false;
	//}
	//Matrix localMatrix;
	//localMatrix.translate(-m_transformCenter.x, -m_transformCenter.y, -m_transformCenter.z);
	//localMatrix.scale(m_transform.scale);
	//localMatrix.rotateQuaternion(m_transform.rotation);
	//localMatrix.translate(m_transform.translation);

	// グローバル行列結合
	//if (parent != nullptr)
	//	m_combinedGlobalMatrix = localMatrix * parent->m_combinedGlobalMatrix;
	//else
	//	m_combinedGlobalMatrix = localMatrix;

	//// Component
	//WorldObject* owner = getOwnerObject();
	//if (owner != nullptr)
	//{
	//	m_combinedGlobalMatrix *= owner->transform.getTransformMatrix();
	//}

	onUpdateFrame(deltaTime);

	// 子ノード更新
	int count = m_children->getCount();
	for (int i = 0; i < count; )
	{
		SceneNode* node = m_children->getAt(i);
		node->updateFrameHierarchy(this, deltaTime);

		if (node->isAutoRemove() && node->getReferenceCount() == 1)
		{
			m_children->removeAt(i);
			count = m_children->getCount();
		}
		else
		{
			++i;
		}
	}
}

//------------------------------------------------------------------------------
void SceneNode::onRender2(RenderingContext* renderer)
{
}

//------------------------------------------------------------------------------
void SceneNode::onUpdate()
{
	Component::onUpdate();
}

//==============================================================================
// SceneNodeList
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SceneNodeList, ObjectList<SceneNode*>);

//------------------------------------------------------------------------------
SceneNodeList::SceneNodeList()
{
}

//------------------------------------------------------------------------------
SceneNodeList::~SceneNodeList()
{
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
