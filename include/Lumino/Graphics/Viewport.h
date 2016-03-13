
#pragma once
#include "Texture.h"

LN_NAMESPACE_BEGIN

class RenderingCompositionContext
{
public:
	RenderTarget* RequestRenderTarget(int width, int height);
};

//// Camera Ç∆Ç©Ç™åpè≥Ç∑ÇÈ
//class IRenderingCompositionItem
//{
//public:
//	//void Render(RenderingCompositionContext* context);
//};

class ViewportLayer
	: public Object
{
public:
	///// ëOï`âÊ
	//virtual void PreRender(const SizeF& viewSize) {}

	/// ñ{ï`âÊ
	virtual void Render(RenderTarget* renderTarget) {}

	/// å„ï`âÊ
	//virtual void PostRender() {}
};

/**
	@brief		
*/
class Viewport
	: public Object
{
public:

	static Viewport* GetMainWindowViewport();

	void SetBackgroundColor(const Color& color);

	void AddViewportLayer(ViewportLayer* layer) { m_viewportLayerList.Add(layer); }
	void RemoveViewportLayer(ViewportLayer* layer) { m_viewportLayerList.Remove(RefPtr<ViewportLayer>(layer)); }

LN_INTERNAL_ACCESS:	// TODO: Ç¢Ç‹ÇÕÇ∆ÇËÇ†Ç¶Ç∏ì‡ïîópìr
	Viewport();
	virtual ~Viewport();
	void Initialize(GraphicsManager* manager, RenderTarget* renderTarget);
	void Render();

private:
	GraphicsManager*				m_manager;
	RenderTarget*					m_renderTarget;
	Array<RefPtr<ViewportLayer>>	m_viewportLayerList;
	ColorF							m_backgroundColor;
};

LN_NAMESPACE_END
