
#pragma once
#include "UIElement.h"
#include "../Graphics/Viewport.h"

LN_NAMESPACE_BEGIN
class UIViewportLayer;
class PostEffect;
namespace detail { class InternalRenderer; }
namespace detail { class DrawElementListSet; }

/**
	@brief		
	@attention	GetSize() と GetViewSize() で得られるサイズの違いに気を付けてください。
				GetSize() は UIElement としてのサイズを返します。これは、レターボックスも含めた全体のサイズです。
				GetViewSize() はビュー内部のコンテンツのサイズ (3Dシーンなどが描画されるバックバッファのサイズ) です。
*/
class UIViewport
	: public UIElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/** */
	const Size& GetViewSize() const { return m_viewSize; }

	void SetViewBackgroundColor(const Color& color);
	const Color& GetViewBackgroundColor() const { return m_backgroundColor; }

	/** ビューポートの配置方法を設定します。*/
	void SetPlacement(ViewportPlacement placement);

	/** Placement が AutoResize ではない場合に使用されるビューサイズを設定します。 */
	void SetBackbufferSize(int width, int height);

	void AddViewportLayer(UIViewportLayer* layer);

protected:
	virtual void OnRoutedEvent(UIEventArgs* e) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual void OnRender(DrawingContext* g) override;


LN_CONSTRUCT_ACCESS:
	UIViewport();
	virtual ~UIViewport();
	void Initialize();

private:
	void UpdateFramebufferSizeIfNeeded(const SizeI& viewSize);
	void MakeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat);

	SizeI							m_backbufferSize;
	Size							m_viewSize;
	ViewportPlacement				m_placement;
	Color							m_backgroundColor;
	List<RefPtr<UIViewportLayer>>	m_viewportLayerList;
	RefPtr<RenderTargetTexture>		m_primaryLayerTarget;
	RefPtr<RenderTargetTexture>		m_secondaryLayerTarget;
	RefPtr<DepthBuffer>				m_depthBuffer;
};

/**
	@brief		
*/
class UIViewportLayer
	: public Object
{
	//LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIViewport* GetOwnerViewport() const { return m_owner; }
	void AddPostEffect(PostEffect* postEffect);

protected:
	UIViewportLayer();
	virtual ~UIViewportLayer();

	virtual void OnRoutedEvent(UIEventArgs* e);
	virtual void UpdateLayout(const Size& viewSize);
	virtual void Render() = 0;
	virtual void ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) = 0;

private:
	void PostRender(DrawList* context, RefPtr<RenderTargetTexture>* primaryLayerTarget, RefPtr<RenderTargetTexture>* secondaryLayerTarget);

	UIViewport*					m_owner;
	List<RefPtr<PostEffect>>	m_postEffects;
	friend class UIViewport;
};


/**
	@brief		
*/
class UILayoutLayer
	: public UIViewportLayer
{
	//LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UILayoutView* GetLayoutView() const;

LN_CONSTRUCT_ACCESS:
	UILayoutLayer();
	virtual ~UILayoutLayer();
	void Initialize();

protected:
	virtual void UpdateLayout(const Size& viewSize);
	virtual void Render() override;
	virtual void ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
	virtual void OnRoutedEvent(UIEventArgs* e) override;

private:
	RefPtr<UILayoutView>				m_root;
	RefPtr<DrawingContext>				m_drawingContext;
	RefPtr<detail::InternalRenderer>	m_internalRenderer;
	RefPtr<detail::DrawElementListSet>	m_drawElementListSet;
};

/**
	@brief
*/
class PostEffect
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIViewportLayer* GetOwnerLayer() const { return m_ownerLayer; }

protected:
	PostEffect();
	virtual ~PostEffect();
	void Initialize();

	virtual void OnRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

private:
	UIViewportLayer*	m_ownerLayer;
	friend class UIViewportLayer;
};

LN_NAMESPACE_END
