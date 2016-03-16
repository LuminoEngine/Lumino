
#pragma once
#include "Texture.h"
#include "ImageEffect/ImageEffect.h"

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
	ImageEffectList* GetImageEffects() const { return m_imageEffects; }

protected:
	ViewportLayer();
	virtual ~ViewportLayer();

	///// ëOï`âÊ
	//virtual void PreRender(const SizeF& viewSize) {}

	/// ñ{ï`âÊ
	virtual void Render(RenderTarget* renderTarget) = 0;

	/// å„ï`âÊ
	void PostRender(RenderingContext2* renderingContext, RenderTarget** primaryLayerTarget, RenderTarget** secondaryLayerTarget);

private:
	RefPtr<ImageEffectList>	m_imageEffects;

	friend class Viewport;
};

/**
	@brief		
*/
class Viewport
	: public Object
{
public:

	static Viewport* GetMainWindowViewport();

	const Size& GetSize() const;

	void SetBackgroundColor(const Color& color);

	void AddViewportLayer(ViewportLayer* layer) { m_viewportLayerList.Add(layer); }
	void RemoveViewportLayer(ViewportLayer* layer) { m_viewportLayerList.Remove(RefPtr<ViewportLayer>(layer)); }

LN_INTERNAL_ACCESS:	// TODO: Ç¢Ç‹ÇÕÇ∆ÇËÇ†Ç¶Ç∏ì‡ïîópìr
	Viewport();
	virtual ~Viewport();
	void Initialize(GraphicsManager* manager, RenderTarget* renderTarget);
	void Render();

private:
	void TryRemakeLayerTargets();

	GraphicsManager*				m_manager;
	RenderTarget*					m_renderTarget;
	Array<RefPtr<ViewportLayer>>	m_viewportLayerList;
	ColorF							m_backgroundColor;
	RenderTarget*					m_primaryLayerTarget;
	RenderTarget*					m_secondaryLayerTarget;
};

LN_NAMESPACE_END
