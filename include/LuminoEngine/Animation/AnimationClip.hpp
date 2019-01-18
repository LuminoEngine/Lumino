
#pragma once
#include "Common.hpp"

namespace ln {

/**
 * オブジェクトやそのプロパティに影響を与えるアニメーションデータです。
 *
 * スキンメッシュアニメーションでは、「歩く」「走る」といった 1 モーションの単位です。
 */
class AnimationClip
	: public Object
{
public:


protected:

LN_CONSTRUCT_ACCESS:
	AnimationClip();
	virtual ~AnimationClip();

private:
};

} // namespace ln
