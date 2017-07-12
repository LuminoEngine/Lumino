#pragma once
#include "../Graphics/Common.h"

LN_NAMESPACE_BEGIN
namespace detail {
class DrawElementBatch;

class IRenderFeature
{
public:
	virtual bool isStandaloneShader() const = 0;
	virtual void flush() = 0;
	virtual void onActivated() = 0;
	virtual void onDeactivated() = 0;

	virtual void onSetState(const DrawElementBatch* state) {}
};

} // namespace detail
LN_NAMESPACE_END
