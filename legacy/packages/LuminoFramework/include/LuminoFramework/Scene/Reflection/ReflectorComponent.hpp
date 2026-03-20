
#pragma once
#include "../../Visual/VisualComponent.hpp"

namespace ln {
class OffscreenWorldRenderView;

LN_CLASS()
class ReflectorComponent
	: public VisualComponent
{
	LN_OBJECT;
public:
	const Ref<RenderTargetTexture>& renderTarget() const { return m_renderTarget; }

protected:
	void onPrepareRender(RenderingContext* context) override;
    void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	ReflectorComponent();
	virtual ~ReflectorComponent() = default;
	bool init();

private:
	Ref<RenderTargetTexture> m_renderTarget;
	Ref<OffscreenWorldRenderView> m_offscreenRenderView;
	Ref<Material> m_material;
	Vector2 m_size;
};

} // namespace ln
