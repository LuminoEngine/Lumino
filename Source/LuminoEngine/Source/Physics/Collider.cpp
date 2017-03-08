
#include "Internal.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletDynamicsCommon.h>
#include <Lumino/Physics/CollisionShape.h>
#include <Lumino/Physics/PhysicsWorld.h>
#include <Lumino/Physics/Collider.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// LocalGhostObject
//==============================================================================
class Collider::LocalGhostObject : public btGhostObject
{
public:
	Collider*	m_owner;

	LocalGhostObject(Collider* owner)
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
			if (m_owner->IsTrigger())
			{
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
			if (m_owner->IsTrigger())
			{
				m_owner->OnTriggerLeave(reinterpret_cast<PhysicsObject*>(otherObject->getUserPointer()));
			}
		}
	}
};

//==============================================================================
// Collider
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Collider, PhysicsObject);

//------------------------------------------------------------------------------
RefPtr<Collider> Collider::Create(CollisionShape* shape)
{
	auto ptr = RefPtr<Collider>::MakeRef();
	ptr->Initialize();
	ptr->AddShape(shape);
	return ptr;
}

//------------------------------------------------------------------------------
Collider::Collider()
	: PhysicsObject()
	, m_btGhostObject(nullptr)
	, m_shape(nullptr)
	, m_isTrigger(false)
	, m_initialUpdate(false)
{
}

//------------------------------------------------------------------------------
Collider::~Collider()
{
	DeleteInternalObject();
}

//------------------------------------------------------------------------------
void Collider::Initialize()
{
	PhysicsObject::Initialize();
	m_initialUpdate = true;
	detail::EngineDomain::GetPhysicsWorld3D()->AddPhysicsObject(this);
}

//------------------------------------------------------------------------------
const Matrix& Collider::GetTransform() const
{
	return m_transform;
}

//------------------------------------------------------------------------------
void Collider::AddShape(CollisionShape* shape)
{
	LN_FAIL_CHECK_ARG(shape != nullptr) return;

	if (m_shape != nullptr)
	{
		// TODO: 今は1つのみ許可
		LN_NOTIMPLEMENTED();
	}
	else
	{
		m_shape = shape;
	}
}

//------------------------------------------------------------------------------
void Collider::SetTrigger(bool enabled)
{
	m_isTrigger = enabled;
}

//------------------------------------------------------------------------------
bool Collider::IsTrigger() const
{
	return m_isTrigger;
}


//------------------------------------------------------------------------------
void Collider::ConnectOnTriggerEnter(CollisionEventHandler handler)
{
	onTriggerEnter.AddHandler(handler);
}

//------------------------------------------------------------------------------
void Collider::ConnectOnTriggerLeave(CollisionEventHandler handler)
{
	onTriggerLeave.AddHandler(handler);
}

//------------------------------------------------------------------------------
void Collider::ConnectOnTriggerStay(CollisionEventHandler handler)
{
	onTriggerStay.AddHandler(handler);
}

//------------------------------------------------------------------------------
void Collider::OnBeforeStepSimulation()
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
	if (m_shape != nullptr)
	{
		if (m_btGhostObject->getCollisionShape() != m_shape->GetBtCollisionShape())
		{
			m_btGhostObject->setCollisionShape(m_shape->GetBtCollisionShape());
		}
	}
}

//------------------------------------------------------------------------------
void Collider::OnAfterStepSimulation()
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
void Collider::OnRemovedFromWorld()
{
	// Bullet の World に追加するのはこのクラスの役目なので、除外もここで行う。
	if (m_btGhostObject != nullptr)
	{
		GetOwnerWorld()->GetBtWorld()->removeCollisionObject(m_btGhostObject);
	}
}

//------------------------------------------------------------------------------
void Collider::OnTriggerEnter(PhysicsObject* otherObject)
{
	onTriggerEnter.Raise(otherObject);
}

//------------------------------------------------------------------------------
void Collider::OnTriggerLeave(PhysicsObject* otherObject)
{
	onTriggerLeave.Raise(otherObject);
}

//------------------------------------------------------------------------------
void Collider::OnTriggerStay(PhysicsObject* otherObject)
{
	onTriggerStay.Raise(otherObject);
}

//------------------------------------------------------------------------------
void Collider::CreateInternalObject()
{
	DeleteInternalObject();

	// 現状、この時点で必ず m_shape が無ければならない。
	LN_ASSERT(m_shape != nullptr);

	m_btGhostObject = new LocalGhostObject(this);
	m_btGhostObject->setCollisionShape(m_shape->GetBtCollisionShape());
	m_btGhostObject->setUserPointer(this);


	GetOwnerWorld()->GetBtWorld()->addCollisionObject(m_btGhostObject, GetCollisionFilterGroup(), GetCollisionFilterMask());
}

//------------------------------------------------------------------------------
void Collider::DeleteInternalObject()
{
	LN_SAFE_DELETE(m_btGhostObject);
}

LN_NAMESPACE_END
