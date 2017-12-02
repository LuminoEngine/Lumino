
#include "../Internal.h"
#include <btBulletDynamicsCommon.h>
#include <Lumino/Physics/RigidBody.h>
#include <Lumino/Physics/Joint.h>
#include "BulletUtils.h"
#include "PhysicsManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Joint
//==============================================================================

//------------------------------------------------------------------------------
Joint::Joint()
	: /*m_manager(nullptr)
	, */m_btConstraint(nullptr)
{
}

//------------------------------------------------------------------------------
Joint::~Joint()
{
	if (m_btConstraint != nullptr)
	{
		LN_SAFE_DELETE(m_btConstraint);
	}
}

//------------------------------------------------------------------------------
void Joint::initialize(btTypedConstraint* constraint)
{
	m_btConstraint = constraint;
}


//==============================================================================
// DofSpringJoint
//==============================================================================

//------------------------------------------------------------------------------
DofSpringJoint::DofSpringJoint()
	: m_btDofSpringConstraint(nullptr)
	, m_bodyA(nullptr)
	, m_bodyB(nullptr)
	, m_initialUpdate(true)
{
	//memset(m_enableSpring, 0, sizeof(0));
	//memset(m_stiffness, 0, sizeof(0));
	//memset(m_damping, 0, sizeof(0));
	//memset(m_equilibriumPointIndex, 0, sizeof(0));
	//memset(m_limit, 0, sizeof(0));
	//memset(m_linearLowerLimit, 0, sizeof(0));
	//memset(m_linearUpperLimit, 0, sizeof(0));
	//memset(m_angularLowerLimitt, 0, sizeof(0));
	//memset(m_angularUpperLimit, 0, sizeof(0));
}

//------------------------------------------------------------------------------
DofSpringJoint::~DofSpringJoint()
{
}

//------------------------------------------------------------------------------
void DofSpringJoint::initialize(RigidBodyComponent* bodyA, RigidBodyComponent* bodyB, const Matrix& localOffsetA, const Matrix& localOffsetB)
{
	if (LN_REQUIRE(bodyA != nullptr)) return;
	if (LN_REQUIRE(bodyB != nullptr)) return;
	m_bodyA = bodyA;
	m_bodyB = bodyB;

	btTransform localA, localB;
	localA.setFromOpenGLMatrix((const btScalar*)&localOffsetA);
	localB.setFromOpenGLMatrix((const btScalar*)&localOffsetB);

	m_btDofSpringConstraint = new btGeneric6DofSpringConstraint(
        *m_bodyA->GetBtRigidBody(),
        *m_bodyB->GetBtRigidBody(),
		localA,
        localB,
        true);

	Joint::initialize(m_btDofSpringConstraint);
	m_initialUpdate = true;
}

//------------------------------------------------------------------------------
void DofSpringJoint::enableSpring(int index, bool enabled)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能

	//LN_ASSERT(0 <= index && index <= 5);
	//m_enableSpring[index] = enabled;
	//m_modifiedFlags |= Modified_EnableSpring;
	m_btDofSpringConstraint->enableSpring(index, enabled);
}

//------------------------------------------------------------------------------
void DofSpringJoint::setStiffness(int index, float stiffness)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	//LN_ASSERT(0 <= index && index <= 5);
	//m_stiffness[index] = stiffness;
	//m_modifiedFlags |= Modified_Stiffness;
	m_btDofSpringConstraint->setStiffness(index, stiffness);
}

//------------------------------------------------------------------------------
void DofSpringJoint::setDamping(int index, float damping)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	//LN_ASSERT(0 <= index && index <= 5);
	//m_damping[index] = damping;
	//m_modifiedFlags |= Modified_Damping;
	m_btDofSpringConstraint->setDamping(index, damping);
}

//------------------------------------------------------------------------------
void DofSpringJoint::setEquilibriumPoint()
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	m_btDofSpringConstraint->setEquilibriumPoint();
}

//------------------------------------------------------------------------------
void DofSpringJoint::setEquilibriumPoint(int index)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	m_btDofSpringConstraint->setEquilibriumPoint( index );
}


//------------------------------------------------------------------------------
void DofSpringJoint::setLimit(int index, float low, float hi)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLimit(index, low, hi);
}

//------------------------------------------------------------------------------
void DofSpringJoint::setLinearLowerLimit(const Vector3& linearLower)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLinearLowerLimit(
		detail::BulletUtil::LNVector3ToBtVector3(linearLower));
}

//------------------------------------------------------------------------------
void DofSpringJoint::setLinearUpperLimit(const Vector3& linearUpper)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLinearUpperLimit(
		detail::BulletUtil::LNVector3ToBtVector3(linearUpper));
}

//------------------------------------------------------------------------------
void DofSpringJoint::setAngularLowerLimit(const Vector3& angularLower)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	m_btDofSpringConstraint->setAngularLowerLimit(
		detail::BulletUtil::LNVector3ToBtVector3(angularLower));
}

//------------------------------------------------------------------------------
void DofSpringJoint::setAngularUpperLimit(const Vector3& angularUpper)
{
	if (LN_REQUIRE(m_initialUpdate)) return;	// 初回 stepSimulation() 前のみ可能
	m_btDofSpringConstraint->setAngularUpperLimit(
		detail::BulletUtil::LNVector3ToBtVector3(angularUpper));
}



//==============================================================================
// Joint2
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Joint2, PhysicsResource2);

Joint2::Joint2()
	: PhysicsResource2(PhysicsResourceType::Joint)
{
}

Joint2::~Joint2()
{
}

void Joint2::initialize()
{
	PhysicsResource2::initialize();
}


//==============================================================================
// DofSpringJoint2
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(DofSpringJoint2, Joint2);

DofSpringJoint2::DofSpringJoint2()
	: m_btDofSpringConstraint(nullptr)
{
}

DofSpringJoint2::~DofSpringJoint2()
{
	LN_SAFE_DELETE(m_btDofSpringConstraint);
}

void DofSpringJoint2::initialize(const DofSpringJointSimpleConstructionData& data)
{
	// 回転行列作成
	btMatrix3x3	btmRotationMat;

	btmRotationMat.setEulerZYX(data.rotationAngles.x, data.rotationAngles.y, data.rotationAngles.z);

	// コンストレイントのトランスフォームを作成
	btTransform		bttrTransform;

	bttrTransform.setIdentity();
	bttrTransform.setOrigin(btVector3(data.position.x, data.position.y, data.position.z));
	bttrTransform.setBasis(btmRotationMat);

	// 剛体A,Bから見たコンストレイントのトランスフォームを作成 
	btTransform bttrRigidAInvTransform = data.bodyA->getBtRigidBody()->getWorldTransform().inverse(),
	bttrRigidBInvTransform = data.bodyA->getBtRigidBody()->getWorldTransform().inverse();

	bttrRigidAInvTransform = bttrRigidAInvTransform * bttrTransform;
	bttrRigidBInvTransform = bttrRigidBInvTransform * bttrTransform;

	m_btDofSpringConstraint = new btGeneric6DofSpringConstraint(
		*data.bodyA->getBtRigidBody(), *data.bodyA->getBtRigidBody(),
		bttrRigidAInvTransform, bttrRigidBInvTransform, true);

	// 各種制限パラメータのセット
	m_btDofSpringConstraint->setLinearLowerLimit(btVector3(data.vec3PosLimitL.x, data.vec3PosLimitL.y, data.vec3PosLimitL.z));
	m_btDofSpringConstraint->setLinearUpperLimit(btVector3(data.vec3PosLimitU.x, data.vec3PosLimitU.y, data.vec3PosLimitU.z));

	m_btDofSpringConstraint->setAngularLowerLimit(btVector3(data.vec3RotLimitL.x, data.vec3RotLimitL.y, data.vec3RotLimitL.z));
	m_btDofSpringConstraint->setAngularUpperLimit(btVector3(data.vec3RotLimitU.x, data.vec3RotLimitU.y, data.vec3RotLimitU.z));

	// 0 : translation X
	if (data.vec3SpringPos.x != 0.0f)
	{
		m_btDofSpringConstraint->enableSpring(0, true);
		m_btDofSpringConstraint->setStiffness(0, data.vec3SpringPos.x);
	}

	// 1 : translation Y
	if (data.vec3SpringPos.y != 0.0f)
	{
		m_btDofSpringConstraint->enableSpring(1, true);
		m_btDofSpringConstraint->setStiffness(1, data.vec3SpringPos.y);
	}

	// 2 : translation Z
	if (data.vec3SpringPos.z != 0.0f)
	{
		m_btDofSpringConstraint->enableSpring(2, true);
		m_btDofSpringConstraint->setStiffness(2, data.vec3SpringPos.z);
	}

	// 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
	// 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
	// 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
	m_btDofSpringConstraint->enableSpring(3, true);	m_btDofSpringConstraint->setStiffness(3, data.vec3SpringRot.x);
	m_btDofSpringConstraint->enableSpring(4, true);	m_btDofSpringConstraint->setStiffness(4, data.vec3SpringRot.y);
	m_btDofSpringConstraint->enableSpring(5, true);	m_btDofSpringConstraint->setStiffness(5, data.vec3SpringRot.z);
}

LN_NAMESPACE_END
