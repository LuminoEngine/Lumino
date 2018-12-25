
#pragma once
#include "Common.hpp"

namespace ln {

/**  */
// Context に管理され、経過時間が通知される。
// 管理しない場合は Standalone 指定で作成。その場合、自分で advanceTime する。
class AnimationClock
	: public Object
{
public:

	bool isFinished() const;

	// TODO: internal
	void advanceTime(float deltaTime);

protected:

LN_CONSTRUCT_ACCESS:
    AnimationClock();
	virtual ~AnimationClock();

private:
};

class PropertyAnimation
{
public:
    static void start();
};

} // namespace ln
