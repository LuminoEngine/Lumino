
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	再生中の視覚効果を操作します。
*/
class VisualEffectInstance
	: public tr::ReflectionObject
{
	LN_OBJECT();
public:
	virtual void stop() = 0;
	virtual bool isPlaying() = 0;
	virtual void SetWorldMatrix(const Matrix& matrix) = 0;

private:
};

LN_NAMESPACE_END
