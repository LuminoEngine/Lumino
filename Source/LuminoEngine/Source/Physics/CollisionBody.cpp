
#include "Internal.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletDynamicsCommon.h>
#include <Lumino/Physics/CollisionShape.h>
#include <Lumino/Physics/PhysicsWorld.h>
#include <Lumino/Physics/CollisionBody.h>
#include "BulletUtils.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// BtShapeManager
//		各種 Body にアタッチする btCollisionShape を効率よく管理するためのクラス。
//		Bullet は、Shape が姿勢のオフセットを持つことはできない。
//		持たせたければ btCompoundShape を経なければならない。
//		常に btCompoundShape を作ってしまえばいいのだがそれだとメモリや演算の効率が悪い。
//		そこで、姿勢のオフセットを持たない単一の Shape しか使わなければそれを、
//		そうでなければ　btCompoundShape を作る。
//==============================================================================

//------------------------------------------------------------------------------
BtShapeManager::BtShapeManager()
	: m_collisionShape()
	, m_btCompoundShape(nullptr)
	, m_activeShape(nullptr)
	, m_dirty(false)
{

}

//------------------------------------------------------------------------------
BtShapeManager::~BtShapeManager()
{
	LN_SAFE_DELETE(m_btCompoundShape);
}

//------------------------------------------------------------------------------
void BtShapeManager::AddShape(CollisionShape* shape)
{
	if (LN_CHECK_ARG(shape != nullptr)) return;

	if (m_collisionShape != nullptr)
	{
		// TODO: 今は1つのみ許可
		LN_NOTIMPLEMENTED();
	}
	else
	{
		m_collisionShape = shape;
	}

	m_dirty = true;
}

//------------------------------------------------------------------------------
bool BtShapeManager::IsEmpty() const
{
	return m_collisionShape == nullptr;
}

//------------------------------------------------------------------------------
btCollisionShape* BtShapeManager::GetBtCollisionShape()
{
	if (m_dirty)
	{
		Refresh();
		m_dirty = false;
	}
	return m_activeShape;
}

//------------------------------------------------------------------------------
void BtShapeManager::Refresh()
{
	btCollisionShape* shape;
	if (m_collisionShape->GetCenter() != Vector3::Zero)
	{
		if (m_btCompoundShape == nullptr)
		{
			m_btCompoundShape = new btCompoundShape();
		}
		else
		{
			for (int i = m_btCompoundShape->getNumChildShapes() - 1; i >= 0; i--)
			{
				m_btCompoundShape->removeChildShapeByIndex(i);
			}
		}

		btTransform t;
		t.setBasis(btMatrix3x3::getIdentity());
		t.setOrigin(BulletUtil::LNVector3ToBtVector3(m_collisionShape->GetCenter()));
		m_btCompoundShape->addChildShape(t, m_collisionShape->GetBtCollisionShape());

		shape = m_btCompoundShape;
	}
	else
	{
		shape = m_collisionShape->GetBtCollisionShape();
	}

	m_activeShape = shape;
}

} // namespace detail



//==============================================================================
// LocalGhostObject
//	btGhostObject の 衝突開始/終了 をフックして CollisionBody へ通知する
//==============================================================================
class CollisionBody::LocalGhostObject : public btGhostObject
{
public:
	CollisionBody*	m_owner;

	LocalGhostObject(CollisionBody* owner)
		: m_owner(owner)
	{}

	virtual ~LocalGhostObject()
	{}

	virtual void addOverlappingObjectInternal(btBroadphaseProxy* otherProxy, btBroadphaseProxy* thisProxy = 0) override
	{
		btCollisionObject* otherObject = (btCollisionObject*)otherProxy->m_clientObject;
		btAssert(otherObject);
		int index = m_overlappingObjects.findLinearSearch(otherObject);
		if (index == m_overlappingObjects.size())
		{
			//not found
			m_overlappingObjects.push_back(otherObject);

			// 通知
			if (m_owner->IsTrigger() && otherObject->getUserPointer() != nullptr)
			{
				m_owner->m_contactObjects.Add(reinterpret_cast<PhysicsObject*>(otherObject->getUserPointer()));
				m_owner->OnTriggerEnter(reinterpret_cast<PhysicsObject*>(otherObject->getUserPointer()));
			}
		}
	}

	virtual void removeOverlappingObjectInternal(btBroadphaseProxy* otherProxy, btDispatcher* dispatcher, btBroadphaseProxy* thisProxy = 0) override
	{
		btCollisionObject* otherObject = (btCollisionObject*)otherProxy->m_clientObject;
		btAssert(otherObject);
		int index = m_overlappingObjects.findLinearSearch(otherObject);
		if (index < m_overlappingObjects.size())
		{
			m_overlappingObjects[index] = m_overlappingObjects[m_overlappingObjects.size() - 1];
			m_overlappingObjects.pop_back();

			// 通知
			if (m_owner->IsTrigger() && otherObject->getUserPointer() != nullptr)
			{
				m_owner->m_contactObjects.Remove(reinterpret_cast<PhysicsObject*>(otherObject->getUserPointer()));
				m_owner->OnTriggerLeave(reinterpret_cast<PhysicsObject*>(otherObject->getUserPointer()));
			}
		}
	}
};

//==============================================================================
// CollisionBody
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CollisionBody, PhysicsObject);

//------------------------------------------------------------------------------
RefPtr<CollisionBody> CollisionBody::Create(CollisionShape* shape)
{
	auto ptr = RefPtr<CollisionBody>::MakeRef();
	ptr->initialize();
	ptr->AddShape(shape);
	return ptr;
}

//------------------------------------------------------------------------------
CollisionBody::CollisionBody()
	: PhysicsObject()
	, m_btGhostObject(nullptr)
	, m_isTrigger(false)
	, m_initialUpdate(false)
{
}

//------------------------------------------------------------------------------
CollisionBody::~CollisionBody()
{
	DeleteInternalObject();
}

//------------------------------------------------------------------------------
void CollisionBody::initialize()
{
	PhysicsObject::initialize();
	m_initialUpdate = true;
	detail::EngineDomain::GetPhysicsWorld3D()->AddPhysicsObject(this);
}

//------------------------------------------------------------------------------
//const Matrix& CollisionBody::GetTransform() const
//{
//	return m_transform;
//}

//------------------------------------------------------------------------------
void CollisionBody::AddShape(CollisionShape* shape)
{
	m_btShapeManager.AddShape(shape);
}

//------------------------------------------------------------------------------
void CollisionBody::SetTrigger(bool enabled)
{
	m_isTrigger = enabled;
}

//------------------------------------------------------------------------------
bool CollisionBody::IsTrigger() const
{
	return m_isTrigger;
}


//------------------------------------------------------------------------------
EventConnection CollisionBody::ConnectOnTriggerEnter(CollisionEventHandler handler)
{
	return onTriggerEnter.Connect(handler);
}

//------------------------------------------------------------------------------
EventConnection CollisionBody::ConnectOnTriggerLeave(CollisionEventHandler handler)
{
	return onTriggerLeave.Connect(handler);
}

//------------------------------------------------------------------------------
EventConnection CollisionBody::ConnectOnTriggerStay(CollisionEventHandler handler)
{
	return onTriggerStay.Connect(handler);
}

//------------------------------------------------------------------------------
void CollisionBody::OnBeforeStepSimulation()
{
	if (m_initialUpdate)
	{
		CreateInternalObject();
		m_initialUpdate = false;
	}

	// btCollisionObject::CF_NO_CONTACT_RESPONSE が付加されると、
	// 他のオブジェクトと物理シミュレーションで接触しないことを示す (すり抜ける)
	int flags = m_btGhostObject->getCollisionFlags();
	m_btGhostObject->setCollisionFlags(
		m_isTrigger ?
		flags | btCollisionObject::CF_NO_CONTACT_RESPONSE :
		flags & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// set shape
	btCollisionShape* shape = m_btShapeManager.GetBtCollisionShape();
	if (m_btGhostObject->getCollisionShape() != shape)
	{
		m_btGhostObject->setCollisionShape(shape);
	}
	

	auto* transform = GetTransform();
	if (transform != nullptr)
	{
		btTransform t;
		t.setFromOpenGLMatrix((btScalar*)&transform->GetWorldMatrix());
		m_btGhostObject->setWorldTransform(t);
	}
}

//------------------------------------------------------------------------------
void CollisionBody::OnAfterStepSimulation()
{
	// get transform
	m_btGhostObject->getWorldTransform().getOpenGLMatrix((btScalar*)&m_transform);

	// notify OnTriggerStay
	if (IsTrigger())
	{
		int count = m_btGhostObject->getNumOverlappingObjects();
		for (int i = 0; i < count; i++)
		{
			btCollisionObject* btObj = m_btGhostObject->getOverlappingObject(i);
			OnTriggerStay(reinterpret_cast<PhysicsObject*>(btObj->getUserPointer()));
		}
	}
}

//------------------------------------------------------------------------------
void CollisionBody::OnRemovedFromWorld()
{
	// Bullet の World に追加するのはこのクラスの役目なので、除外もここで行う。
	if (m_btGhostObject != nullptr)
	{
		GetOwnerWorld()->GetBtWorld()->removeCollisionObject(m_btGhostObject);
	}
}

//------------------------------------------------------------------------------
void CollisionBody::OnTriggerEnter(PhysicsObject* otherObject)
{
	onTriggerEnter.Raise(otherObject);
}

//------------------------------------------------------------------------------
void CollisionBody::OnTriggerLeave(PhysicsObject* otherObject)
{
	onTriggerLeave.Raise(otherObject);
}

//------------------------------------------------------------------------------
void CollisionBody::OnTriggerStay(PhysicsObject* otherObject)
{
	onTriggerStay.Raise(otherObject);
}

//------------------------------------------------------------------------------
void CollisionBody::CreateInternalObject()
{
	DeleteInternalObject();

	// 現状、この時点で必ず m_shape が無ければならない。
	LN_ASSERT(!m_btShapeManager.IsEmpty());

	m_btGhostObject = new LocalGhostObject(this);
	m_btGhostObject->setCollisionShape(m_btShapeManager.GetBtCollisionShape());
	m_btGhostObject->setUserPointer(this);

	GetOwnerWorld()->GetBtWorld()->addCollisionObject(m_btGhostObject, GetCollisionFilterGroup(), GetCollisionFilterMask());
}

//------------------------------------------------------------------------------
void CollisionBody::DeleteInternalObject()
{
	LN_SAFE_DELETE(m_btGhostObject);
}

LN_NAMESPACE_END
