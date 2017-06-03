
#include "../Internal.h"
#include <float.h>
#include "SceneGraphManager.h"
#include <Lumino/Graphics/Rendering.h>
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
	, m_transform()
	, m_transformCenter()
	, m_rotOrder(RotationOrder::XYZ)
	, m_priority(DepthPriority::Normal)
	, m_billboardType(BillboardType::None)
	, m_renderingMode(SceneNodeRenderingMode::Visible)
	//, m_transformModified(true)
	, m_isAutoUpdate(false)
	, m_isAutoRemove(false)
	, m_children(RefPtr<SceneNodeList>::MakeRef())
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
void SceneNode::Initialize()
{
	m_manager = detail::EngineDomain::GetSceneGraphManager();
}

//------------------------------------------------------------------------------
void SceneNode::SetName(const String& name)
{
	if (m_name != name)
	{
		m_manager->OnNodeRename(this, m_name, name);
		m_name = name;
	}
}

//------------------------------------------------------------------------------
void SceneNode::AddChild(SceneNode* child)
{
	if (LN_CHECK_ARG(child != nullptr)) return;

	// 別のノードの子であれば外す
	// ※ WPF などでは既に別ノードの子であれば例外するが、この SceneGraph ではしない。
	//    ノードは作成と同時にツリーに追加されるため、別ノードに AddChild したいときは一度 Remove しなければならないがさすがに煩わしい。
	if (child->m_parentNode != nullptr) {
		RemoveChild(child);
	}

	// 子として追加
	m_children->Add(child);
	child->m_parentNode = this;
}

//------------------------------------------------------------------------------
void SceneNode::RemoveChild(SceneNode* child)
{
	if (LN_CHECK_ARG(child != nullptr)) return;
	if (child->m_parentNode == this)
	{
		m_children->Remove(child);
		child->m_parentNode = nullptr;
	}
}

//------------------------------------------------------------------------------
void SceneNode::UpdateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	// ワールド行列の更新が必要な場合は再計算
	//if (m_transformModified)
	//{
	//	m_localMatrix = Matrix::Identity;
	//	m_localMatrix.Translate(-m_transformCenter.x, -m_transformCenter.y, -m_transformCenter.z);
	//	m_localMatrix.Scale(m_transform.scale);
	//	m_localMatrix.RotateQuaternion(m_transform.rotation);
	//	m_localMatrix.Translate(m_transform.translation);
	//	m_transformModified = false;
	//}
	Matrix localMatrix;
	localMatrix.Translate(-m_transformCenter.x, -m_transformCenter.y, -m_transformCenter.z);
	localMatrix.Scale(m_transform.scale);
	localMatrix.RotateQuaternion(m_transform.rotation);
	localMatrix.Translate(m_transform.translation);

	// グローバル行列結合
	//if (parent != nullptr)
	//	m_combinedGlobalMatrix = localMatrix * parent->m_combinedGlobalMatrix;
	//else
	//	m_combinedGlobalMatrix = localMatrix;

	//// Component
	//WorldObject* owner = GetOwnerObject();
	//if (owner != nullptr)
	//{
	//	m_combinedGlobalMatrix *= owner->transform.GetTransformMatrix();
	//}

	OnUpdateFrame(deltaTime);

	// 子ノード更新
	int count = m_children->GetCount();
	for (int i = 0; i < count; )
	{
		SceneNode* node = m_children->GetAt(i);
		node->UpdateFrameHierarchy(this, deltaTime);

		if (node->IsAutoRemove() && node->GetReferenceCount() == 1)
		{
			m_children->RemoveAt(i);
			count = m_children->GetCount();
		}
		else
		{
			++i;
		}
	}
}

//------------------------------------------------------------------------------
void SceneNode::OnRender2(DrawList* renderer)
{
}

//------------------------------------------------------------------------------
void SceneNode::OnUpdate()
{
	Component::OnUpdate();
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
