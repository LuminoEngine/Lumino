
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
void Joint::Initialize(btTypedConstraint* constraint)
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
void DofSpringJoint::Initialize(RigidBody* bodyA, RigidBody* bodyB, const Matrix& localOffsetA, const Matrix& localOffsetB)
{
	LN_CHECK_ARG(bodyA != nullptr);
	LN_CHECK_ARG(bodyB != nullptr);
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

	Joint::Initialize(m_btDofSpringConstraint);
	m_initialUpdate = true;
}

//------------------------------------------------------------------------------
void DofSpringJoint::EnableSpring(int index, bool enabled)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能

	//LN_ASSERT(0 <= index && index <= 5);
	//m_enableSpring[index] = enabled;
	//m_modifiedFlags |= Modified_EnableSpring;
	m_btDofSpringConstraint->enableSpring(index, enabled);
}

//------------------------------------------------------------------------------
void DofSpringJoint::SetStiffness(int index, float stiffness)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	//LN_ASSERT(0 <= index && index <= 5);
	//m_stiffness[index] = stiffness;
	//m_modifiedFlags |= Modified_Stiffness;
	m_btDofSpringConstraint->setStiffness(index, stiffness);
}

//------------------------------------------------------------------------------
void DofSpringJoint::SetDamping(int index, float damping)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	//LN_ASSERT(0 <= index && index <= 5);
	//m_damping[index] = damping;
	//m_modifiedFlags |= Modified_Damping;
	m_btDofSpringConstraint->setDamping(index, damping);
}

//------------------------------------------------------------------------------
void DofSpringJoint::SetEquilibriumPoint()
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setEquilibriumPoint();
}

//------------------------------------------------------------------------------
void DofSpringJoint::SetEquilibriumPoint(int index)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setEquilibriumPoint( index );
}


//------------------------------------------------------------------------------
void DofSpringJoint::SetLimit(int index, float low, float hi)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLimit(index, low, hi);
}

//------------------------------------------------------------------------------
void DofSpringJoint::SetLinearLowerLimit(const Vector3& linearLower)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLinearLowerLimit(
		detail::BulletUtil::LNVector3ToBtVector3(linearLower));
}

//------------------------------------------------------------------------------
void DofSpringJoint::SetLinearUpperLimit(const Vector3& linearUpper)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLinearUpperLimit(
		detail::BulletUtil::LNVector3ToBtVector3(linearUpper));
}

//------------------------------------------------------------------------------
void DofSpringJoint::SetAngularLowerLimit(const Vector3& angularLower)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setAngularLowerLimit(
		detail::BulletUtil::LNVector3ToBtVector3(angularLower));
}

//------------------------------------------------------------------------------
void DofSpringJoint::SetAngularUpperLimit(const Vector3& angularUpper)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setAngularUpperLimit(
		detail::BulletUtil::LNVector3ToBtVector3(angularUpper));
}

LN_NAMESPACE_END
