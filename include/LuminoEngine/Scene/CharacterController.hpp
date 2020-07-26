
#pragma once
#include "Component.hpp"

namespace ln {
class InputController;
class UIEventArgs;
class EventConnection;

enum class CharacterControllerMode
{
	// 視点優先。
	ViewMajor,

	// キャラクター位置優先。カメラは鎖のようにキャラクターの背後に追従しようと位置調整される。
	CharacterMajor,
};

class CharacterController
	: public Component
{
public:

protected:
	// CharacterController を継承してカスタマイズする場合、
	// この onUpdate はキャラクターが操作を受け付ける時のみ呼び出すべき。
	// 例えば攻撃を受けてのけぞっている途中は、よびだすべきではない。
	void onUpdate(float elapsedSeconds) override;

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
	float m_turnTime = 0.25f;	// 何秒で振り向きを完了するか

	// https://docs.unity3d.com/ja/2019.4/Manual/class-CharacterController.html
	float m_height = 2.0f;

	// キャラクターとカメラの最大距離。
	// 鎖の要領で、この範囲内でキャラクターが動いても、カメラは位置を更新しない。
	float m_cameraRadius = 5.0f;

	// キャラクター位置から前方向にどれだけ移動したところを注視点とするか
	float m_frontFocusPointOffset = 1.0f;

	// キャラクター及びカメラの目標注視点方向
	Vector3 m_characterTargetFrontDirXZ;
	
	Vector3 m_cameraLookAt;


	Camera* m_lastCamera = nullptr;
	Ref<EventConnection> m_renderViewEventConnection;

	Vector2 m_lastMousePos;

	bool m_resetCameraPosition = true;
};

} // namespace ln
