
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
void BtShapeManager::addShape(CollisionShape* shape)
{
	if (LN_REQUIRE(shape != nullptr)) return;

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
bool BtShapeManager::isEmpty() const
{
	return m_collisionShape == nullptr;
}

//------------------------------------------------------------------------------
btCollisionShape* BtShapeManager::getBtCollisionShape()
{
	if (m_dirty)
	{
		refresh();
		m_dirty = false;
	}
	return m_activeShape;
}

//------------------------------------------------------------------------------
void BtShapeManager::refresh()
{
	btCollisionShape* shape;
	if (m_collisionShape->getCenter() != Vector3::Zero)
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
		t.setOrigin(BulletUtil::LNVector3ToBtVector3(m_collisionShape->getCenter()));
		m_btCompoundShape->addChildShape(t, m_collisionShape->getBtCollisionShape());

		shape = m_btCompoundShape;
	}
	else
	{
		shape = m_collisionShape->getBtCollisionShape();
	}

	m_activeShape = shape;
}

} // namespace detail

//==============================================================================
// LocalGhostObject
//	btGhostObject の 衝突開始/終了 をフックして CollisionBody へ通知する
//==============================================================================
class CollisionBody2::LocalGhostObject : public btGhostObject
{
public:
	CollisionBody2*	m_owner;

	LocalGhostObject(CollisionBody2* owner)
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
			if (m_owner->isTrigger() && otherObject->getUserPointer() != nullptr)
			{
				m_owner->m_contactObjects.add(reinterpret_cast<PhysicsComponent*>(otherObject->getUserPointer()));
				m_owner->onTriggerEnter(reinterpret_cast<PhysicsComponent*>(otherObject->getUserPointer()));
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
			if (m_owner->isTrigger() && otherObject->getUserPointer() != nullptr)
			{
				m_owner->m_contactObjects.remove(reinterpret_cast<PhysicsComponent*>(otherObject->getUserPointer()));
				m_owner->onTriggerLeave(reinterpret_cast<PhysicsComponent*>(otherObject->getUserPointer()));
			}
		}
	}
};


//==============================================================================
// CollisionBodyComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CollisionBody2, PhysicsObject);

//------------------------------------------------------------------------------
CollisionBody2::CollisionBody2()
	: PhysicsObject()
	, m_btGhostObject(nullptr)
	, m_isTrigger(false)
	, m_initialUpdate(false)
{
}

//------------------------------------------------------------------------------
CollisionBody2::~CollisionBody2()
{
	deleteInternalObject();
}

//------------------------------------------------------------------------------
void CollisionBody2::initialize()
{
	PhysicsObject::initialize();
	m_initialUpdate = true;
	detail::EngineDomain::getPhysicsWorld3D()->addPhysicsObject(this);
}

//------------------------------------------------------------------------------
void CollisionBody2::addShape(CollisionShape* shape)
{
	m_btShapeManager.addShape(shape);
}

//------------------------------------------------------------------------------
void CollisionBody2::setTrigger(bool enabled)
{
	m_isTrigger = enabled;
}

//------------------------------------------------------------------------------
bool CollisionBody2::isTrigger() const
{
	return m_isTrigger;
}

//------------------------------------------------------------------------------
void CollisionBody2::onBeforeStepSimulation()
{
	if (m_initialUpdate)
	{
		createInternalObject();
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
	btCollisionShape* shape = m_btShapeManager.getBtCollisionShape();
	if (m_btGhostObject->getCollisionShape() != shape)
	{
		m_btGhostObject->setCollisionShape(shape);
	}


	auto* transform = getTransform();
	if (transform != nullptr)
	{
		btTransform t;
		t.setFromOpenGLMatrix((btScalar*)&transform->getWorldMatrix());
		m_btGhostObject->setWorldTransform(t);
	}
}

//------------------------------------------------------------------------------
void CollisionBody2::onAfterStepSimulation()
{
	// get transform
	m_btGhostObject->getWorldTransform().getOpenGLMatrix((btScalar*)&m_transform);

	// notify onTriggerStay
	if (isTrigger())
	{
		int count = m_btGhostObject->getNumOverlappingObjects();
		for (int i = 0; i < count; i++)
		{
			btCollisionObject* btObj = m_btGhostObject->getOverlappingObject(i);
			onTriggerStay(reinterpret_cast<PhysicsComponent*>(btObj->getUserPointer()));
		}
	}
}

//------------------------------------------------------------------------------
void CollisionBody2::onRemovedFromWorld()
{
	// Bullet の World に追加するのはこのクラスの役目なので、除外もここで行う。
	if (m_btGhostObject != nullptr)
	{
		getOwnerWorld()->getBtWorld()->removeCollisionObject(m_btGhostObject);
	}
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::onTriggerEnter(PhysicsComponent* otherObject)
{
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::onTriggerLeave(PhysicsComponent* otherObject)
{
}

//------------------------------------------------------------------------------
void CollisionBody2::onTriggerStay(PhysicsComponent* otherObject)
{
}

//------------------------------------------------------------------------------
void CollisionBody2::createInternalObject()
{
	deleteInternalObject();

	// 現状、この時点で必ず m_shape が無ければならない。
	LN_ASSERT(!m_btShapeManager.isEmpty());

	m_btGhostObject = new LocalGhostObject(this);
	m_btGhostObject->setCollisionShape(m_btShapeManager.getBtCollisionShape());
	m_btGhostObject->setUserPointer(this);

	getOwnerWorld()->getBtWorld()->addCollisionObject(m_btGhostObject, getCollisionFilterGroup(), getCollisionFilterMask());
}

//------------------------------------------------------------------------------
void CollisionBody2::deleteInternalObject()
{
	LN_SAFE_DELETE(m_btGhostObject);
}

//==============================================================================
// CollisionBodyComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(CollisionBodyComponent, PhysicsComponent);

//------------------------------------------------------------------------------
Ref<CollisionBodyComponent> CollisionBodyComponent::create(CollisionShape* shape)
{
	auto ptr = Ref<CollisionBodyComponent>::makeRef();
	ptr->initialize();
	ptr->addShape(shape);
	return ptr;
}

//------------------------------------------------------------------------------
CollisionBodyComponent::CollisionBodyComponent()
	: PhysicsComponent()
	, m_btGhostObject(nullptr)
	, m_isTrigger(false)
	, m_initialUpdate(false)
{
}

//------------------------------------------------------------------------------
CollisionBodyComponent::~CollisionBodyComponent()
{
	deleteInternalObject();
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::initialize()
{
	PhysicsComponent::initialize();
	m_initialUpdate = true;
	detail::EngineDomain::getPhysicsWorld3D()->addPhysicsObject(this);
}

//------------------------------------------------------------------------------
//const Matrix& CollisionBodyComponent::getTransform() const
//{
//	return m_transform;
//}

//------------------------------------------------------------------------------
void CollisionBodyComponent::addShape(CollisionShape* shape)
{
	m_btShapeManager.addShape(shape);
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::setTrigger(bool enabled)
{
	m_isTrigger = enabled;
}

//------------------------------------------------------------------------------
bool CollisionBodyComponent::isTrigger() const
{
	return m_isTrigger;
}


//------------------------------------------------------------------------------
EventConnection CollisionBodyComponent::connectOnTriggerEnter(CollisionEventHandler handler)
{
	return m_onTriggerEnter.connect(handler);
}

//------------------------------------------------------------------------------
EventConnection CollisionBodyComponent::connectOnTriggerLeave(CollisionEventHandler handler)
{
	return m_onTriggerLeave.connect(handler);
}

//------------------------------------------------------------------------------
EventConnection CollisionBodyComponent::connectOnTriggerStay(CollisionEventHandler handler)
{
	return m_onTriggerStay.connect(handler);
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::onBeforeStepSimulation()
{
	if (m_initialUpdate)
	{
		createInternalObject();
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
	btCollisionShape* shape = m_btShapeManager.getBtCollisionShape();
	if (m_btGhostObject->getCollisionShape() != shape)
	{
		m_btGhostObject->setCollisionShape(shape);
	}
	

	auto* transform = getTransform();
	if (transform != nullptr)
	{
		btTransform t;
		t.setFromOpenGLMatrix((btScalar*)&transform->getWorldMatrix());
		m_btGhostObject->setWorldTransform(t);
	}
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::onAfterStepSimulation()
{
	// get transform
	m_btGhostObject->getWorldTransform().getOpenGLMatrix((btScalar*)&m_transform);

	// notify onTriggerStay
	if (isTrigger())
	{
		int count = m_btGhostObject->getNumOverlappingObjects();
		for (int i = 0; i < count; i++)
		{
			btCollisionObject* btObj = m_btGhostObject->getOverlappingObject(i);
			onTriggerStay(reinterpret_cast<PhysicsComponent*>(btObj->getUserPointer()));
		}
	}
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::onRemovedFromWorld()
{
	// Bullet の World に追加するのはこのクラスの役目なので、除外もここで行う。
	if (m_btGhostObject != nullptr)
	{
		getOwnerWorld()->getBtWorld()->removeCollisionObject(m_btGhostObject);
	}
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::onTriggerEnter(PhysicsComponent* otherObject)
{
	m_onTriggerEnter.raise(otherObject);
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::onTriggerLeave(PhysicsComponent* otherObject)
{
	m_onTriggerLeave.raise(otherObject);
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::onTriggerStay(PhysicsComponent* otherObject)
{
	m_onTriggerStay.raise(otherObject);
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::createInternalObject()
{
	deleteInternalObject();

	// 現状、この時点で必ず m_shape が無ければならない。
	LN_ASSERT(!m_btShapeManager.isEmpty());

	m_btGhostObject = new LocalGhostObject(this);
	m_btGhostObject->setCollisionShape(m_btShapeManager.getBtCollisionShape());
	m_btGhostObject->setUserPointer(this);

	getOwnerWorld()->getBtWorld()->addCollisionObject(m_btGhostObject, getCollisionFilterGroup(), getCollisionFilterMask());
}

//------------------------------------------------------------------------------
void CollisionBodyComponent::deleteInternalObject()
{
	LN_SAFE_DELETE(m_btGhostObject);
}

LN_NAMESPACE_END
