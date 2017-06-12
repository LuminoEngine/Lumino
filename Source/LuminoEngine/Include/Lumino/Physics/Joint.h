
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class PhysicsWorld;
class RigidBody;

/// ジョイントのベースクラス
class Joint
    : public RefObject
{
public:


protected:
	Joint();
	virtual ~Joint();
	void initialize(btTypedConstraint* constraint);

LN_INTERNAL_ACCESS:
	btTypedConstraint* GetBtConstraint() { return m_btConstraint; }

private:
	//detail::PhysicsWorld*	m_world;
    btTypedConstraint*		m_btConstraint;
};

/// DofSpringJoint
///		現状、set 系は初期値 set でしか使わないので遅延設定の対応は今のところ必要ない。
///		代わりに、1度でも StepSimulation() した後に set しようとしたら例外になる。
class DofSpringJoint
	: public Joint
{
public:

	// 各 index は 0～5
	void EnableSpring(int index, bool enabled);
	void SetStiffness(int index, float stiffness);
	void SetDamping(int index, float damping);
	void SetEquilibriumPoint();
	void SetEquilibriumPoint(int index);

	void SetLimit(int index, float low, float hi);
	void SetLinearLowerLimit(const Vector3& linearLower);
	void SetLinearUpperLimit(const Vector3& linearUpper);
	void SetAngularLowerLimit(const Vector3& angularLower);
	void SetAngularUpperLimit(const Vector3& angularUpper);

LN_INTERNAL_ACCESS:
	DofSpringJoint();
	virtual ~DofSpringJoint();
	void initialize(RigidBody* bodyA, RigidBody* bodyB, const Matrix& localOffsetA, const Matrix& localOffsetB);

private:
	btGeneric6DofSpringConstraint*	m_btDofSpringConstraint;
	RefPtr<RigidBody>				m_bodyA;
	RefPtr<RigidBody>				m_bodyB;

	//bool			m_enableSpring[6];
	//float			m_stiffness[6];
	//float			m_damping[6];
	//int				m_equilibriumPointIndex[6];
	//Vector2			m_limit[6];					///< x:low y:hi
	//Vector3			m_linearLowerLimit[6];
	//Vector3			m_linearUpperLimit[6];
	//Vector3			m_angularLowerLimitt[6];
	//Vector3			m_angularUpperLimit[6];

	//uint32_t		m_modifiedFlags;
	volatile bool	m_initialUpdate;
};

LN_NAMESPACE_END
