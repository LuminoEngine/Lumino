
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIFrameWindow.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIFrameWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIFrameWindow, Object)

//------------------------------------------------------------------------------
UIFrameWindow::UIFrameWindow()
	: m_manager(nullptr)
	, m_platformWindow(nullptr)
	//, m_backBufferViewport(nullptr)
{
}

//------------------------------------------------------------------------------
UIFrameWindow::~UIFrameWindow()
{
}

//------------------------------------------------------------------------------
void UIFrameWindow::Initialize(detail::UIManager* manager, PlatformWindow* platformWindow)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_platformWindow = platformWindow;
}


//==============================================================================
// UIMainWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIMainWindow, UIFrameWindow)

//------------------------------------------------------------------------------
UIMainWindow::UIMainWindow()
	: m_mainUIContext(nullptr)
{
}

//------------------------------------------------------------------------------
UIMainWindow::~UIMainWindow()
{
	LN_SAFE_RELEASE(m_mainUIContext);
}

//------------------------------------------------------------------------------
void UIMainWindow::Initialize(detail::UIManager* manager, PlatformWindow* platformWindow)
{
	UIFrameWindow::Initialize(manager, platformWindow);

	m_mainUIContext = LN_NEW UIContext();
	m_mainUIContext->Initialize(manager);

	//m_mainUIContext->GetMainWindowView()->InjectViewportSizeChanged(
	//	platformWindow->GetSize().width, platformWindow->GetSize().height);
}

//------------------------------------------------------------------------------
void UIMainWindow::InjectElapsedTime(float elapsedTime)
{
	m_mainUIContext->InjectElapsedTime(elapsedTime);
}

//------------------------------------------------------------------------------
void UIMainWindow::UpdateLayout(const SizeF& viewSize)
{
	m_mainUIContext->GetMainWindowView()->UpdateLayout(viewSize);
}

//------------------------------------------------------------------------------
void UIMainWindow::Render(GraphicsContext* g)
{
	// TODO: このへんで、このウィンドウが持っている SwapChain のバックバッファを g にセットする

	m_mainUIContext->GetMainWindowView()->Render(g);
}


LN_NAMESPACE_END
