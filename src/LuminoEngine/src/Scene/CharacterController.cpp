
#include "Internal.hpp"
#include <LuminoEngine/Scene/CharacterController.hpp>
#include "../Input/InputManager.hpp"

namespace ln {
    
//==============================================================================
// CharacterController

CharacterController::CharacterController()
    : m_inputController(nullptr)
{
}

bool CharacterController::init()
{
    if (!Component::init()) return false;

    m_inputController = detail::EngineDomain::inputManager()->getVirtualPad(0);


    return true;
}

void CharacterController::onUpdate(float elapsedSeconds)
{

}

CharacterController::InputState::InputState()
{
	reset();
}

void CharacterController::InputState::reset()
{
	moveForward = false;
	moveBack = false;
	turnRight = false;
	turnLeft = false;
	sneak = false;
}
} // namespace ln

