
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN
class PlatformWindow;

/**
	@brief		
*/
class UIFrameWindow
	: public Object	// TODO: UILayoutRoot のサブクラスのほうが一般的なUIフレームワーク？
{
	LN_UI_TYPEINFO_DECLARE();
public:

	PlatformWindow* GetPlatformWindow() const { return m_platformWindow; }

	Viewport* GetViewport() const { return m_mainViewport; }

protected:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void Initialize(detail::UIManager* manager, PlatformWindow* platformWindow, SwapChain* swapChain);

LN_INTERNAL_ACCESS:
	detail::UIManager* GetManager() const { return m_manager; }
	void Render();
	void BeginRendering();
	void EndRendering();
	void RenderContents();

private:
	detail::UIManager*	m_manager;
	PlatformWindow*		m_platformWindow;
	SwapChain*			m_swapChain;
	Viewport*			m_mainViewport;
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

LN_INTERNAL_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void Initialize(detail::UIManager* manager, PlatformWindow* platformWindow);

	void InjectElapsedTime(float elapsedTime);
	void UpdateLayout(const SizeF& viewSize);	// TODO: ゆくゆくは SwapChain や Viewport も UIFrameWindow にもってくる。そのとき、この viewSize はいらなくなる
	void RenderUI();

private:
	UIContext*	m_mainUIContext;
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
	static UINativeHostWindowPtr Create(void* windowHandle);


LN_INTERNAL_ACCESS:
	UINativeHostWindow();
	virtual ~UINativeHostWindow();
	void Initialize(detail::UIManager* manager, void* windowHandle);

private:
};

LN_NAMESPACE_END
