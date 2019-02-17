
#pragma once
#include "Component.hpp"
#include "../Physics/PhysicsWorld2D.hpp"

namespace ln {

class RigidBody2DComponent
	: public Component
	, protected detail::IPhysicsObjectEventListener
{
public:
    void setVelocity(const Vector2& value) { m_body->setVelocity(value); }

    const Vector2& velocity() const { return m_body->velocity(); }

    void setMass(float value);
    void addCollisionShape(CollisionShape2D* shape);

    void setFixedRotation(bool value) { m_body->setFixedRotation(value); }

    /** 重心に力を加えます。 */
    void applyForce(const Vector2& force) { m_body->applyForce(force); }

    /** 指定したローカル位置に力を加えます。 */
    void applyForce(const Vector2& force, const Vector2& localPosition) { m_body->applyForce(force, localPosition); }

    /** 重心に衝撃を与えます。 */
    void applyImpulse(const Vector2& impulse) { m_body->applyImpulse(impulse); }

    /** 指定したローカル位置に衝撃を与えます。 */
    void applyImpulse(const Vector2& impulse, const Vector2& localPosition) { m_body->applyImpulse(impulse, localPosition); }

    /** トルクをかけます。 */
    void applyTorque(float torque) { m_body->applyTorque(torque); }

    /** トルク衝撃を与えます。 */
    void applyTorqueImpulse(float torque) { m_body->applyTorqueImpulse(torque); }

LN_CONSTRUCT_ACCESS:
	RigidBody2DComponent();
	virtual ~RigidBody2DComponent() = default;
	void init();

protected:
	virtual void onAttachedWorld(World* newOwner) override;
	virtual void onDetachedWorld(World* oldOwner) override;
	virtual void onBeforeStepSimulation() override;
	virtual void onAfterStepSimulation() override;

private:
	Ref<RigidBody2D> m_body;
};

} // namespace ln
