
#pragma once
#include "../Platform/EventListener.h"
#include "../Graphics/Viewport.h"
#include "UIElement.h"
#include "UILayoutView.h"

LN_NAMESPACE_BEGIN
namespace detail { class InputManager; }
namespace detail { class SceneRenderer; }
class PlatformWindow;
class SwapChain;
class DrawingContext;
class WorldRenderView;
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
	LN_OBJECT;
public:

	/*  */
	void setAllowDropFiles(bool value);

	PlatformWindow* getPlatformWindow() const;

	DrawingContext* getDrawingContext() const;

	//void setSize(const SizeI& size);

	RenderDiag* GetRenderDiagnostic() const;

LN_CONSTRUCT_ACCESS:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void initialize(PlatformWindow* platformWindow, SwapChain* swapChain, UIContext* context);
	void initialize();
	virtual void dispose() override;
	virtual bool onPlatformEvent(const PlatformEventArgs& e) override;
	virtual void onPresentRenderingContexts();

LN_INTERNAL_ACCESS:
	detail::UIManager* getManager() const { return m_manager; }
	SwapChain* getSwapChain() const;
	void setDelayedRenderingSkip(bool enabled) { m_delayedRenderingSkip = enabled; }

	void renderContents();
	void presentRenderingContexts();

private:
	void initialize_UIRenderer();
	void executeDrawList_UIRenderer();
	bool isStandaloneSwapChain() const { return !m_swapChain.isNull(); }

	detail::UIManager*		m_manager;
	Ref<PlatformWindow>	m_platformWindow;
	Ref<SwapChain>		m_swapChain;

	Ref<DrawingContext>				m_drawingContext;
	Ref<detail::SceneRenderer>	m_internalRenderer;
	Ref<RenderView>	m_drawElementListSet;		// いまは作業用変数を使うためのダミー
	Ref<RenderDiag>	m_renderDiag;
	bool								m_delayedRenderingSkip;

	friend class detail::UIManager;
};


/**
	@brief		メインウィンドウのクラス。
*/
class UIMainWindow
	: public UIFrameWindow
{
	LN_OBJECT;
public:
	UIContext* getMainUIContext() const { return m_mainUIContext; }
	UIViewport* getViewport() const;

protected:
	virtual void onPresentRenderingContexts() override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual bool onPlatformEvent(const PlatformEventArgs& e) override;

LN_INTERNAL_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void initialize(PlatformWindow* platformWindow, World2D* defaultWorld2D, World3D* defaultWorld3D);

	//void injectElapsedTime(float elapsedTime);
	void updateLayout(const Size& viewSize);	// TODO: ゆくゆくは SwapChain や Viewport も UIFrameWindow にもってくる。そのとき、この viewSize はいらなくなる
	void renderUI();

	WorldRenderView* getDefaultCameraViewportLayer2D() const;
	WorldRenderView* getDefaultCameraViewportLayer3D() const;
	UILayoutLayer* getDefaultUILayer() const;
	void setInputManager(detail::InputManager* inputManager) { m_inputManager = inputManager; }

private:
	UIContext*					m_mainUIContext;
	Ref<UIViewport>				m_mainUIViewport;
	Ref<WorldRenderView>	m_cameraViewportLayer3D;
	Ref<WorldRenderView>	m_cameraViewportLayer2D;
	Ref<UILayoutLayer>			m_uiLayer;
	detail::InputManager*		m_inputManager;
};




class UINativeHostWindow;
using UINativeHostWindowPtr = Ref<UINativeHostWindow>;

/**
	@brief		ネイティブウィンドウをホストするための UIFrameWindow クラスです。
*/
class UINativeHostWindow
	: public UIFrameWindow
{
	LN_OBJECT;
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
