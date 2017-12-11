
#pragma once
#include "UIElement.h"
#include "../Graphics/Viewport.h"
#include "../Rendering/RenderView.h"

LN_NAMESPACE_BEGIN
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

	/** ビューポートの配置方法を設定します。*/
	void setPlacement(ViewportPlacement placement);

	/** Placement が Stretch または ViewBox である場合に使用されるビューサイズを設定します。 */
	void setBackbufferSize(int width, int height);

	void addViewportLayer(RenderView* layer);

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
	void updateFramebufferIfNeeded();
	void makeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat);

	SizeI							m_backbufferSize;
	Size							m_viewSize;
	ViewportPlacement				m_placement;

	detail::RenderViewLayerList		m_layerList;
	Ref<RenderTargetTexture>		m_primaryLayerTarget;
	Ref<DepthBuffer>				m_depthBuffer;

	// TODO: RenderView
	Ref<DrawingContext>				m_drawingContext;
	Ref<detail::NonShadingRenderer>	m_sceneRenderer;
	Ref<RenderView>					m_renderView;
};

/**
	@brief		
*/
class UILayoutLayer
	: public WorldRenderView
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
	virtual void renderScene(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;

private:
	Ref<UILayoutView>			m_root;
	Ref<DrawingContext>			m_drawingContext;
	Ref<detail::SceneRenderer>	m_internalRenderer;
};

LN_NAMESPACE_END
