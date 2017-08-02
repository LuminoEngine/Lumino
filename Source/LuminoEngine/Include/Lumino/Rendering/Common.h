#pragma once
#include "../Graphics/Common.h"

LN_NAMESPACE_BEGIN
namespace detail {
class DrawElementBatch;
struct ElementInfo;
struct SubsetInfo;

class IRenderFeature
{
public:
	virtual bool isStandaloneShader() const = 0;
	virtual void flush() = 0;
	virtual void onActivated() = 0;
	virtual void onDeactivated() = 0;

	virtual void onSetState(const DrawElementBatch* state) {}

	// RenderFeature の都合で、フレームワークがシェーダに渡す情報に制限をかけたい場合に使用する
	virtual void onShaderElementInfoOverride(ElementInfo* elementInfo) {}
	virtual void onShaderSubsetInfoOverride(SubsetInfo* subsetInfo) {}
};

} // namespace detail
LN_NAMESPACE_END
