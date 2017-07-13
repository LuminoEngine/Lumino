
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
	LN_OBJECT();
public:

	PlatformWindow* getPlatformWindow() const;

	DrawingContext* getDrawingContext() const;

	//void setSize(const SizeI& size);

	RenderDiag* GetRenderDiagnostic() const;

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void initialize(PlatformWindow* platformWindow, SwapChain* swapChain, UIContext* context);
	void initialize();
	virtual void Dispose() override;
	virtual bool onEvent(const PlatformEventArgs& e) override;
	virtual void onPresentRenderingContexts();

LN_INTERNAL_ACCESS:
	detail::UIManager* getManager() const { return m_manager; }
	SwapChain* getSwapChain() const;
	void setDelayedRenderingSkip(bool enabled) { m_delayedRenderingSkip = enabled; }

	void renderContents();
	virtual void presentRenderingContexts();

private:
	void initialize_UIRenderer();
	void executeDrawList_UIRenderer();
	bool isStandaloneSwapChain() const { return !m_swapChain.isNull(); }

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
	LN_OBJECT();
public:
	UIContext* getMainUIContext() const { return m_mainUIContext; }
	UIViewport* getViewport() const;

protected:
	virtual void onPresentRenderingContexts() override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_INTERNAL_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void initialize(PlatformWindow* platformWindow, World2D* defaultWorld2D, World3D* defaultWorld3D);

	//void injectElapsedTime(float elapsedTime);
	void updateLayout(const Size& viewSize);	// TODO: ゆくゆくは SwapChain や Viewport も UIFrameWindow にもってくる。そのとき、この viewSize はいらなくなる
	void renderUI();
	virtual void presentRenderingContexts() override;

	CameraViewportLayer2* getDefaultCameraViewportLayer2D() const;
	CameraViewportLayer2* getDefaultCameraViewportLayer3D() const;
	UILayoutLayer* getDefaultUILayer() const;

private:
	void updateViewportTransform();

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
	LN_OBJECT();
public:

	/**
		@brief		UIFrameWindow を作成します。
	*/
	static UINativeHostWindowPtr create(intptr_t windowHandle);


public:

	/** ホストされたネイティブウィンドウへ描画を行います。*/
	//void render();

LN_INTERNAL_ACCESS:
	UINativeHostWindow();
	virtual ~UINativeHostWindow();
	void initialize(intptr_t windowHandle);

private:
	UIContext*	m_mainUIContext;
};

LN_NAMESPACE_END
