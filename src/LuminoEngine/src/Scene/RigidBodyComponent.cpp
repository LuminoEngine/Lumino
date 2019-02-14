
#include "Internal.hpp"
#include <LuminoEngine/Scene/RigidBodyComponent.hpp>

namespace ln {

//=============================================================================
// RigidBody2DComponent

RigidBody2DComponent::RigidBody2DComponent()
{
}

void RigidBody2DComponent::init()
{
	Component::init();
	m_body = newObject<RigidBody2D>();
}

void RigidBody2DComponent::onAttachedWorld(World* newOwner)
{
}

void RigidBody2DComponent::onDetachedWorld(World* oldOwner)
{
}

void RigidBody2DComponent::onBeforeStepSimulation()
{
}

void RigidBody2DComponent::onAfterStepSimulation()
{
}

} // namespace ln

