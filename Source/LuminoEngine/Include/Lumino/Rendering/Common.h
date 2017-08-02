#pragma once
#include "../Graphics/Common.h"

LN_NAMESPACE_BEGIN
namespace detail {
class DrawElementBatch;
class SubsetInfo;

class IRenderFeature
{
public:
	virtual bool isStandaloneShader() const = 0;
	virtual void flush() = 0;
	virtual void onActivated() = 0;
	virtual void onDeactivated() = 0;

	virtual void onSetState(const DrawElementBatch* state) {}
	virtual void onShaderSubsetInfoOverride(SubsetInfo* subsetInfo) {}
};

} // namespace detail
LN_NAMESPACE_END
