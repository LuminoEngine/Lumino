
#pragma once

#include "Common.h"
#include "PhysicsManager.h"

namespace Lumino
{
namespace Physics
{

/// ジョイントのベースクラス
class Joint
    : public RefObject
{
protected:
	Joint();
	virtual ~Joint();
	void Create(PhysicsManager* manager, btTypedConstraint* constraint);

public:
	btTypedConstraint* GetBtConstraint() { return m_btConstraint; }

	/// シミュレーション直前更新処理 (メインスレッドから呼ばれる)
	virtual void SyncBeforeStepSimulation() {}

	/// シミュレーション直後更新処理 (メインまたは物理更新スレッドから呼ばれる)
	virtual void SyncAfterStepSimulation() {}

protected:
	PhysicsManager*		m_manager;
    btTypedConstraint*	m_btConstraint;
};

/// DofSpringJoint
///		現状、set 系は初期値 set でしか使わないので遅延設定の対応は今のところ必要ない。
///		代わりに、1度でも StepSimulation() した後に set しようとしたら例外になる。
class DofSpringJoint
	: public Joint
{
public:
	DofSpringJoint();
    virtual ~DofSpringJoint();

public:
	void Create(PhysicsManager* manager, RigidBody* body0, RigidBody* body1, const Matrix& localOffset0, const Matrix& localOffset1);

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

protected:
	virtual void SyncBeforeStepSimulation();
	virtual void SyncAfterStepSimulation() {}

private:

	//enum ModifiedFlags
	//{
	//	Modified_None = 0x0000,

	//	Modified_EnableSpring = 0x0001,
	//	Modified_Stiffness = 0x0002,
	//	Modified_Damping = 0x0004,
	//	Modified_EquilibriumPoint = 0x0008,

	//	Modified_Limit = 0x0010,
	//	Modified_LinearLowerLimit= 0x0020,
	//	Modified_LinearUpperLimit = 0x0040,
	//	Modified_AngularLowerLimit = 0x0080,
	//	Modified_AngularUpperLimit = 0x0100,
	//};

	btGeneric6DofSpringConstraint*	m_btDofSpringConstraint;
	RigidBody*		m_body0;
	RigidBody*		m_body1;

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

} // namespace Physics
} // namespace Lumino
