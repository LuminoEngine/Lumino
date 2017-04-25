
#pragma once
#include "UIElement.h"
#include "../Graphics/Viewport.h"

LN_NAMESPACE_BEGIN
class UIViewportLayer;

/**
	@brief		
	@attention	GetSize() と GetViewSize() で得られるサイズの違いに気を付けてください。
				GetSize() は UIElement としてのサイズを返します。これは、レターボックスも含めた全体のサイズです。
				GetViewSize() はビュー内部のコンテンツのサイズ (3Dシーンなどが描画されるバックバッファのサイズ) です。
*/
class UIViewport
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:

	/** */
	const Size& GetViewSize() const { return m_viewSize; }

	void SetViewBackgroundColor(const Color& color);

	/** ビューポートの配置方法を設定します。*/
	void SetPlacement(ViewportPlacement placement);

	/** Placement が AutoResize ではない場合に使用されるビューサイズを設定します。 */
	void SetBackbufferSize(int width, int height);

	void AddViewportLayer(UIViewportLayer* layer);

protected:
	virtual bool OnEvent(detail::UIInternalEventType type, UIEventArgs* args) override;
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

protected:
	UIViewportLayer();
	virtual ~UIViewportLayer();

	virtual void Render() = 0;
	virtual void ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) = 0;

private:
	UIViewport*	m_owner;
	friend class UIViewport;
};

LN_NAMESPACE_END
