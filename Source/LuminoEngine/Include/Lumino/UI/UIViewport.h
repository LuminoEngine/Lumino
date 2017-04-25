
#pragma once
#include "UIElement.h"
#include "../Graphics/Viewport.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIViewportLayer
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

protected:

LN_PROTECTED_INTERNAL_ACCESS:
	UIViewportLayer();
	virtual ~UIViewportLayer();

	//void UpdateTransform(const Size& viewSize);
	//virtual void Render() = 0;
	//void PostRender(DrawList* context, RefPtr<RenderTargetTexture>* primaryLayerTarget, RefPtr<RenderTargetTexture>* secondaryLayerTarget);

	//virtual void ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);

private:
	Viewport*					m_owner;
	Size						m_size;
	RefPtr<ImageEffectList>		m_imageEffects;
	int							m_zIndex;

	friend class Viewport;
};

/**
	@brief		
*/
class UIViewport
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:

	/** Placement が AutoResize ではない場合に使用されるビューサイズを設定します。 */
	void SetBackbufferSize(int width, int height);

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

	SizeI						m_backbufferSize;
	ViewportPlacement			m_placement;
	Color						m_backgroundColor;
	RefPtr<RenderTargetTexture>	m_primaryLayerTarget;
	RefPtr<RenderTargetTexture>	m_secondaryLayerTarget;
	RefPtr<DepthBuffer>			m_depthBuffer;
};

LN_NAMESPACE_END
