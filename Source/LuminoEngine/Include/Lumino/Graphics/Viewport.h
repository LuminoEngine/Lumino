
#pragma once
#include "../BindingSupport.h"
#include "Texture.h"
#include "Color.h"
#include "ImageEffect/ImageEffect.h"

LN_NAMESPACE_BEGIN
namespace detail { class InternalRenderer; }
namespace detail { class RenderingPass2; }
struct PlatformEventArgs;
class DrawList;

class RenderingCompositionContext
{
public:
	RenderTargetTexture* RequestRenderTarget(int width, int height);
};

/** Viewport の配置方法 */
enum class ViewportPlacement
{
	Stretch,		/**< 転送先領域全体に拡大または縮小する */
	AutoResize,		/**< 転送先領域と同じピクセルサイズになるよう自動的にリサイズする */
	ViewBox,		/**< レイヤーのサイズが維持されるように余白を挿入する */
};



class ViewportLayer
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/** このレイヤーのピクセルサイズを取得します。*/
	const Size& GetSize() const;

	ImageEffectList* GetImageEffects() const { return m_imageEffects; }

	void SetZIndex(int zIndex) { m_zIndex = zIndex; }

	int GetZIndex() const { return m_zIndex; }


	//virtual DrawList* GetRenderer() = 0;

protected:
	virtual bool OnPlatformEvent(const PlatformEventArgs& e);

LN_PROTECTED_INTERNAL_ACCESS:
	ViewportLayer();
	virtual ~ViewportLayer();
	void SetOwner(Viewport* owner) { m_owner = owner; }
	virtual void UpdateTransform(const Size& viewSize);
	void PreRender(const SizeI& ownerViewPixelSize);
	
//protected:


	/// 本描画
	virtual void Render() = 0;

	/// 後描画
	void PostRender(DrawList* context, RefPtr<RenderTargetTexture>* primaryLayerTarget, RefPtr<RenderTargetTexture>* secondaryLayerTarget);

	virtual void ExecuteDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);

private:
	Viewport*					m_owner;
	Size						m_size;
	RefPtr<ImageEffectList>		m_imageEffects;
	int							m_zIndex;

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
	@brief		ビューポートのクラス
*/
class Viewport
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	const Size& GetSize() const;

	void SetBackgroundColor(const Color& color);

	/** ビューポートの配置方法を設定します。*/
	void SetPlacement(ViewportPlacement placement);


	ObjectList<ViewportLayer*>* GetLayers() const { return m_viewportLayerList; }

	void AddViewportLayer(ViewportLayer* layer);
	void RemoveViewportLayer(ViewportLayer* layer);

LN_INTERNAL_ACCESS:	// TODO: いまはとりあえず内部用途
	Viewport();
	virtual ~Viewport();
	void Initialize(detail::GraphicsManager* manager);
	detail::GraphicsManager* GetManager() const { return m_manager; }

	// call from UIFrameWindow
	void UpdateLayersTransform(const Size& viewSize);
	bool DoPlatformEvent(const PlatformEventArgs& e);
	void BeginRender(Details::Renderer* renderer, const SizeI& viewSize);
	void Render(Details::Renderer* renderer);
	void EndRender(Details::Renderer* renderer, RenderTargetTexture* renderTarget);

private:
	void TryRemakeLayerTargets(const SizeI& ownerViewPixelSize);
	void MakeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat);
	void BeginBlitRenderer();
	void FlushBlitRenderer(RenderTargetTexture* renderTarget);

	detail::GraphicsManager*	m_manager;
	Size						m_size;
	ViewportPlacement			m_placement;
	//RenderTarget*				m_renderTarget;

	RefPtr<ViewportLayerList>	m_viewportLayerList;
	Color						m_backgroundColor;
	RefPtr<RenderTargetTexture>	m_primaryLayerTarget;
	RefPtr<RenderTargetTexture>	m_secondaryLayerTarget;
	//RenderTarget*				m_primaryLayerTargetOrg;
	RefPtr<DepthBuffer>			m_depthBuffer;

	RefPtr<DrawList>					m_renderer;			// for PostProcess and Blit to backbuffer.
	RefPtr<detail::InternalRenderer>	m_internalRenderer;	// for PostProcess and Blit to backbuffer.
	//RefPtr<detail::RenderingPass2>		m_pass;				// for PostProcess and Blit to backbuffer.
};

///**
//	@brief		デフォルトで作成される、メインのビューポートのクラス
//*/
//class MainViewport
//	: public Viewport
//{
//	LN_TR_REFLECTION_TYPEINFO_DECLARE();
//public:
//	ViewportLayer* GetDefault2DLayer();
//
//	ViewportLayer* GetDefault3DLayer();
//
//LN_INTERNAL_ACCESS:
//	MainViewport();
//	virtual ~MainViewport();
//	void Initialize(detail::GraphicsManager* manager, RenderTarget* renderTarget);
//};

LN_NAMESPACE_END
