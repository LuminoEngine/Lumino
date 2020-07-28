﻿
#include "Internal.hpp"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/TriggerBody.hpp>
#include "PhysicsManager.hpp"
#include "BulletUtils.hpp"

namespace ln {

//==============================================================================
// LocalGhostObject

class TriggerBody::LocalGhostObject : public btGhostObject
{
public:
	TriggerBody* m_owner;

	LocalGhostObject(TriggerBody* owner)
		: m_owner(owner)
	{}

	virtual void addOverlappingObjectInternal(btBroadphaseProxy* otherProxy, btBroadphaseProxy* thisProxy = 0) override
	{
		btCollisionObject* otherObject = (btCollisionObject*)otherProxy->m_clientObject;
		assert(otherObject);
		int index = m_overlappingObjects.findLinearSearch(otherObject);
		if (index == m_overlappingObjects.size())
		{
			m_overlappingObjects.push_back(otherObject);

			auto* world = m_owner->physicsWorld();
			world->postBeginContact(m_owner, static_cast<PhysicsObject*>(otherObject->getUserPointer()));
		}
	}

	virtual void removeOverlappingObjectInternal(btBroadphaseProxy* otherProxy, btDispatcher* dispatcher, btBroadphaseProxy* thisProxy = 0) override
	{
		btCollisionObject* otherObject = (btCollisionObject*)otherProxy->m_clientObject;
		assert(otherObject);
		int index = m_overlappingObjects.findLinearSearch(otherObject);
		if (index < m_overlappingObjects.size())
		{
			m_overlappingObjects[index] = m_overlappingObjects[m_overlappingObjects.size() - 1];
			m_overlappingObjects.pop_back();

			auto* world = m_owner->physicsWorld();
			world->postEndContact(m_owner, static_cast<PhysicsObject*>(otherObject->getUserPointer()));
		}
	}
};

//==============================================================================
// TriggerBody

Ref<TriggerBody> TriggerBody::create(CollisionShape* shape)
{
    return makeObject<TriggerBody>(shape);
}

TriggerBody::TriggerBody()
    : PhysicsObject(PhysicsObjectType::TriggerBody)
{
}

void TriggerBody::init()
{
	PhysicsObject::init();
}

void TriggerBody::init(CollisionShape* shape)
{
    init();
    addCollisionShape(shape);
}

void TriggerBody::addCollisionShape(CollisionShape* shape)
{
    m_shapeManager.addShape(shape);
	m_dirtyFlags |= DirtyFlags_Shapes;
}

void TriggerBody::setTransform(const Matrix& transform)
{
	m_transform = transform;
	m_dirtyFlags |= DirtyFlags_Transform;
}

void TriggerBody::setCollisionGroup(uint32_t value)
{
    if (m_group != value) {
        m_group = value;
        m_dirtyFlags |= DirtyFlags_Group;
    }
}

void TriggerBody::setCollisionGroupMask(uint32_t value)
{
    if (m_groupMask != value) {
        m_groupMask = value;
        m_dirtyFlags |= DirtyFlags_Group;
    }
}

//void TriggerBody::setPosition(const Vector3& value)
//{
//    //if (m_position != value) {
//    //    m_position = value;
//    //}
//}

void TriggerBody::onBeforeStepSimulation()
{
	PhysicsObject::onBeforeStepSimulation();

	if (!m_btGhostObject || (m_dirtyFlags & (DirtyFlags_InitialUpdate))) {
		createBtObject();
		m_dirtyFlags &= ~DirtyFlags_Shapes;	// createBtObject() の中でまとめて処理されるため、↓で処理する必要はない
		m_dirtyFlags &= ~DirtyFlags_Group;	// createBtObject() の中でまとめて処理されるため、↓で処理する必要はない
	}

	if (m_dirtyFlags & DirtyFlags_Shapes) {
		m_btGhostObject->setCollisionShape(m_shapeManager.getBtCollisionShape());
		readdToWorld();
		m_dirtyFlags &= ~DirtyFlags_Group;	// readdToWorld() 処理済み
	}

	if (m_dirtyFlags & DirtyFlags_Group) {
		readdToWorld();
	}

	if (m_dirtyFlags & DirtyFlags_Transform) {
		btTransform transform;
		transform.setFromOpenGLMatrix((btScalar*)&m_transform);
		m_btGhostObject->setWorldTransform(transform);
	}

	m_dirtyFlags = DirtyFlags_None;
}

void TriggerBody::onAfterStepSimulation()
{
	PhysicsObject::onAfterStepSimulation();
}

void TriggerBody::createBtObject()
{
	deleteBtObject();

	// 現状、この時点で必ず m_shape が無ければならない。
	// TODO: ちゃんとエラー処理
	assert(!m_shapeManager.isEmpty());

	m_btGhostObject = new LocalGhostObject(this);
	m_btGhostObject->setUserPointer(this);

	// setCollisionShape() は World に追加する前に必須
	m_btGhostObject->setCollisionShape(m_shapeManager.getBtCollisionShape());

	// btCollisionObject::CF_NO_CONTACT_RESPONSE が付加されると、
	// 他のオブジェクトと物理シミュレーションで接触しないことを示す (すり抜ける)
	m_btGhostObject->setCollisionFlags(m_btGhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	//m_btGhostObject->setWorldTransform(detail::BulletUtil::LNMatrixToBtTransform(mtmp));


	physicsWorld()->getBtWorld()->addCollisionObject(m_btGhostObject, m_group, m_groupMask);
}

void TriggerBody::deleteBtObject()
{
	if (m_btGhostObject) {
		physicsWorld()->getBtWorld()->removeCollisionObject(m_btGhostObject);
		delete m_btGhostObject;
		m_btGhostObject = nullptr;
	}
}

void TriggerBody::readdToWorld()
{
	if (m_btGhostObject) {
		physicsWorld()->getBtWorld()->removeCollisionObject(m_btGhostObject);
		physicsWorld()->getBtWorld()->addCollisionObject(m_btGhostObject, m_group, m_groupMask);
	}
}

} // namespace ln

