
#include "Internal.hpp"
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/Input/InputController.hpp>
#include <LuminoEngine/Physics/CollisionShape.hpp>
#include <LuminoEngine/Physics/RigidBody.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Scene/Physics/Collision.hpp>
#include <LuminoEngine/Scene/CharacterController.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include "SceneManager.hpp"
#include "../Input/InputManager.hpp"
#include "../Engine/EngineManager.hpp"

namespace ln {
    
//==============================================================================
// CharacterController
//   https://yomotsu.net/blog/2015/01/03/tps-game-engine.html
//   https://www.digifie.jp/blog/archives/1408

CharacterController::CharacterController()
    : m_inputController(nullptr)
	, m_inputControlEnabled(true)
	, m_cameraControlEnabled(true)
	, m_cameraTransformSync(true)
{
}

bool CharacterController::init()
{
    if (!Component::init()) return false;

    m_inputController = detail::EngineDomain::inputManager()->getVirtualPad(0);


    return true;
}

void CharacterController::setInputControlEnabled(bool value)
{
	m_inputControlEnabled = value;
	if (!m_inputControlEnabled) {
		m_currentVelocity = Vector3::Zero;
	}
}

void CharacterController::setCameraControlEnabled(bool value)
{
	if (m_cameraControlEnabled != value) {
		if (value)
			retainCursorGrab();
		else
			releaseCursorGrab();
		m_cameraControlEnabled = value;
	}
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

void CharacterController::setCollisionEnter(Ref<CollisionEventHandler> handler)
{
	m_onCollisionEnter.setPrimaryHandler(handler);
}

void CharacterController::setCollisionLeave(Ref<CollisionEventHandler> handler)
{
	m_onCollisionLeave.setPrimaryHandler(handler);
}

void CharacterController::setCollisionStay(Ref<CollisionEventHandler> handler)
{
	m_onCollisionStay.setPrimaryHandler(handler);
}

void CharacterController::onPreUpdate(float elapsedSeconds)
{
	prepareRigidBody();

	// 物理更新フェーズ前に、character の速度を決める
	{
		prepareViewCamera();

		WorldObject* character = worldObject();
		Camera* camera = viewCamera();

		if (m_inputControlEnabled) {
			const auto axis = Vector2::safeNormalize(Vector2(
				-m_inputController->getAxisValue(_TT("left")) + m_inputController->getAxisValue(_TT("right")),
				-m_inputController->getAxisValue(_TT("down")) + m_inputController->getAxisValue(_TT("up"))),
				Vector2::Zero);
			m_inputState.turnVelocity = axis.x;
			m_inputState.forwardVelocity = axis.y;
			//m_inputState.verticalVelocity = 0.0f;
		}
		else {
			m_inputState.turnVelocity = 0.0f;
			m_inputState.forwardVelocity = 0.0f;
			//m_inputState.verticalVelocity = 0.0f;
		}

		const auto characterCurrentFront = Vector3::transform(Vector3::UnitZ, camera->rotation());


		if (m_cameraTransformSync && m_resetCameraPosition) {
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




		Vector3 moveOffset;
		if (m_inputControlEnabled) {
			const auto cameraCurrentFront = Vector3::transform(Vector3::UnitZ, camera->rotation());
			const auto cameraCurrentFrontXZ = Vector3::safeNormalize(Vector3(cameraCurrentFront.x, 0, cameraCurrentFront.z), Vector3::UnitZ);
#ifdef LN_COORD_RH
			const auto cameraCurrentRightDirXZ = Vector3::cross(cameraCurrentFrontXZ, Vector3::UnitY);
#else
			const auto cameraCurrentRightDirXZ = Vector3::cross(Vector3::UnitY, cameraCurrentFrontXZ);
#endif
			const auto moveVector = (cameraCurrentRightDirXZ * m_inputState.turnVelocity) + (cameraCurrentFrontXZ * m_inputState.forwardVelocity);
			moveOffset = moveVector * (m_walkVelocity/* * elapsedSeconds*/);
		}
		else {
			moveOffset = m_currentVelocity;
		}

		if (m_rigidBody) {
			// 外部で character->setPosition() などで姿勢が変更されている場合に備えて、
			// 物理シミュレーション前に姿勢を剛体へ適用する。
			m_rigidBody->setTransform(character->worldMatrix());

			//m_rigidBody->setVelocity(moveOffset);
			m_rigidBody->setVelocity(Vector3(moveOffset.x, m_rigidBody->velocity().y, moveOffset.z));
		}

		m_currentVelocity = moveOffset;

		// TODO: 物理移動
		//character->setPosition(character->position() + moveOffset);
	}


}

void CharacterController::onBeforeStepSimulation_Deprecated()
{


	// Sync transform from WorldObject to Body
	//m_rigidBody->setTransform(worldObject()->worldMatrix());
}

void CharacterController::onAfterStepSimulation()
{
	if (m_rigidBody) {
		// Sync transform from Body to WorldObject
		//worldObject()->setRotation(Quaternion::makeFromRotationMatrix(m_rigidBody->transform()));
		worldObject()->setPosition(m_rigidBody->transform().position());
	}


	//WorldObject* character = worldObject();
	//Camera* camera = viewCamera();
	//camera->setPosition(character->position() + Vector3(0, 0, 10));
	//camera->lookAt(character->position());
}

void CharacterController::onUpdate(float elapsedSeconds)
{
	if (0) {
		WorldObject* character = worldObject();
		Camera* camera = viewCamera();
		camera->setPosition(m_rigidBody->transform().position() + Vector3(0, 0, 15));
		camera->lookAt(m_rigidBody->transform().position());
		return;
	}

	WorldObject* character = worldObject();
	Camera* camera = viewCamera();

	const auto cameraCurrentFront = Vector3::transform(Vector3::UnitZ, camera->rotation());
	const auto cameraCurrentFrontXZ = Vector3::safeNormalize(Vector3(cameraCurrentFront.x, 0, cameraCurrentFront.z), Vector3::UnitZ);
	const auto cameraCurrentRightDirXZ = Vector3::cross(Vector3::UnitY, cameraCurrentFrontXZ);
	//const auto moveVector = (cameraCurrentRightDirXZ * m_inputState.turnVelocity) + (cameraCurrentFrontXZ * m_inputState.forwardVelocity);

	const auto moveVector = m_currentVelocity;




	// 移動後のキャラ位置をベースに、カメラの注視点を決める
	const auto characterEyePos = character->position() + Vector3(0, m_height, 0);
	const auto cameraLookAtPos =
		characterEyePos +
		cameraCurrentRightDirXZ * m_lookAtOffset.x +
		Vector3(0, m_lookAtOffset.y, 0) +
		cameraCurrentFrontXZ * m_lookAtOffset.z;

	if (m_cameraTransformSync) {



#if 1
		{


			m_theta += m_inputState.cameraH * 0.01;
			m_phi += m_inputState.cameraV * 0.01;

			m_phi += Math::PIDiv2;
			m_phi = Math::clamp(m_phi, Math::PI * m_cameraMinTilt, Math::PI * m_cameraMaxTilt);
			m_phi -= Math::PIDiv2;


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

	}

	// 確定したカメラ向きから、キャラクターが向くべき方向を求める
	if (!moveVector.isZero()) {
		const auto characterTargetFrontDir = Vector3::transform(Vector3::UnitZ, camera->rotation());
		const auto characterTargetFrontDirXZ = Vector3::safeNormalize(Vector3(characterTargetFrontDir.x, 0, characterTargetFrontDir.z), Vector3::UnitZ);

		// TODO: ひとまず移動方向に向ける
		const auto targetDir = (1) ? Vector3::normalize(m_currentVelocity) : characterTargetFrontDirXZ;

		const auto characterCurrentFrontDir = Vector3::transform(Vector3::UnitZ, character->rotation());
		const auto characterCurrentFrontDirXZ = Vector3::safeNormalize(Vector3(characterCurrentFrontDir.x, 0, characterCurrentFrontDir.z), Vector3::UnitZ);

		const float rotDelta = Math::PI * (1.0f / m_turnTime) * elapsedSeconds;

		const float dot = Vector3::dot(characterCurrentFrontDirXZ, targetDir);
		const float rotDiff = (dot > 1.0f) ? 0.0f : std::acos(dot);	// (dot > 1.0f) は誤差対策。角度差なしとする

		if (rotDiff < rotDelta) {
			character->lookAt(character->position() + (targetDir));
		}
		else {
			
			// .y の符号で、characterCurrentFrontDirXZ を characterTargetFrontDirXZ に向けるにはどちらに回転するのが最短なのかわかる。
			// そのまま回転軸にもできる。
			const auto cross = Vector3::cross(characterCurrentFrontDirXZ, targetDir);
			const auto newDir = Vector3::transform(characterCurrentFrontDirXZ, Quaternion::makeFromRotationAxis(cross, rotDelta));


			//const auto characterTargetFrontPos = pos + (characterTargetFrontDir * m_frontFocusPointOffset);
			const auto characterTargetFrontPos = character->position() + (newDir);
			character->lookAt(characterTargetFrontPos);
		}

		//character->lookAt(character->position() + (characterTargetFrontDirXZ));


		//character->lookAt(character->position() + cameraCurrentFrontXZ);
	}


	//camera->setPosition(character->position() + Vector3(0, 0, 10));
	//camera->lookAt(character->position());


	// カメラの地形との接触判定。めり込まないようにする
	if (m_cameraTransformSync) {
	
		const auto cameraOuterDir = -camera->front();
		

		// TODO: UpdateContext みたいなの受け取って、今いる World を使いたい
		PhysicsRaycastResult result;
		if (detail::EngineDomain::engineManager()->mainPhysicsWorld()->raycast(
			cameraLookAtPos, cameraOuterDir, m_cameraRadius, 0xFFFFFFFF, &result)) {
			camera->setPosition(cameraLookAtPos + cameraOuterDir * result.distance);
		}

		//cameraLookAtPos
	}

	m_inputState.reset();
}

void CharacterController::onCollisionEnter(PhysicsObject* otherObject, ContactPoint* contact)
{
	auto* ownerComponent = reinterpret_cast<CharacterController*>(otherObject->ownerData());
	auto* worldObject = (ownerComponent) ? ownerComponent->worldObject() : nullptr;

	// TODO: Cache
	auto c = makeObject_deprecated<Collision>(worldObject, otherObject);
	m_onCollisionEnter.raise(c);
}

void CharacterController::onCollisionLeave(PhysicsObject* otherObject, ContactPoint* contact)
{
	auto* ownerComponent = reinterpret_cast<CharacterController*>(otherObject->ownerData());
	auto* worldObject = (ownerComponent) ? ownerComponent->worldObject() : nullptr;

	// TODO: Cache
	auto c = makeObject_deprecated<Collision>(worldObject, otherObject);
	m_onCollisionLeave.raise(c);
}

void CharacterController::onCollisionStay(PhysicsObject* otherObject, ContactPoint* contact)
{
	auto* ownerComponent = reinterpret_cast<CharacterController*>(otherObject->ownerData());
	auto* worldObject = (ownerComponent) ? ownerComponent->worldObject() : nullptr;

	// TODO: Cache
	auto c = makeObject_deprecated<Collision>(worldObject, otherObject);
	m_onCollisionStay.raise(c);
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
		
		if (m_cameraControlEnabled) {
			renderView->viewport()->grabCursor();
		}

		m_lastCamera = camera;
	}
}

void CharacterController::prepareRigidBody()
{
	if (!m_rigidBody) {
		auto shape = makeObject_deprecated<CapsuleCollisionShape>(0.25, m_height);
		shape->setPosition(Vector3(0, m_height * 0.5f, 0));

		m_rigidBody = makeObject_deprecated<RigidBody>();
		m_rigidBody->addCollisionShape(shape);
		m_rigidBody->setMass(50.0f);
		m_rigidBody->setAngularLimits(RigidBodyLimitFlags::LockedRotation);
		m_rigidBody->setEventListener(this);
		//m_rigidBody->setOwnerData(this);

		// TODO: UpdateContext みたいなの受け取って、今いる World に追加したい
		detail::EngineDomain::engineManager()->mainPhysicsWorld()->addPhysicsObject(m_rigidBody);
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
	if (!m_inputControlEnabled) {
		return;
	}

	if (e->type() == UIEvents::MouseMoveEvent) {
		const auto* me = static_cast<UIMouseEventArgs*>(e);
		m_inputState.cameraH = me->grabOffsetX;
		m_inputState.cameraV = -me->grabOffsetY;

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

