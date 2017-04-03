
#pragma once
#include "../Platform/EventListener.h"
#include "../Graphics/Viewport.h"
#include "UIElement.h"
#include "UILayoutView.h"

LN_NAMESPACE_BEGIN
class PlatformWindow;
class SwapChain;
class DrawingContext;
class CameraViewportLayer;
class World2D;
class World3D;

/**
	@brief		
*/
class UIFrameWindow
	: public UILayoutView
	, public IEventListener
{
	LN_UI_TYPEINFO_DECLARE();
public:

	PlatformWindow* GetPlatformWindow() const { return m_platformWindow; }

	DrawingContext* GetDrawingContext() const;

	void SetSize(const SizeI& size);

protected:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void Initialize(detail::UIManager* manager, PlatformWindow* platformWindow, SwapChain* swapChain, UIContext* context);
	virtual bool OnEvent(const PlatformEventArgs& e) override;
	virtual void OnRenderContents();
	virtual void OnPresentRenderingContexts();

LN_INTERNAL_ACCESS:
	detail::UIManager* GetManager() const { return m_manager; }
	SwapChain* GetSwapChain() const { return m_swapChain; }
	void SetDelayedRenderingSkip(bool enabled) { m_delayedRenderingSkip = enabled; }

	virtual void RenderContents();
	virtual void PresentRenderingContexts();

private:
	void Initialize_UIRenderer();
	void Render_UIRenderer();
	void ExecuteDrawList_UIRenderer();

	detail::UIManager*		m_manager;
	PlatformWindow*			m_platformWindow;
	SwapChain*				m_swapChain;

	RefPtr<DrawingContext>				m_drawingContext;
	RefPtr<detail::InternalRenderer>	m_internalRenderer;
	bool								m_delayedRenderingSkip;
};


/**
	@brief		メインウィンドウのクラス。
*/
class UIMainWindow
	: public UIFrameWindow
{
	LN_UI_TYPEINFO_DECLARE();
public:
	UIContext* GetMainUIContext() const { return m_mainUIContext; }
	Viewport* GetViewport() const { return m_mainViewport; }

protected:
	virtual void OnPresentRenderingContexts() override;

LN_INTERNAL_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void Initialize(detail::UIManager* manager, PlatformWindow* platformWindow, World2D* defaultWorld2D, World3D* defaultWorld3D);

	CameraViewportLayer* GetDefault2DCameraViewportLayer();
	CameraViewportLayer* GetDefault3DCameraViewportLayer();

	void InjectElapsedTime(float elapsedTime);
	void UpdateLayout(const Size& viewSize);	// TODO: ゆくゆくは SwapChain や Viewport も UIFrameWindow にもってくる。そのとき、この viewSize はいらなくなる
	void RenderUI();

	virtual bool OnEvent(const PlatformEventArgs& e) override;
	virtual void OnRenderContents() override;
	virtual void PresentRenderingContexts() override;

private:
	void UpdateViewportTransform();

	UIContext*					m_mainUIContext;
	RefPtr<Viewport>			m_mainViewport;		// TODO: 後で Control 化する。直接描画しない
	RefPtr<CameraViewportLayer>	m_default2DCameraViewportLayer;
	RefPtr<CameraViewportLayer>	m_default3DCameraViewportLayer;
};




class UINativeHostWindow;
using UINativeHostWindowPtr = RefPtr<UINativeHostWindow>;

/**
	@brief		ネイティブウィンドウをホストするための UIFrameWindow クラスです。
*/
class UINativeHostWindow
	: public UIFrameWindow
{
	LN_UI_TYPEINFO_DECLARE();
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
	void Initialize(detail::UIManager* manager, intptr_t windowHandle);

private:
	UIContext*	m_mainUIContext;
};

LN_NAMESPACE_END
