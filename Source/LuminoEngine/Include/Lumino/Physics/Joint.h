
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class PhysicsWorld;
class RigidBodyComponent;

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
	btTypedConstraint* getBtConstraint() { return m_btConstraint; }

private:
	//detail::PhysicsWorld*	m_world;
    btTypedConstraint*		m_btConstraint;
};

/// DofSpringJoint
///		現状、set 系は初期値 set でしか使わないので遅延設定の対応は今のところ必要ない。
///		代わりに、1度でも stepSimulation() した後に set しようとしたら例外になる。
class DofSpringJoint
	: public Joint
{
public:

	// 各 index は 0～5
	void enableSpring(int index, bool enabled);
	void setStiffness(int index, float stiffness);
	void setDamping(int index, float damping);
	void setEquilibriumPoint();
	void setEquilibriumPoint(int index);

	void setLimit(int index, float low, float hi);
	void setLinearLowerLimit(const Vector3& linearLower);
	void setLinearUpperLimit(const Vector3& linearUpper);
	void setAngularLowerLimit(const Vector3& angularLower);
	void setAngularUpperLimit(const Vector3& angularUpper);

LN_INTERNAL_ACCESS:
	DofSpringJoint();
	virtual ~DofSpringJoint();
	void initialize(RigidBodyComponent* bodyA, RigidBodyComponent* bodyB, const Matrix& localOffsetA, const Matrix& localOffsetB);

private:
	btGeneric6DofSpringConstraint*	m_btDofSpringConstraint;
	Ref<RigidBodyComponent>				m_bodyA;
	Ref<RigidBodyComponent>				m_bodyB;

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



class Joint2
	: public Object
{
	LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
	Joint2();
	virtual ~Joint2();
	void initialize();
	virtual btTypedConstraint* getBtConstraint() const = 0;

private:
};

struct DofSpringJointSimpleConstructionData
{
	Vector3			position;
	Vector3			rotationAngles;

	RigidBody2*		bodyA;
	RigidBody2*		bodyB;

	Vector3			vec3PosLimitL;	// 移動制限1
	Vector3			vec3PosLimitU;	// 移動制限2

	Vector3			vec3RotLimitL;	// 回転制限1
	Vector3			vec3RotLimitU;	// 回転制限2

	Vector3			vec3SpringPos;	// ばね移動
	Vector3			vec3SpringRot;	// ばね回転
};

class DofSpringJoint2
	: public Joint2
{
	LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
	DofSpringJoint2();
	virtual ~DofSpringJoint2();
	void initialize(const DofSpringJointSimpleConstructionData& data);

private:
	btGeneric6DofSpringConstraint*	m_btDofSpringConstraint;
	Ref<RigidBody2>					m_bodyA;
	Ref<RigidBody2>					m_bodyB;
};

LN_NAMESPACE_END
