﻿
#include "Internal.hpp"
#include <LuminoEngine/Input/InputController.hpp>
#include <LuminoEngine/Scene/CharacterController.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
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
	prepareViewCamera();

	Camera* camera = viewCamera();

	m_inputState.turnVelocity = -m_inputController->getAxisValue(u"left") + m_inputController->getAxisValue(u"right");
	m_inputState.forwardVelocity = -m_inputController->getAxisValue(u"down") + m_inputController->getAxisValue(u"up");

	const auto front = Vector3::transform(Vector3::UnitZ, camera->rotation());
	const auto frontXZ = Vector3::safeNormalize(Vector3(front.x, 0, front.z), Vector3::UnitZ);
	const auto rightXZ = Vector3::cross(Vector3::UnitY, frontXZ);
	const auto unitDir = (rightXZ * m_inputState.turnVelocity) + (frontXZ * m_inputState.forwardVelocity);

	const auto moveOffset = unitDir * (m_walkVelocity * elapsedSeconds);
	WorldObject* character = worldObject();
	Vector3 pos = character->position();
	pos += moveOffset;
	character->setPosition(pos);


	if (1) {
		// カメラも同じだけ移動する (平行移動)
		Vector3 pos = camera->position();
		pos += moveOffset;
		m_cameraLookAt += moveOffset;
		camera->setPosition(pos);
	}



	const auto characterCurrentFrontDir = Vector3::transform(Vector3::UnitZ, character->rotation());
	const auto characterCurrentFrontDirXZ = Vector3::safeNormalize(Vector3(characterCurrentFrontDir.x, 0, characterCurrentFrontDir.z), Vector3::UnitZ);


	auto characterTargetFrontDir = characterCurrentFrontDir;
	if (!unitDir.isZero()) {
		characterTargetFrontDir = Vector3::normalize(unitDir);
		m_characterTargetFrontDirXZ = Vector3::safeNormalize(Vector3(characterTargetFrontDir.x, 0, characterTargetFrontDir.z), Vector3::UnitZ);



		const float rot = Math::PI * (1.0f / m_turnTime) * elapsedSeconds;

		if (std::acos(Vector3::dot(characterCurrentFrontDirXZ, m_characterTargetFrontDirXZ)) < rot) {
			character->lookAt(pos + (characterCurrentFrontDirXZ * m_frontFocusPointOffset));
		}
		else {


			// .y の符号で、characterCurrentFrontDirXZ を characterTargetFrontDirXZ に向けるにはどちらに回転するのが最短なのかわかる。
			// そのまま回転軸にもできる。
			const auto cross = Vector3::cross(characterCurrentFrontDirXZ, m_characterTargetFrontDirXZ);
			const auto newDir = Vector3::transform(characterCurrentFrontDirXZ, Quaternion::makeFromRotationAxis(cross, rot));


			//const auto characterTargetFrontPos = pos + (characterTargetFrontDir * m_frontFocusPointOffset);
			const auto characterTargetFrontPos = pos + (newDir * m_frontFocusPointOffset);
			character->lookAt(characterTargetFrontPos);
		}

	}








	// キャラクターの頭辺りの高さの、正面をカメラの注視点とする
	const auto cameraCenter = Vector3(pos.x, pos.y/* + m_height*/, pos.z) + m_characterTargetFrontDirXZ;

	if (m_resetCameraPosition) {
		camera->setPosition(cameraCenter - characterCurrentFrontDirXZ * m_cameraRadius);
		m_cameraLookAt = cameraCenter;
		m_characterTargetFrontDirXZ = Vector3::safeNormalize(Vector3(characterTargetFrontDir.x, 0, characterTargetFrontDir.z), Vector3::UnitZ);
		m_resetCameraPosition = false;
	}

	switch (m_mode) {
		case ln::CharacterControllerMode::ViewMajor: {
			const auto cameraPos = camera->position();
			const auto dir = Vector3::safeNormalize(cameraPos - cameraCenter, Vector3::UnitZ);
			const auto lengthXZ = Vector2(dir.x, dir.z).length();
			float theta = std::atan2(dir.x, dir.z);		// Z+ 方向を角度 0 とする XZ 平面 (Y軸角度)
			float phi = -std::atan2(dir.y, lengthXZ);	// (X軸角度)


			theta += m_inputState.cameraH * 0.01;
			phi += m_inputState.cameraV * 0.01;


			;

			//const auto newDir = Vector3::transform(Vector3::UnitZ, Quaternion::makeFromYawPitchRoll(theta, phi, 0.0f));
			const auto newDir = Vector3::transform(Vector3::UnitZ, Quaternion::makeFromEulerAngles(Vector3(phi, theta, 0), RotationOrder::XYZ));
			const auto newPos = cameraCenter +  (newDir * m_cameraRadius);//Vector3::distance(cameraPos, cameraCenter);
			camera->setPosition(newPos);

			//const auto dirXZ = Vector2::safeNormalize(Vector2(cameraPos.x - cameraCenter.x, cameraPos.z - cameraCenter.z), Vector2::UnitX);
			//const auto dirXY = Vector2::safeNormalize(Vector2(cameraPos.x - cameraCenter.x, cameraPos.y - cameraCenter.y), Vector2::Zero);

			//float theta = std::atan2(dirXZ.x, dirXZ.y);	// Z+ 方向を角度 0 とする XZ 平面
			//float phi = std::atan(dirXY.y);	// 通常は y を XY 長さで除算するが、正規化済みなのでこれでよい 


			//theta += m_inputState.cameraH * 0.01;
			//phi += m_inputState.cameraV * 0.01;

			//Vector3 tpos;
			//Math::sinCos(theta, &tpos.x, &tpos.z);


			//const auto dir = Vector3::transform(Vector3::UnitZ, Quaternion::makeFromYawPitchRoll(theta, phi, 0.0f));
			//const auto newPos = dir * m_cameraRadius;//Vector3::distance(cameraPos, cameraCenter);
			//camera->setPosition(newPos);



			//const auto dirXY = Vector2::safeNormalize(Vector2(cameraPos.x - cameraCenter.x, cameraPos.y - cameraCenter.y), Vector2::UnitY);
			//const auto dirXZ = Vector2::safeNormalize(Vector2(cameraPos.x - cameraCenter.x, cameraPos.z - cameraCenter.z), Vector2::UnitX);
			//float theta = std::atan2(dirXY.y, dirXY.x);	// V
			//float phi = std::atan2(dirXZ.y, dirXZ.x);	// H

			//theta += m_inputState.cameraV * 0.01;
			//phi += m_inputState.cameraH * 0.01;

			//

			//
			//float s, c;
			//Math::sinCos(phi, &s, &c);
			//float s2 = std::sin(theta);

			//const auto dir = 

			//camera->setPosition(cameraHomePos);
			break;
		}
		case ln::CharacterControllerMode::CharacterMajor:
			LN_NOTIMPLEMENTED();
			break;
			//const auto cameraHomePos = cameraCenter - (characterFrontXZ * m_cameraRadius);
			//if (Vector3::distance(pos, camera->position()) > m_cameraRadius) {
			//	const auto diff = cameraHomePos - pos;

			//	camera->setPosition(cameraHomePos);
			//}
		default:
			LN_UNREACHABLE();
			break;
	}


	m_cameraLookAt += (cameraCenter - m_cameraLookAt) / 60.0f;


	camera->lookAt(m_cameraLookAt);

	//if (m_inputController->pressed(u"up")) {
	//	m_inputState.moveForward = true;
	//}
	//else {
	//	m_inputState.moveForward = back;
	//}

	m_inputState.reset();
}

void CharacterController::resetCameraPosition()
{
	m_resetCameraPosition = true;
}

void CharacterController::prepareViewCamera()
{
	Camera* camera = viewCamera();
	if (m_lastCamera != camera) {
		if (m_renderViewEventConnection) {
			m_renderViewEventConnection->disconnect();
		}

		m_renderViewEventConnection = camera->renderView()->connectOnUIEvent(ln::bind(this, &CharacterController::handleUIEvent));

		m_lastCamera = camera;
	}
}

Camera* CharacterController::viewCamera() const
{
	if (m_targetCamera)
		return m_targetCamera;
	else
		return detail::EngineDomain::sceneManager()->defaultCamera();
}

void CharacterController::handleUIEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseMoveEvent) {
		const auto* me = static_cast<UIMouseEventArgs*>(e);
		const auto pos = me->getPosition();
		const auto diff = pos - m_lastMousePos;

		m_inputState.cameraH = diff.x;
		m_inputState.cameraV = -diff.y;

		m_lastMousePos = pos;

		e->handled = true;
	}
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
	cameraH = 0.0f;
	cameraV = 0.0f;
}
} // namespace ln

