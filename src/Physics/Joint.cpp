
#include "../Internal.h"
#include <btBulletDynamicsCommon.h>
#include <Lumino/Physics/RigidBody.h>
#include <Lumino/Physics/Joint.h>
#include "BulletUtils.h"

LN_NAMESPACE_BEGIN
namespace Physics
{

//=============================================================================
// Joint
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Joint::Joint()
	: m_manager(NULL)
	, m_btConstraint(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Joint::~Joint()
{
	if (m_manager != NULL)
    {
		if (m_btConstraint != NULL)
        {
			m_manager->GetBtWorld()->removeConstraint(m_btConstraint);
			LN_SAFE_DELETE(m_btConstraint);
        }
		LN_SAFE_RELEASE(m_manager);
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Joint::Create(PhysicsManager* manager, btTypedConstraint* constraint)
{
	LN_REFOBJ_SET(m_manager, manager);
	m_btConstraint = constraint;
	m_manager->AddJoint(this);
}

//=============================================================================
// DofSpringJoint
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DofSpringJoint::DofSpringJoint()
	: m_btDofSpringConstraint(NULL)
	, m_body0(NULL)
	, m_body1(NULL)
	//, m_modifiedFlags(Modified_None)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DofSpringJoint::~DofSpringJoint()
{
	// コンストレイントを先に解放する必要があるため、ベースのデストラクタより先に解放処理を行う
	if (m_manager != NULL)
	{
		if (m_btConstraint != NULL)
		{
			m_manager->GetBtWorld()->removeConstraint(m_btConstraint);
			LN_SAFE_DELETE(m_btConstraint);
		}
	}

	LN_SAFE_RELEASE(m_body0);
	LN_SAFE_RELEASE(m_body1);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::Create(PhysicsManager* manager, RigidBody* body0, RigidBody* body1, const Matrix& localOffset0, const Matrix& localOffset1)
{
	LN_ASSERT(manager && body0 && body1);

	LN_REFOBJ_SET(m_body0, body0);
	LN_REFOBJ_SET(m_body1, body1);

	btTransform local0, local1;
	local0.setFromOpenGLMatrix((const btScalar*)&localOffset0);
	local1.setFromOpenGLMatrix((const btScalar*)&localOffset1);

	m_btDofSpringConstraint = new btGeneric6DofSpringConstraint(
        *body0->GetBtRigidBody(),
        *body1->GetBtRigidBody(),
		local0,
        local1,
        true);

	Joint::Create(manager, m_btDofSpringConstraint);
	m_initialUpdate = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::EnableSpring(int index, bool enabled)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能

	//LN_ASSERT(0 <= index && index <= 5);
	//m_enableSpring[index] = enabled;
	//m_modifiedFlags |= Modified_EnableSpring;
	m_btDofSpringConstraint->enableSpring(index, enabled);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetStiffness(int index, float stiffness)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	//LN_ASSERT(0 <= index && index <= 5);
	//m_stiffness[index] = stiffness;
	//m_modifiedFlags |= Modified_Stiffness;
	m_btDofSpringConstraint->setStiffness(index, stiffness);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetDamping(int index, float damping)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	//LN_ASSERT(0 <= index && index <= 5);
	//m_damping[index] = damping;
	//m_modifiedFlags |= Modified_Damping;
	m_btDofSpringConstraint->setDamping(index, damping);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetEquilibriumPoint()
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setEquilibriumPoint();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetEquilibriumPoint(int index)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setEquilibriumPoint( index );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetLimit(int index, float low, float hi)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLimit(index, low, hi);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetLinearLowerLimit(const Vector3& linearLower)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLinearLowerLimit(
		BulletUtil::LNVector3ToBtVector3(linearLower));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetLinearUpperLimit(const Vector3& linearUpper)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setLinearUpperLimit(
		BulletUtil::LNVector3ToBtVector3(linearUpper));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetAngularLowerLimit(const Vector3& angularLower)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setAngularLowerLimit(
		BulletUtil::LNVector3ToBtVector3(angularLower));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SetAngularUpperLimit(const Vector3& angularUpper)
{
	LN_THROW(!m_initialUpdate, InvalidOperationException);	// 初回 StepSimulation() 前のみ可能
	m_btDofSpringConstraint->setAngularUpperLimit(
		BulletUtil::LNVector3ToBtVector3(angularUpper));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DofSpringJoint::SyncBeforeStepSimulation()
{
}

} // namespace Physics
LN_NAMESPACE_END
