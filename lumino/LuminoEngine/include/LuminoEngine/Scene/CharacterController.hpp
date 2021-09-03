
#pragma once
#include "Common.hpp"
#include "Component.hpp"

namespace ln {
class InputController;
class UIEventArgs;
class EventConnection;
class RigidBody;
class PhysicsObject;
class ContactPoint;

enum class CharacterControllerMode
{
	// 視点優先。
	ViewMajor,

	// キャラクター位置優先。カメラは鎖のようにキャラクターの背後に追従しようと位置調整される。
	CharacterMajor,
};

/**
 * CharacterController
 */
LN_CLASS()
class CharacterController
	: public Component
	, protected detail::IPhysicsObjectEventListener
{
public:
	void retainCursorGrab();
	void releaseCursorGrab();

	const Ref<RigidBody>& rigidBody() const { return m_rigidBody; }
	
	/** walkVelocity */
	LN_METHOD(Property)
	void setWalkVelocity(float value) { m_walkVelocity = value; }

	/** walkVelocity */
	LN_METHOD(Property)
	float walkVelocity() const { return m_walkVelocity; }

	// InputControlEnabled=true の時に有効
	/** setVelocity */
	LN_METHOD(Property)
	void setVelocity(const Vector3& value) { m_currentVelocity = value; }

	/** velocity */
	LN_METHOD(Property)
	const Vector3& velocity() const { return m_currentVelocity; }

	/**
	 * キーボードやゲームパッドによる操作の有効状態を設定します。 (default: true)
	 *
	 * false を指定した場合、キャラクターの想定外の自走を防止するため、速度も 0 にリセットされます。
	 */
	LN_METHOD(Property)
	void setInputControlEnabled(bool value);

	/**
	 * マウスによるカメラ操作の有効状態を設定します。 (default: true)
	 *
	 * 有効である場合、関連付けられているカメラを通じて、描画先となるビューの MouseGrab を取得します。
	 * つまり、マウスカーソルは非表示となり UI をポイントしてクリックする等の操作はできなくなります。
	 */
	LN_METHOD(Property)
	void setCameraControlEnabled(bool value);
	
	/** キャラクターの高さを設定します。この値はカプセルコライダーの高さや、カメラの注視点として使用されます。 (default: 2.0) */
	LN_METHOD(Property)
	void setHeight(float value) { m_height = value; }

	/** キャラクターの高さを取得します。 */
	LN_METHOD(Property)
	float height() const { return m_height; }

	/** キャラクターとカメラの距離を設定します。(default: 5.0) */
	LN_METHOD(Property)
	void setCameraRadius(float value) { m_cameraRadius = value; }

	/** キャラクターとカメラの距離を取得します。 */
	LN_METHOD(Property)
	float cameraRadius() const { return m_cameraRadius; }

	/** CollisionEnter イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD()
	void setCollisionEnter(Ref<CollisionEventHandler> handler);

	/** CollisionLeave イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD()
	void setCollisionLeave(Ref<CollisionEventHandler> handler);

	/** CollisionStay イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD()
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

	/** CharacterController を作成します。 */
	LN_METHOD(Property)
	bool init();

private:
	struct InputState
	{
		float forwardVelocity;
		float turnVelocity;
		//float verticalVelocity;
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
	float m_cameraRadius = 5.0f;

	// 真上からキャラを見下ろす角度が 0、真下から見上げる角度が 1
	float m_cameraMinTilt = 0.001f;
	float m_cameraMaxTilt = 0.8f;

	// Character の EyePoint からの、カメラ注視点のオフセット
	Vector3 m_lookAtOffset = Vector3(0, 0, 0);//Vector3(0.25, 0, 0);


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

	bool m_inputControlEnabled;
	bool m_cameraControlEnabled;
	bool m_cameraTransformSync;
};

} // namespace ln
