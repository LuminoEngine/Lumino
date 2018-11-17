
#pragma once
#include "Common.hpp"

namespace ln {

/**  */
class AnimationClock
	: public Object
{
public:


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
