
#pragma once
#include "Rendering.h"

LN_NAMESPACE_BEGIN
class PostEffect;
class UIEventArgs;
class UIElement;
class RenderLayer;

namespace detail {

class RenderViewLayerList
{
public:
	void addRenderView(RenderLayer* renderView);

	void updateLayout(const Size& viewSize);
	void onRoutedEvent(UIEventArgs* e);
	UIElement* checkMouseHoverElement(const Point& globalPt);
	void render(DrawList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);

public:
	List<Ref<RenderLayer>>	m_viewportLayerList;

};

} // namespace detail

/**
	@brief	
*/
class WorldRenderView
	: public RenderView
{
public:
	void setLayerCullingMask(uint32_t mask) { m_layerCullingMask = mask; }
	uint32_t getLayerCullingMask() const { return m_layerCullingMask; }
	
LN_CONSTRUCT_ACCESS:
	WorldRenderView();
	virtual ~WorldRenderView();

private:
	uint32_t	m_layerCullingMask;
};

/** レイヤー背景のクリア方法を表します。 */
enum class RenderLayerClearMode
{
	None,			/**< クリアしない */
	ColorDepth,		/**< 色と深度をクリアする */
	Color,			/**< 色のみをクリアする */
	Depth,			/**< 深度のみをクリアする */
};

/**
	@brief		
*/
class RenderLayer
	//: public WorldRenderView
	: public Object
{
	//LN_OBJECT;
public:
	void addPostEffect(PostEffect* postEffect);

	void setClearMode(RenderLayerClearMode mode) { m_clearMode = mode; }
	RenderLayerClearMode getClearMode() const { return m_clearMode; }

	void setBackgroundColor(const Color& color) { m_backgroundColor = color; }
	const Color& getBackgroundColor() const { return m_backgroundColor; }

	void addChildRenderView(RenderLayer* renderView);

	WorldRenderView* getRenderView() const { return m_renderView; }

protected:
	RenderLayer();
	virtual ~RenderLayer();
	void initialize();

	virtual UIElement* hitTestUIElement(const Point& globalPt);	// TODO: globalPt じゃなくて local のほうがやりやすい
	virtual void onRoutedEvent(UIEventArgs* e);
	virtual void updateLayout(const Size& viewSize);
	virtual void renderScene(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) = 0;

private:
	void render(DrawList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
	void postRender(DrawList* context, Ref<RenderTargetTexture>* primaryLayerTarget, Ref<RenderTargetTexture>* secondaryLayerTarget);
	void updateFramebufferIfNeeded();

	RenderLayerClearMode				m_clearMode;
	Color						m_backgroundColor;
	detail::RenderViewLayerList	m_layerList;
	Ref<WorldRenderView>			m_renderView;
	List<Ref<PostEffect>>		m_postEffects;
	Ref<RenderTargetTexture>	m_primaryLayerTarget;
	Ref<RenderTargetTexture>	m_secondaryLayerTarget;
	Ref<DepthBuffer>			m_depthBuffer;
	friend class detail::RenderViewLayerList;
};




/**
	@brief
*/
class PostEffect
	: public Object
{
	LN_OBJECT;
public:
	RenderLayer* GetOwnerLayer() const { return m_ownerLayer; }

protected:
	PostEffect();
	virtual ~PostEffect();
	void initialize();

	virtual void onAttached();
	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

private:
	RenderLayer*	m_ownerLayer;
	friend class RenderLayer;
};

LN_NAMESPACE_END
