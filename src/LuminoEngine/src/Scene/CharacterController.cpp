
#include "Internal.hpp"
#include <LuminoEngine/Input/InputController.hpp>
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
	m_inputState.forwardVelocity = -m_inputController->getAxisValue(u"left") + m_inputController->getAxisValue(u"right");
	m_inputState.turnVelocity = -m_inputController->getAxisValue(u"up") + m_inputController->getAxisValue(u"down");


	//if (m_inputController->pressed(u"up")) {
	//	m_inputState.moveForward = true;
	//}
	//else {
	//	m_inputState.moveForward = back;
	//}
}

CharacterController::InputState::InputState()
{
	reset();
}

void CharacterController::InputState::reset()
{
	forwardVelocity = 0.0f;
	turnVelocity = 0.0f;
	//moveForward = false;
	//moveBack = false;
	//turnRight = false;
	//turnLeft = false;
	//sneak = false;
}
} // namespace ln

