
#pragma once
#include "../Platform/EventListener.h"
#include "../Graphics/Viewport.h"
#include "UIElement.h"
#include "UILayoutView.h"

LN_NAMESPACE_BEGIN
namespace detail { class SceneRenderer; }
class PlatformWindow;
class SwapChain;
class DrawingContext;
class CameraViewportLayer2;
class World2D;
class World3D;
class UIViewport;
class UILayoutLayer;
class RenderView;
class RenderDiag;

/**
	@brief		
*/
class UIFrameWindow
	: public UILayoutView
	, public IEventListener
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	PlatformWindow* GetPlatformWindow() const;

	DrawingContext* GetDrawingContext() const;

	//void SetSize(const SizeI& size);

	RenderDiag* GetRenderDiagnostic() const;

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void Initialize(PlatformWindow* platformWindow, SwapChain* swapChain, UIContext* context);
	void Initialize();
	virtual void Dispose();
	virtual bool OnEvent(const PlatformEventArgs& e) override;
	virtual void OnRenderContents();
	virtual void OnPresentRenderingContexts();

LN_INTERNAL_ACCESS:
	detail::UIManager* GetManager() const { return m_manager; }
	SwapChain* GetSwapChain() const;
	void SetDelayedRenderingSkip(bool enabled) { m_delayedRenderingSkip = enabled; }

	void RenderContents();
	virtual void PresentRenderingContexts();

private:
	void Initialize_UIRenderer();
	void Render_UIRenderer();
	void ExecuteDrawList_UIRenderer();
	bool IsStandaloneSwapChain() const { return !m_swapChain.IsNull(); }

	detail::UIManager*		m_manager;
	RefPtr<PlatformWindow>	m_platformWindow;
	RefPtr<SwapChain>		m_swapChain;

	RefPtr<DrawingContext>				m_drawingContext;
	RefPtr<detail::SceneRenderer>	m_internalRenderer;
	RefPtr<RenderView>	m_drawElementListSet;		// いまは作業用変数を使うためのダミー
	RefPtr<RenderDiag>	m_renderDiag;
	bool								m_delayedRenderingSkip;

	friend class detail::UIManager;
};


/**
	@brief		メインウィンドウのクラス。
*/
class UIMainWindow
	: public UIFrameWindow
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	UIContext* GetMainUIContext() const { return m_mainUIContext; }
	UIViewport* GetViewport() const;

protected:
	virtual void OnPresentRenderingContexts() override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

LN_INTERNAL_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void Initialize(PlatformWindow* platformWindow, World2D* defaultWorld2D, World3D* defaultWorld3D);

	//void InjectElapsedTime(float elapsedTime);
	void UpdateLayout(const Size& viewSize);	// TODO: ゆくゆくは SwapChain や Viewport も UIFrameWindow にもってくる。そのとき、この viewSize はいらなくなる
	void RenderUI();

	//virtual bool OnEvent(const PlatformEventArgs& e) override;
	virtual void OnRenderContents() override;
	virtual void PresentRenderingContexts() override;

	CameraViewportLayer2* GetDefaultCameraViewportLayer2D() const;
	CameraViewportLayer2* GetDefaultCameraViewportLayer3D() const;
	UILayoutLayer* GetDefaultUILayer() const;

private:
	void UpdateViewportTransform();

	UIContext*						m_mainUIContext;
	RefPtr<UIViewport>				m_mainUIViewport;
	RefPtr<CameraViewportLayer2>	m_cameraViewportLayer3D;
	RefPtr<CameraViewportLayer2>	m_cameraViewportLayer2D;
	RefPtr<UILayoutLayer>			m_uiLayer;
};




class UINativeHostWindow;
using UINativeHostWindowPtr = RefPtr<UINativeHostWindow>;

/**
	@brief		ネイティブウィンドウをホストするための UIFrameWindow クラスです。
*/
class UINativeHostWindow
	: public UIFrameWindow
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		UIFrameWindow を作成します。
	*/
	static UINativeHostWindowPtr Create(intptr_t windowHandle);


public:

	/** ホストされたネイティブウィンドウへ描画を行います。*/
	//void Render();

LN_INTERNAL_ACCESS:
	UINativeHostWindow();
	virtual ~UINativeHostWindow();
	void Initialize(intptr_t windowHandle);

private:
	UIContext*	m_mainUIContext;
};

LN_NAMESPACE_END
