
#include "Internal.hpp"
#include <LuminoEngine/Input/InputController.hpp>
#include <LuminoEngine/Scene/CharacterController.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include "SceneManager.hpp"
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
	m_inputState.turnVelocity = -m_inputController->getAxisValue(u"left") + m_inputController->getAxisValue(u"right");
	m_inputState.forwardVelocity = -m_inputController->getAxisValue(u"down") + m_inputController->getAxisValue(u"up");

	const auto front = Vector3::transform(Vector3::UnitZ, camera()->rotation());
	const auto frontXZ = Vector3::safeNormalize(Vector3(front.x, 0, front.z), Vector3::UnitZ);
	const auto rightXZ = Vector3::cross(Vector3::UnitY, frontXZ);;
	const auto unitDir = (rightXZ * m_inputState.turnVelocity) + (frontXZ * m_inputState.forwardVelocity);

	WorldObject* obj = worldObject();
	Vector3 pos = obj->position();
	pos += unitDir * (m_walkVelocity * elapsedSeconds);
	obj->setPosition(pos);

	//if (m_inputController->pressed(u"up")) {
	//	m_inputState.moveForward = true;
	//}
	//else {
	//	m_inputState.moveForward = back;
	//}
}

Camera* CharacterController::camera() const
{
	if (m_targetCamera)
		return m_targetCamera;
	else
		return detail::EngineDomain::sceneManager()->defaultCamera();
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

