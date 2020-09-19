
#pragma once
#include "Common.hpp"
#include "Component.hpp"

namespace ln {
class InputController;
class UIEventArgs;
class EventConnection;
class RigidBody;

enum class CharacterControllerMode
{
	// 視点優先。
	ViewMajor,

	// キャラクター位置優先。カメラは鎖のようにキャラクターの背後に追従しようと位置調整される。
	CharacterMajor,
};

class CharacterController
	: public Component
	, protected detail::IPhysicsObjectEventListener
{
public:
	void retainCursorGrab();
	void releaseCursorGrab();

	const Ref<RigidBody>& rigidBody() const { return m_rigidBody; }

	const Vector3& velocity() const { return m_currentVelocity; }


	/** CollisionEnter イベントの通知を受け取るコールバックを登録します。*/
	void setCollisionEnter(Ref<CollisionEventHandler> handler);

	/** CollisionLeave イベントの通知を受け取るコールバックを登録します。*/
	void setCollisionLeave(Ref<CollisionEventHandler> handler);

	/** CollisionStay イベントの通知を受け取るコールバックを登録します。*/
	void setCollisionStay(Ref<CollisionEventHandler> handler);

protected:
	void onPreUpdate(float elapsedSeconds) override;
	// CharacterController を継承してカスタマイズする場合、
	// この onUpdate はキャラクターが操作を受け付ける時のみ呼び出すべき。
	// 例えば攻撃を受けてのけぞっている途中は、よびだすべきではない。
	void onUpdate(float elapsedSeconds) override;

	void onBeforeStepSimulation_Deprecated() override;
	void onAfterStepSimulation() override;
	void onCollisionEnter(PhysicsObject* otherObject, ContactPoint* contact) override;
	void onCollisionLeave(PhysicsObject* otherObject, ContactPoint* contact) override;
	void onCollisionStay(PhysicsObject* otherObject, ContactPoint* contact) override;

LN_CONSTRUCT_ACCESS:
	CharacterController();
	bool init();

private:
	struct InputState
	{
		float forwardVelocity;
		float turnVelocity;
		//bool sneak;

		float cameraH;
		float cameraV;

		InputState();
		void reset();
	};
	void resetCameraPosition();
	void prepareViewCamera();
	void prepareRigidBody();
	Camera* viewCamera() const;
	void handleUIEvent(UIEventArgs* e);

	// 複数プレイヤーをマルチビューで操作するときなどを考慮すると、
	// UIViewport の onKeyDown とかにアタッチするよりは
	// VirtualController に直接アタッチしてしまった方がシンプルに実装できる。
	// キーコンフィグも有効になるし。
	InputController* m_inputController;

	// イベントハンドラなど、入力はいったんここにすべて集めて、次の onUpdate() で処理する
	InputState m_inputState;

	Ref<Camera> m_targetCamera;

	CharacterControllerMode m_mode = CharacterControllerMode::ViewMajor;

	float m_sneakVelocity = 1.0f;
	float m_walkVelocity = 5.0f;
	float m_turnTime = 0.5f;	// 何秒で振り向きを完了するか

	// https://docs.unity3d.com/ja/2019.4/Manual/class-CharacterController.html
	float m_height = 2.0f; //0.5f;//

	// キャラクターとカメラの最大距離。
	// 鎖の要領で、この範囲内でキャラクターが動いても、カメラは位置を更新しない。
	float m_cameraRadius = 5.0f;

	// 真上からキャラを見下ろす角度が 0、真下から見上げる角度が 1
	float m_cameraMinTilt = 0.001f;
	float m_cameraMaxTilt = 0.8f;

	// Character の EyePoint からの、カメラ注視点のオフセット
	Vector3 m_lookAtOffset = Vector3(0.25, 0, 0);


	// キャラクター位置から前方向にどれだけ移動したところを注視点とするか
	float m_frontFocusPointOffset = 1.0f;

	// キャラクター及びカメラの目標注視点方向
	//Vector3 m_characterTargetFrontDirXZ;

	Vector3 m_characterTargetFrontCenterXZ;
	
	Vector3 m_cameraLookAt;


	// Z+ 方向を角度 0 とする XZ 平面 (Y軸角度。PI で、Z- 方向に配置。ワールド座標系)
	float m_theta = Math::PI;

	// (X軸角度)
	float m_phi = 0.0f;




	Camera* m_lastCamera = nullptr;
	Ref<EventConnection> m_renderViewEventConnection;
	Vector2 m_lastMousePos;
	bool m_resetCameraPosition = true;

	Event<CollisionEventHandler> m_onCollisionEnter;
	Event<CollisionEventHandler> m_onCollisionLeave;
	Event<CollisionEventHandler> m_onCollisionStay;

	Ref<RigidBody> m_rigidBody;

	Vector3 m_currentVelocity;
};

} // namespace ln
