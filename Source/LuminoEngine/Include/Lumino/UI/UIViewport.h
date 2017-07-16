
#pragma once
#include "UIElement.h"
#include "../Graphics/Viewport.h"

LN_NAMESPACE_BEGIN
class UIViewportLayer;
class PostEffect;
namespace detail { class SceneRenderer; }
namespace detail { class NonShadingRenderer; }
class RenderView;

/**
	@brief		
	@attention	getSize() と getViewSize() で得られるサイズの違いに気を付けてください。
				getSize() は UIElement としてのサイズを返します。これは、レターボックスも含めた全体のサイズです。
				getViewSize() はビュー内部のコンテンツのサイズ (3Dシーンなどが描画されるバックバッファのサイズ) です。
*/
class UIViewport
	: public UIElement
{
	LN_OBJECT;
public:

	/** */
	const Size& getViewSize() const { return m_viewSize; }

	void setViewBackgroundColor(const Color& color);
	const Color& getViewBackgroundColor() const { return m_backgroundColor; }

	/** ビューポートの配置方法を設定します。*/
	void setPlacement(ViewportPlacement placement);

	/** Placement が Stretch または ViewBox である場合に使用されるビューサイズを設定します。 */
	void setBackbufferSize(int width, int height);

	void addViewportLayer(UIViewportLayer* layer);

protected:
	virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRender(DrawingContext* g) override;

	virtual UIElement* checkMouseHoverElement(const Point& globalPt) override;

LN_CONSTRUCT_ACCESS:
	UIViewport();
	virtual ~UIViewport();
	void initialize();

private:
	void updateFramebufferSizeIfNeeded();
	void makeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat);

	SizeI							m_backbufferSize;
	Size							m_viewSize;
	ViewportPlacement				m_placement;
	Color							m_backgroundColor;
	List<Ref<UIViewportLayer>>	m_viewportLayerList;
	Ref<RenderTargetTexture>		m_primaryLayerTarget;
	Ref<RenderTargetTexture>		m_secondaryLayerTarget;
	Ref<DepthBuffer>				m_depthBuffer;

	// TODO: RenderView
	Ref<DrawingContext>				m_drawingContext;
	Ref<detail::NonShadingRenderer>	m_sceneRenderer;
	Ref<RenderView>					m_renderView;
};

/**
	@brief		
*/
class UIViewportLayer
	: public Object
{
	//LN_OBJECT;
public:
	UIViewport* getOwnerViewport() const { return m_owner; }
	void addPostEffect(PostEffect* postEffect);

protected:
	UIViewportLayer();
	virtual ~UIViewportLayer();

	virtual UIElement* hitTestUIElement(const Point& globalPt);	// TODO: globalPt じゃなくて local のほうがやりやすい
	virtual void onRoutedEvent(UIEventArgs* e);
	virtual void updateLayout(const Size& viewSize);
	virtual void render(bool clearColorBuffer) = 0;
	virtual void executeDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer, bool clearColorBuffer) = 0;

private:
	void postRender(DrawList* context, Ref<RenderTargetTexture>* primaryLayerTarget, Ref<RenderTargetTexture>* secondaryLayerTarget);

	UIViewport*					m_owner;
	List<Ref<PostEffect>>	m_postEffects;
	friend class UIViewport;
};


/**
	@brief		
*/
class UILayoutLayer
	: public UIViewportLayer
{
	//LN_OBJECT;
public:
	UILayoutView* GetLayoutView() const;

LN_CONSTRUCT_ACCESS:
	UILayoutLayer();
	virtual ~UILayoutLayer();
	void initialize();

protected:
	virtual UIElement* hitTestUIElement(const Point& globalPt) override;
	virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual void updateLayout(const Size& viewSize) override;
	virtual void render(bool clearColorBuffer) override;
	virtual void executeDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer, bool clearColorBuffer) override;

private:
	Ref<UILayoutView>				m_root;
	Ref<DrawingContext>				m_drawingContext;
	Ref<detail::SceneRenderer>	m_internalRenderer;
	Ref<RenderView>	m_drawElementListSet;
};

/**
	@brief
*/
class PostEffect
	: public Object
{
	LN_OBJECT;
public:
	UIViewportLayer* GetOwnerLayer() const { return m_ownerLayer; }

protected:
	PostEffect();
	virtual ~PostEffect();
	void initialize();

	virtual void onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

private:
	UIViewportLayer*	m_ownerLayer;
	friend class UIViewportLayer;
};

LN_NAMESPACE_END
