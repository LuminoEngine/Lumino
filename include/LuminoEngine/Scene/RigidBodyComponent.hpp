
#pragma once
#include "Component.hpp"
#include "../Physics/PhysicsWorld2D.hpp"

namespace ln {

class RigidBody2DComponent
	: public Component
	, protected detail::IPhysicsObjectEventListener
{
public:

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
