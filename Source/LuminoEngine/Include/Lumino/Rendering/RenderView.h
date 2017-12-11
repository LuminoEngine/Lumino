
#pragma once
#include "Rendering.h"

LN_NAMESPACE_BEGIN
class PostEffect;
class UIEventArgs;
class UIElement;
class RenderView;

namespace detail {

class RenderViewLayerList
{
public:
	void addRenderView(RenderView* renderView);

	void updateLayout(const Size& viewSize);
	void onRoutedEvent(UIEventArgs* e);
	UIElement* checkMouseHoverElement(const Point& globalPt);
	void render(DrawList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);

public:
	List<Ref<RenderView>>	m_viewportLayerList;

};

} // namespace detail


/** レイヤー背景のクリア方法を表します。 */
enum class RenderLayerClearMode
{
	None,			/**< クリアしない */
	ColorDepth,		/**< 色と深度をクリアする */
	Color,			/**< 色のみをクリアする */
	Depth,			/**< 深度のみをクリアする */
};

/**
	@brief	ある視点を起点としたレンダリングのエントリーポイントとなるビューを表します。
*/
class RenderView
	: public Object
{
public:
	// 描画リストと視点情報のまとまり。
	// ある1つの視点から、複数の描画リストを結合して描画するために使用する。

	// 今のところ、DrawList 経由でビューサイズや視点情報を、Particle などが知るために用意している。


	List<detail::DrawElementList*>	m_lists;
	detail::CameraInfo				m_cameraInfo;

	// 作業用
	//List<detail::DrawElement*>				m_renderingElementList;


	const ln::Size& getViewSize() const { return m_viewSize; }
	
	detail::SceneRenderer* getSceneRenderer() { return m_sceneRenderer; }
	void setSceneRenderer(detail::SceneRenderer* sr) { m_sceneRenderer = sr; }


	virtual void filterWorldMatrix(Matrix* outMatrix);
	



	void addPostEffect(PostEffect* postEffect);

	void setClearMode(RenderLayerClearMode mode) { m_clearMode = mode; }
	RenderLayerClearMode getClearMode() const { return m_clearMode; }

	void setBackgroundColor(const Color& color) { m_backgroundColor = color; }
	const Color& getBackgroundColor() const { return m_backgroundColor; }

	void addChildRenderView(RenderView* renderView);

protected:
	virtual UIElement* hitTestUIElement(const Point& globalPt);	// TODO: globalPt じゃなくて local のほうがやりやすい
	virtual void onRoutedEvent(UIEventArgs* e);
	virtual void updateLayout(const Size& viewSize);
	virtual void renderScene(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) {};


LN_CONSTRUCT_ACCESS:
	RenderView();
	virtual ~RenderView();

LN_INTERNAL_ACCESS:
	void setViewSize(const ln::Size& size) { m_viewSize = size; }

private:
	ln::Size    m_viewSize;
	detail::SceneRenderer*	m_sceneRenderer;

	void render(DrawList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
	void postRender(DrawList* context, Ref<RenderTargetTexture>* primaryLayerTarget, Ref<RenderTargetTexture>* secondaryLayerTarget);
	void updateFramebufferIfNeeded();

	RenderLayerClearMode				m_clearMode;
	Color						m_backgroundColor;
	detail::RenderViewLayerList	m_layerList;
	List<Ref<PostEffect>>		m_postEffects;
	Ref<RenderTargetTexture>	m_primaryLayerTarget;
	Ref<RenderTargetTexture>	m_secondaryLayerTarget;
	Ref<DepthBuffer>			m_depthBuffer;
	friend class detail::RenderViewLayerList;
};

/**
	@brief	
*/
class WorldRenderViewBase
	: public RenderView
{
public:
	void setLayerCullingMask(uint32_t mask) { m_layerCullingMask = mask; }
	uint32_t getLayerCullingMask() const { return m_layerCullingMask; }
	
LN_CONSTRUCT_ACCESS:
	WorldRenderViewBase();
	virtual ~WorldRenderViewBase();

private:
	uint32_t	m_layerCullingMask;
};

///**
//	@brief		
//*/
//class RenderLayer
//	: public WorldRenderViewBase
//{
//	//LN_OBJECT;
//public:
//	void addPostEffect(PostEffect* postEffect);
//
//	void setClearMode(RenderLayerClearMode mode) { m_clearMode = mode; }
//	RenderLayerClearMode getClearMode() const { return m_clearMode; }
//
//	void setBackgroundColor(const Color& color) { m_backgroundColor = color; }
//	const Color& getBackgroundColor() const { return m_backgroundColor; }
//
//	void addChildRenderView(RenderLayer* renderView);
//
//protected:
//	RenderLayer();
//	virtual ~RenderLayer();
//
//	virtual UIElement* hitTestUIElement(const Point& globalPt);	// TODO: globalPt じゃなくて local のほうがやりやすい
//	virtual void onRoutedEvent(UIEventArgs* e);
//	virtual void updateLayout(const Size& viewSize);
//	virtual void renderScene(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) = 0;
//
//private:
//	void render(DrawList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
//	void postRender(DrawList* context, Ref<RenderTargetTexture>* primaryLayerTarget, Ref<RenderTargetTexture>* secondaryLayerTarget);
//	void updateFramebufferIfNeeded();
//
//	RenderLayerClearMode				m_clearMode;
//	Color						m_backgroundColor;
//	detail::RenderViewLayerList	m_layerList;
//	List<Ref<PostEffect>>		m_postEffects;
//	Ref<RenderTargetTexture>	m_primaryLayerTarget;
//	Ref<RenderTargetTexture>	m_secondaryLayerTarget;
//	Ref<DepthBuffer>			m_depthBuffer;
//	friend class detail::RenderViewLayerList;
//};




/**
	@brief
*/
class PostEffect
	: public Object
{
	LN_OBJECT;
public:
	RenderView * GetOwnerLayer() const { return m_ownerLayer; }

protected:
	PostEffect();
	virtual ~PostEffect();
	void initialize();

	virtual void onAttached();
	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

private:
	RenderView *	m_ownerLayer;
	friend class RenderView;
};

LN_NAMESPACE_END
