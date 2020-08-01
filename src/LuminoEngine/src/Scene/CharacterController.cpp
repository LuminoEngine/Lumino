
#include "Internal.hpp"
#include <LuminoEngine/UI/UIViewport.hpp>
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

void CharacterController::retainCursorGrab()
{
	if (Camera* camera = viewCamera()) {
		WorldRenderView* renderView = camera->renderView();
		renderView->viewport()->grabCursor();
	}
}

void CharacterController::releaseCursorGrab()
{
	if (Camera* camera = viewCamera()) {
		WorldRenderView* renderView = camera->renderView();
		renderView->viewport()->releaseCursor();
	}
}

void CharacterController::onUpdate(float elapsedSeconds)
{
	prepareViewCamera();

	WorldObject* character = worldObject();
	Camera* camera = viewCamera();

	m_inputState.turnVelocity = -m_inputController->getAxisValue(u"left") + m_inputController->getAxisValue(u"right");
	m_inputState.forwardVelocity = -m_inputController->getAxisValue(u"down") + m_inputController->getAxisValue(u"up");

	const auto characterCurrentFront = Vector3::transform(Vector3::UnitZ, camera->rotation());
	

	if (m_resetCameraPosition) {
		// Character を真後ろから見るようにカメラを移動する。
		// この後の計算で、カメラ正面方向を使い、位置と注視点は再設定されるため、オフセットは気にしなくてよい。
		camera->setPosition(character->position() - characterCurrentFront);
		camera->lookAt(character->position());
		m_theta = Math::PI;
		m_phi = 0.0f;
		//m_cameraLookAt = cameraCenter;
		//m_characterTargetFrontDirXZ = Vector3::safeNormalize(Vector3(characterTargetFrontDir.x, 0, characterTargetFrontDir.z), Vector3::UnitZ);
		m_resetCameraPosition = false;
	}




	const auto cameraCurrentFront = Vector3::transform(Vector3::UnitZ, camera->rotation());
	const auto cameraCurrentFrontXZ = Vector3::safeNormalize(Vector3(cameraCurrentFront.x, 0, cameraCurrentFront.z), Vector3::UnitZ);
	const auto cameraCurrentRightDirXZ = Vector3::cross(Vector3::UnitY, cameraCurrentFrontXZ);
	const auto moveVector = (cameraCurrentRightDirXZ * m_inputState.turnVelocity) + (cameraCurrentFrontXZ * m_inputState.forwardVelocity);
	const auto moveOffset = moveVector * (m_walkVelocity * elapsedSeconds);



	// TODO: 物理移動
	character->setPosition(character->position() + moveOffset);


	// 移動後のキャラ位置をベースに、カメラの注視点を決める
	const auto characterEyePos = character->position() + Vector3(0, m_height, 0);
	const auto cameraLookAtPos =
		characterEyePos +
		cameraCurrentRightDirXZ * m_lookAtOffset.x +
		Vector3(0, m_lookAtOffset.y, 0) +
		cameraCurrentFrontXZ * m_lookAtOffset.z;


#if 1
	{


		m_theta += m_inputState.cameraH * 0.01;
		m_phi += m_inputState.cameraV * 0.01;



		//const auto newDir = Vector3::transform(Vector3::UnitZ, Quaternion::makeFromYawPitchRoll(theta, phi, 0.0f));
		const auto newDir = Vector3::transform(Vector3::UnitZ, Quaternion::makeFromEulerAngles(Vector3(m_phi, m_theta, 0), RotationOrder::XYZ));
		const auto newPos = cameraLookAtPos + (newDir * m_cameraRadius);//Vector3::distance(cameraPos, cameraCenter);
		camera->setPosition(newPos);
	}

#else	// 毎フレーム、現在位置から theta と phi を求める方式。この場合キャラ移動に併せて徐々に背面に近づいていくので、
		// ジョイパッドで操作するようなときに自然に追従していくように動く。
	{

		const auto cameraPos = camera->position();
		const auto dir = Vector3::safeNormalize(cameraPos - cameraLookAtPos, Vector3::UnitZ);
		const auto lengthXZ = Vector2(dir.x, dir.z).length();
		float theta = std::atan2(dir.x, dir.z);		// Z+ 方向を角度 0 とする XZ 平面 (Y軸角度)
		float phi = -std::atan2(dir.y, lengthXZ);	// X軸角度。-PI:真上から見下ろす ~ 0:水平 ~ PI:直下から見上げる


		theta += m_inputState.cameraH * 0.01;
		phi += m_inputState.cameraV * 0.01;



		//const auto newDir = Vector3::transform(Vector3::UnitZ, Quaternion::makeFromYawPitchRoll(theta, phi, 0.0f));
		const auto newDir = Vector3::transform(Vector3::UnitZ, Quaternion::makeFromEulerAngles(Vector3(phi, theta, 0), RotationOrder::XYZ));
		const auto newPos = cameraLookAtPos + (newDir * m_cameraRadius);//Vector3::distance(cameraPos, cameraCenter);
		camera->setPosition(newPos);
	}
#endif






	//camera->setPosition(cameraLookAtPos - cameraCurrentFrontXZ * m_cameraRadius);
	camera->lookAt(cameraLookAtPos);


	// 確定したカメラ向きから、キャラクターが向くべき方向を求める
	if (!moveVector.isZero()) {
		const auto characterTargetFrontDir = Vector3::transform(Vector3::UnitZ, camera->rotation());
		const auto characterTargetFrontDirXZ = Vector3::safeNormalize(Vector3(characterTargetFrontDir.x, 0, characterTargetFrontDir.z), Vector3::UnitZ);

		const auto characterCurrentFrontDir = Vector3::transform(Vector3::UnitZ, character->rotation());
		const auto characterCurrentFrontDirXZ = Vector3::safeNormalize(Vector3(characterCurrentFrontDir.x, 0, characterCurrentFrontDir.z), Vector3::UnitZ);

		const float rotDelta = Math::PI * (1.0f / m_turnTime) * elapsedSeconds;

		const float dot = Vector3::dot(characterCurrentFrontDirXZ, characterTargetFrontDirXZ);
		const float rotDiff = (dot > 1.0f) ? 0.0f : std::acos(dot);	// (dot > 1.0f) は誤差対策。角度差なしとする
		printf("rotDiff:%f, d:%f\n", rotDiff, dot);

		if (rotDiff < rotDelta) {
			character->lookAt(character->position() + (characterTargetFrontDirXZ));
			printf("fixed\n");
		}
		else {
			printf("rotationg\n");


			// .y の符号で、characterCurrentFrontDirXZ を characterTargetFrontDirXZ に向けるにはどちらに回転するのが最短なのかわかる。
			// そのまま回転軸にもできる。
			const auto cross = Vector3::cross(characterCurrentFrontDirXZ, characterTargetFrontDirXZ);
			const auto newDir = Vector3::transform(characterCurrentFrontDirXZ, Quaternion::makeFromRotationAxis(cross, rotDelta));


			//const auto characterTargetFrontPos = pos + (characterTargetFrontDir * m_frontFocusPointOffset);
			const auto characterTargetFrontPos = character->position() + (newDir);
			character->lookAt(characterTargetFrontPos);
		}

		character->lookAt(character->position() + (characterTargetFrontDirXZ));


		//character->lookAt(character->position() + cameraCurrentFrontXZ);
	}

#if 0

	const auto cameraCurrentFront = Vector3::transform(Vector3::UnitZ, camera->rotation());
	const auto cameraCurrentFrontXZ = Vector3::safeNormalize(Vector3(cameraCurrentFront.x, 0, cameraCurrentFront.z), Vector3::UnitZ);
	const auto rightXZ = Vector3::cross(Vector3::UnitY, cameraCurrentFrontXZ);
	const auto cameraCurrentHorizontalDir = rightXZ;
	const auto unitDir = (rightXZ * m_inputState.turnVelocity) + (cameraCurrentFrontXZ * m_inputState.forwardVelocity);

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
	const auto cameraCenter = Vector3(pos.x, pos.y + m_height, pos.z) +
		cameraCurrentFront +			// カメラから見て、キャラクターの少し向こう側を注視点とする。
		(cameraCurrentHorizontalDir * m_inputState.turnVelocity);	// 左右入力があれば、少し視点をずらす
	//if (m_inputState.forwardVelocity != 0.0f) {
	//	cameraCenter
	//}


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
#endif

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

		WorldRenderView* renderView = camera->renderView();
		m_renderViewEventConnection = renderView->connectOnUIEvent(ln::bind(this, &CharacterController::handleUIEvent));
		renderView->viewport()->grabCursor();

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
		//const auto pos = me->getPosition();
		//const auto diff = pos - m_lastMousePos;

		//std::cout << pos.x << "," << pos.y << std::endl;
		//std::cout << me->grabOffsetX << "," << me->grabOffsetY << std::endl;

		//m_inputState.cameraH = diff.x;
		//m_inputState.cameraV = -diff.y;
		m_inputState.cameraH = me->grabOffsetX;
		m_inputState.cameraV = -me->grabOffsetY;

		//m_lastMousePos = pos;

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

