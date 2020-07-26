
#pragma once
#include "Component.hpp"

namespace ln {
class InputController;

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

		InputState();
		void reset();
	};

	Camera* camera() const;

	// 複数プレイヤーをマルチビューで操作するときなどを考慮すると、
	// UIViewport の onKeyDown とかにアタッチするよりは
	// VirtualController に直接アタッチしてしまった方がシンプルに実装できる。
	// キーコンフィグも有効になるし。
	InputController* m_inputController;

	InputState m_inputState;

	Ref<Camera> m_targetCamera;

	float m_sneakVelocity = 1.0f;
	float m_walkVelocity = 5.0f;
};

} // namespace ln
