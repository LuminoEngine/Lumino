
#include "Internal.h"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <btBulletDynamicsCommon.h>
#include <LinearMath/btMotionState.h>
#include <Lumino/Physics/CollisionShape.h>
#include <Lumino/Physics/RigidBody.h>
#include <Lumino/Physics/PhysicsWorld.h>
#include "BulletUtils.h"
#include "PhysicsManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// SynchronizeMotionState
//==============================================================================
struct SynchronizeMotionState
	: public btDefaultMotionState
{
	RigidBody*	m_owner;

	SynchronizeMotionState(RigidBody* owner, const btTransform& startTrans = btTransform::getIdentity(), const btTransform& centerOfMassOffset = btTransform::getIdentity())
		: btDefaultMotionState(startTrans, centerOfMassOffset)
		, m_owner(owner)
	{
	}

	virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans) override
	{
		btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);
		m_owner->SetTransformFromMotionState(centerOfMassWorldTrans);
	}
};

//==============================================================================
// RigidBody
/*
	
*/
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(RigidBody, PhysicsObject);

//------------------------------------------------------------------------------
RefPtr<RigidBody> RigidBody::Create(CollisionShape* collider)
{
	ConfigData data;
	data.Mass = 1.0f;
	auto ptr = RefPtr<RigidBody>::MakeRef();
	ptr->Initialize(collider, data);
	return ptr;
}

//------------------------------------------------------------------------------
RigidBody::RigidBody()
	: PhysicsObject()
	, m_btRigidBody(nullptr)
	, m_collisionShape(nullptr)
	, m_constraintFlags(RigidbodyConstraintFlags::None)
	, m_modifiedFlags(Modified_None)
{
}

//------------------------------------------------------------------------------
RigidBody::~RigidBody()
{
	if (m_btRigidBody != nullptr)
	{
		btMotionState* state = m_btRigidBody->getMotionState();
		LN_SAFE_DELETE(state);
		LN_SAFE_DELETE(m_btRigidBody);
	}
}

//------------------------------------------------------------------------------
void RigidBody::Initialize(CollisionShape* collider, const ConfigData& configData)
{
	LN_CHECK_ARG(collider != nullptr);
	PhysicsObject::Initialize();
	m_collisionShape = collider;
	m_data = configData;
	m_modifiedFlags |= Modified_InitialUpdate;
	detail::EngineDomain::GetPhysicsWorld3D()->AddPhysicsObject(this);
}

//------------------------------------------------------------------------------
void RigidBody::InitializeCore(CollisionShape* collider, const ConfigData& configData, float scale)
{
	LN_CHECK_ARG(collider != nullptr);
	PhysicsObject::Initialize();
	m_collisionShape = collider;
	m_data = configData;
	m_data.Scale = scale;
	SetCollisionFilterGroup(configData.Group);
	SetCollisionFilterMask(configData.GroupMask);
	m_modifiedFlags |= Modified_InitialUpdate;
}

//------------------------------------------------------------------------------
void RigidBody::SetPosition(const Vector3& position)
{
	//m_btRigidBody->activate();

	m_data.InitialTransform.m[3][0] = position.x;
	m_data.InitialTransform.m[3][1] = position.y;
	m_data.InitialTransform.m[3][2] = position.z;
	m_modifiedFlags |= Modified_WorldTransform;	// 姿勢を更新した
	m_modifiedFlags |= Modified_Activate;		// Activate要求

	//btVector3 pos = BulletUtil::LNVector3ToBtVector3( position );

	//if ( m_btRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT )
	//{
	//	btTransform Transform = m_btRigidBody->getWorldTransform();
	//	Transform.setOrigin( pos );
	//	m_btRigidBody->setWorldTransform( Transform );
	//}
	//else
	//{
	//	m_btRigidBody->getWorldTransform().setOrigin( pos );
	//}
	//m_btRigidBody->activate( true );

	//m_btRigidBody->getWorldTransform().getOpenGLMatrix((btScalar*)&mWorldMatrix);
}
void RigidBody::SetPosition(float x, float y, float z)
{
	SetPosition(Vector3(x, y, z));
}

//------------------------------------------------------------------------------
void RigidBody::SetLinearVelocity(const Vector3& velocity)
{
	// TODO: m_btRigidBody ここで参照しない
	m_btRigidBody->setLinearVelocity(detail::BulletUtil::LNVector3ToBtVector3(velocity));
}

//------------------------------------------------------------------------------
void RigidBody::SetAngularVelocity(const Vector3& velocity)
{
	// TODO: m_btRigidBody ここで参照しない
	m_btRigidBody->setAngularVelocity(detail::BulletUtil::LNVector3ToBtVector3(velocity));
}

//------------------------------------------------------------------------------
void RigidBody::SetConstraints(RigidbodyConstraintFlags flags)
{
	m_constraintFlags = flags;
	m_modifiedFlags |= Modified_RigidBodyConstraintFlags;
}


#if 0
//------------------------------------------------------------------------------
void RigidBody::setRotation( const LQuaternion& rotation )
{
	btQuaternion q = BulletUtil::LNQuaternionToBtQuaternion( rotation );
	m_btRigidBody->getWorldTransform().setRotation( q );

		
	//m_btRigidBody->getWorldTransform().setBasis().
	//btTransform transform = m_btRigidBody->getCenterOfMassTransform().setRotation( q );
	//transform.setRotation( q );
	//m_btRigidBody

	m_btRigidBody->getWorldTransform().getOpenGLMatrix((btScalar*)&mWorldMatrix);
}

//------------------------------------------------------------------------------
void RigidBody::setAngle( const LVector3& euler )
{
	btMatrix3x3 m;
	//m.setEulerZYX( euler.x, euler.y, euler.z );
	m.setEulerYPR( euler.y, euler.x, euler.z );
#if 1
	m_btRigidBody->getWorldTransform().setBasis(m);
#else
		

	btTransform transform = m_btRigidBody->getCenterOfMassTransform();
	transform.setBasis( m );

	m_btRigidBody->setCenterOfMassTransform( transform );
	//m_btRigidBody->getWorldTransform().setBasis( m );
#endif
}

//------------------------------------------------------------------------------
void RigidBody::setDamping( float linDamping, float angDamping )
{
	m_btRigidBody->setDamping( linDamping, angDamping );
}

//------------------------------------------------------------------------------
void RigidBody::setRestitution( float value ) 
{ 
	return m_btRigidBody->setRestitution( value ); 
}

//------------------------------------------------------------------------------
float RigidBody::getRestitution() const 
{ 
	return m_btRigidBody->getRestitution(); 
}

//------------------------------------------------------------------------------
void RigidBody::setFriction( float value ) 
{ 
	m_btRigidBody->setFriction( value ); 
}

//------------------------------------------------------------------------------
float RigidBody::getFriction() const 
{ 
	return m_btRigidBody->getFriction(); 
}

//------------------------------------------------------------------------------
const LMatrix& RigidBody::getWorldMatrix() const
{
	return mWorldMatrix;
}

//------------------------------------------------------------------------------
void RigidBody::setKinematicAlignmentMatrix( const LMatrix& matrix )
{
	mKinematicAlignmentMatrix = matrix;
	if ( mKinematicMotionState )
	{
#if 0
		LMatrix m = mKinematicAlignmentMatrix;
		LMatrix::transformBasis( &m );
		mKinematicMotionState->setMatrix( m );
#else
		mKinematicMotionState->setMatrix( mKinematicAlignmentMatrix );
#endif
	}
}
#endif
	

//------------------------------------------------------------------------------
void RigidBody::SetMass(float mass)
{
	m_data.Mass = mass;
	btVector3 localInertia(0, 0, 0);
	m_collisionShape->GetBtCollisionShape()->calculateLocalInertia(m_data.Mass, localInertia);
	m_modifiedFlags |= Modified_Mass;
}

//------------------------------------------------------------------------------
void RigidBody::ApplyForce(const Vector3& force)
{
	m_appliedForce += force;	// sum up the power to the next simulation
	m_modifiedFlags |= Modified_ApplyForce;
}

//------------------------------------------------------------------------------
void RigidBody::ApplyImpulse(const Vector3& force)
{
	m_appliedImpulse += force;	// sum up the power to the next simulation
	m_modifiedFlags |= Modified_ApplyImpulse;
}

//------------------------------------------------------------------------------
void RigidBody::Activate()
{
	m_modifiedFlags |= Modified_Activate;
}

//------------------------------------------------------------------------------
void RigidBody::SetWorldTransform(const Matrix& matrix)
{
	m_data.InitialTransform = matrix;
	m_modifiedFlags |= Modified_WorldTransform;
}

//------------------------------------------------------------------------------
const Matrix& RigidBody::GetWorldTransform() const
{
	return m_data.InitialTransform;
}

//------------------------------------------------------------------------------
void RigidBody::ClearForces()
{
	m_modifiedFlags |= Modified_ClearForces;
}

//------------------------------------------------------------------------------
void RigidBody::OnBeforeStepSimulation()
{
	if ((m_modifiedFlags & Modified_InitialUpdate) != 0)
	{
		CreateBtRigidBody();
	}

	// SetWorldTransform 要求
	if ((m_modifiedFlags & Modified_WorldTransform) != 0)
	{
		/*
			stepSimulation() の中でこれらが関係する処理は以下のとおり。
				Kinematic(質量が0)な物体の姿勢を MotionState から btCollisionObject::m_worldTransform に取得 getWorldTransform()
					btCollisionObject::m_worldTransform は setWorldTransform からも設定できるので、MotionState を使う意味はあまり無い。
					ただ、m_updateRevision が変わるのでどう影響するか見切れないところ。黙って MotionState 経由にしておくのが無難かも。
				↓
				シミュレーション実行
					btCollisionObject::m_worldTransform は更新される。
					btCollisionObject::m_interpolationWorldTransform も更新される。setCenterOfMassTransform() 経由で。
					setCenterOfMassTransform() は stepSimulation() の下で呼ばれている。
				↓
				Kinematicではない物体の姿勢を MotionState に set。
					このとき、何かの時間を使って m_interpolationWorldTransform を補完し、その結果を MotionState に set している。
					なので、btCollisionObject::m_worldTransform とも btCollisionObject::m_interpolationWorldTransform とも違う姿勢が渡されてくることになり、
					これがシミュレーションの最終結果となる。

			なお、ソースを見た限りだと btMotionState::getWorldTransform() は Kinematic なオブジェクトでしか呼ばれない。
			普通のオブジェクトは btCollisionObject::setWorldTransform() にて姿勢を設定する必要がある。

			念のため…ということで、両方の setWorldTransform() を行い、様子を見てみる。
		*/
		btTransform transform;
		transform.setFromOpenGLMatrix((btScalar*)&m_data.InitialTransform);
		m_btRigidBody->setWorldTransform(transform);

		if (m_btRigidBody != nullptr)
		{
			m_btRigidBody->getMotionState()->setWorldTransform(transform);
		}
	}

	if (m_btRigidBody != nullptr)
	{
		// RigidbodyConstraints
		if ((m_modifiedFlags & Modified_RigidBodyConstraintFlags) != 0)
		{
			btVector3 liner(
				m_constraintFlags.TestFlag(RigidbodyConstraintFlags::FreezePositionX) ? 0.0f : 1.0f,
				m_constraintFlags.TestFlag(RigidbodyConstraintFlags::FreezePositionY) ? 0.0f : 1.0f,
				m_constraintFlags.TestFlag(RigidbodyConstraintFlags::FreezePositionZ) ? 0.0f : 1.0f);
			btVector3 angler(
				m_constraintFlags.TestFlag(RigidbodyConstraintFlags::FreezeRotationX) ? 0.0f : 1.0f,
				m_constraintFlags.TestFlag(RigidbodyConstraintFlags::FreezeRotationY) ? 0.0f : 1.0f,
				m_constraintFlags.TestFlag(RigidbodyConstraintFlags::FreezeRotationZ) ? 0.0f : 1.0f);
			m_btRigidBody->setLinearFactor(liner);
			m_btRigidBody->setAngularFactor(angler);
		}

		// Mass
		if ((m_modifiedFlags & Modified_Mass) != 0)
		{
			/* Bullet は StaticObject はシミュレーションから完全に除外している。
			* StaticObject でないものは World の m_nonStaticRigidBodies に追加されてシミュレーションの
			* 対象となるのだが、このリストに追加されるのは addRigidBody() された時のみ。
			* 後から StaticObject であるフラグが変更されても、再度 addRigidBody() しなければ
			* シミュレーションの対象とはならない。
			* ここでは、setMassProps() の前後でフラグ isStaticObject() が変わった場合、World に追加しなおしている。
			*/
			bool isStatic = m_btRigidBody->isStaticObject();
			btVector3 inertia;
			m_btRigidBody->getCollisionShape()->calculateLocalInertia(m_data.Mass, inertia);
			m_btRigidBody->setMassProps(m_data.Mass, inertia);


			if (isStatic != m_btRigidBody->isStaticObject())
			{
				GetOwnerWorld()->GetBtWorld()->removeRigidBody(m_btRigidBody);
				GetOwnerWorld()->GetBtWorld()->addRigidBody(m_btRigidBody);
			}
		}

		// ClearForces 要求
		if ((m_modifiedFlags & Modified_ClearForces) != 0)
		{
			m_btRigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
			m_btRigidBody->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
			//m_btRigidBody->setInterpolationLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
			//m_btRigidBody->setInterpolationAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
			//m_btRigidBody->setInterpolationWorldTransform(m_btRigidBody->getCenterOfMassTransform());
			m_btRigidBody->clearForces();
		}

		// ApplyForce
		if ((m_modifiedFlags & Modified_ApplyForce) != 0)
		{
			m_btRigidBody->applyForce(detail::BulletUtil::LNVector3ToBtVector3(m_appliedForce), btVector3(0, 0, 0));
			m_appliedForce = Vector3::Zero;
		}
		// ApplyImpulse
		if ((m_modifiedFlags & Modified_ApplyImpulse) != 0)
		{
			m_btRigidBody->applyImpulse(detail::BulletUtil::LNVector3ToBtVector3(m_appliedImpulse), btVector3(0, 0, 0));
			m_appliedImpulse = Vector3::Zero;
		}
	}

	// Activate 要求
	if ((m_modifiedFlags & Modified_Activate) != 0)
	{
		m_btRigidBody->activate();
	}

	m_modifiedFlags = Modified_None;
}

//------------------------------------------------------------------------------
void RigidBody::OnAfterStepSimulation()
{
	if (m_btRigidBody != nullptr)
	{
		if (m_btRigidBody->isKinematicObject())
		{
			// static オブジェクトなので受け取る必要はない
		}
		else
		{
			btTransform transform;
			m_btRigidBody->getMotionState()->getWorldTransform(transform);
			transform.getOpenGLMatrix((btScalar*)&m_data.InitialTransform);
		}
	}
}

//------------------------------------------------------------------------------
void RigidBody::SetTransformFromMotionState(const btTransform& transform)
{
	//if (m_btGhostObject != nullptr)
	//{
	//	 トリガーオブジェクトの姿勢を transform(剛体) に同期する
	//	m_btGhostObject->setWorldTransform(transform);
	//}
}

void RigidBody::MarkMMDDynamic()
{
	m_btRigidBody->setCollisionFlags(m_btRigidBody->getCollisionFlags() & -3);
}

//------------------------------------------------------------------------------
void RigidBody::OnRemovedFromWorld()
{
	// Bullet の World に追加するのは RigidBody クラスの役目なので、除外もここで行う。

	if (m_btRigidBody != nullptr)
	{
		GetOwnerWorld()->GetBtWorld()->removeRigidBody(m_btRigidBody);
	}
	//if (m_btGhostObject != nullptr)
	//{
	//	GetOwnerWorld()->GetBtWorld()->removeCollisionObject(m_btGhostObject.get());
	//}
}

//------------------------------------------------------------------------------
void RigidBody::CreateBtRigidBody()
{
	// 各初期プロパティ
	float num = m_data.Mass * m_data.Scale;
	float friction;
	float hitFraction;
	float linearDamping;
	float angularDamping;
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (m_data.KinematicObject)
	{
		num = 0.0f;
		friction = m_data.Friction;
		hitFraction = m_data.Restitution;
		linearDamping = m_data.LinearDamping;
		angularDamping = m_data.AngularDamping;
	}
	else
	{
		m_collisionShape->GetBtCollisionShape()->calculateLocalInertia(num, localInertia);
		friction = m_data.Friction;
		hitFraction = m_data.Restitution;
		linearDamping = m_data.LinearDamping;
		angularDamping = m_data.AngularDamping;
	}

	// 初期姿勢と MotionState
	btTransform initialTransform;
	//if (configData.InitialTransform != nullptr)
	{
		initialTransform.setFromOpenGLMatrix((const btScalar*)&m_data.InitialTransform);
		initialTransform.getOrigin().setX(initialTransform.getOrigin().x() * m_data.Scale);
		initialTransform.getOrigin().setY(initialTransform.getOrigin().y() * m_data.Scale);
		initialTransform.getOrigin().setZ(initialTransform.getOrigin().z() * m_data.Scale);
	}
	//else {
	//	initialTransform.setIdentity();
	//}
	btMotionState* motionState;
	if (m_data.KinematicObject)
	{
		motionState = new SynchronizeMotionState(this, initialTransform/*initialTransformMatrix * Matrix.Translation(frame.Bone.Position * scale)*/);
	}
	else
	{
		//motionState = new DefaultMotionState(Matrix.Invert(rigid.BoneLocalPosition) * Matrix.Translation(frame.Bone.Position) * frame.CombinedMatrix);
		motionState = new SynchronizeMotionState(this, initialTransform/*initialTransformMatrix * Matrix.Translation(frame.Bone.Position * scale)*/);
	}

	// RigidBody 作成
	btRigidBody::btRigidBodyConstructionInfo bodyInfo(num, motionState, m_collisionShape->GetBtCollisionShape(), localInertia);
	bodyInfo.m_linearDamping = m_data.LinearDamping;	// 移動減
	bodyInfo.m_angularDamping = m_data.AngularDamping;	// 回転減
	bodyInfo.m_restitution = m_data.Restitution;	    // 反発力
	bodyInfo.m_friction = m_data.Friction;				// 摩擦力
	bodyInfo.m_additionalDamping = m_data.AdditionalDamping;
	m_btRigidBody = new btRigidBody(bodyInfo);

	if (m_data.KinematicObject)
	{
		// CF_KINEMATIC_OBJECT と DISABLE_DEACTIVATION はセット。決まり事。
		// http://bulletjpn.web.fc2.com/07_RigidBodyDynamics.html
		m_btRigidBody->setCollisionFlags( /*m_btRigidBody->getCollisionFlags() | */btCollisionObject::CF_KINEMATIC_OBJECT);
		m_btRigidBody->setActivationState( /*m_btRigidBody->getActivationState() | */DISABLE_DEACTIVATION);
	}
	else
	{
		m_btRigidBody->setActivationState( /*m_btRigidBody->getActivationState() | */DISABLE_DEACTIVATION);
	}
	m_btRigidBody->setSleepingThresholds(0.0f, 0.0f);

	m_modifiedFlags |= Modified_Activate;

	//BodyBase::Initialize(m_btRigidBody);

	GetOwnerWorld()->GetBtWorld()->addRigidBody(GetBtRigidBody(), GetCollisionFilterGroup(), GetCollisionFilterMask());

}

LN_NAMESPACE_END
