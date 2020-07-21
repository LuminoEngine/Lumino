
#pragma once
#include "Component.hpp"

namespace ln {
class InputController;

class CharacterController
	: public Component
{
public:

protected:
	void onUpdate(float elapsedSeconds) override;

LN_CONSTRUCT_ACCESS:
	CharacterController();
	bool init();

private:
	struct InputState
	{
		bool moveForward;
		bool moveBack;
		bool turnRight;
		bool turnLeft;
		bool sneak;

		InputState();
		void reset();
	};

	// 複数プレイヤーをマルチビューで操作するときなどを考慮すると、
	// UIViewport の onKeyDown とかにアタッチするよりは
	// VirtualController に直接アタッチしてしまった方がシンプルに実装できる。
	// キーコンフィグも有効になるし。
	InputController* m_inputController;

	InputState m_inputState;
};

} // namespace ln
