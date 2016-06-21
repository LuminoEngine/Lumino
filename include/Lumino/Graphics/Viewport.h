
#pragma once
#include "../BindingSupport.h"
#include "Texture.h"
#include "Color.h"
#include "ImageEffect/ImageEffect.h"

LN_NAMESPACE_BEGIN

class RenderingCompositionContext
{
public:
	RenderTarget* RequestRenderTarget(int width, int height);
};

//// Camera とかが継承する
//class IRenderingCompositionItem
//{
//public:
//	//void Render(RenderingCompositionContext* context);
//};

class ViewportLayer
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	const Size& GetViewportSize() const;
	ImageEffectList* GetImageEffects() const { return m_imageEffects; }

protected:
	ViewportLayer();
	virtual ~ViewportLayer();


	///// 前描画
	//virtual void PreRender(const SizeF& viewSize) {}

	/// 本描画
	virtual void Render(RenderTarget* renderTarget) = 0;

	/// 後描画
	void PostRender(RenderingContext* renderingContext, RenderTarget** primaryLayerTarget, RenderTarget** secondaryLayerTarget);

private:
	Viewport*				m_owner;
	RefPtr<ImageEffectList>	m_imageEffects;

	friend class Viewport;
};

class ViewportLayerList
	: public ObjectList<ViewportLayer*>
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
LN_INTERNAL_ACCESS:
	ViewportLayerList();
	virtual ~ViewportLayerList();
};

/**
	@brief		
*/
class Viewport
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	static Viewport* GetMainWindowViewport();

	const Size& GetSize() const;

	void SetBackgroundColor(const Color& color);

	ObjectList<ViewportLayer*>* GetLayers() const { return m_viewportLayerList; }

	void AddViewportLayer(ViewportLayer* layer) { m_viewportLayerList->Add(layer); layer->m_owner = this; }
	void RemoveViewportLayer(ViewportLayer* layer)
	{
		if (m_viewportLayerList->Remove(RefPtr<ViewportLayer>(layer)))
		{
			layer->m_owner = nullptr;
		}
	}

LN_INTERNAL_ACCESS:	// TODO: いまはとりあえず内部用途
	Viewport();
	virtual ~Viewport();
	void Initialize(GraphicsManager* manager, RenderTarget* renderTarget);
	void Render();

private:
	void TryRemakeLayerTargets();
	void MakeViewBoxTransform(const Size& dstSize, const Size& srcSize, Matrix* mat);

	GraphicsManager*			m_manager;
	RenderTarget*				m_renderTarget;

	RefPtr<ViewportLayerList>	m_viewportLayerList;
	ColorF						m_backgroundColor;
	RenderTarget*				m_primaryLayerTarget;
	RenderTarget*				m_secondaryLayerTarget;
};

LN_NAMESPACE_END
