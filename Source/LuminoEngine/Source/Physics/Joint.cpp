
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
void DofSpringJoint::initialize(RigidBody* bodyA, RigidBody* bodyB, const Matrix& localOffsetA, const Matrix& localOffsetB)
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

LN_NAMESPACE_END
