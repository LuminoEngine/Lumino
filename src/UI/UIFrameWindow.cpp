
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIFrameWindow.h>
#include "UIManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RendererImpl.h"
#include "../Platform/PlatformManager.h"
#include "../Platform/PlatformWindowManagerBase.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIFrameWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIFrameWindow, Object)

//------------------------------------------------------------------------------
UIFrameWindow::UIFrameWindow()
	: m_manager(nullptr)
	, m_platformWindow(nullptr)
	, m_swapChain(nullptr)
{
}

//------------------------------------------------------------------------------
UIFrameWindow::~UIFrameWindow()
{
}

//------------------------------------------------------------------------------
void UIFrameWindow::Initialize(detail::UIManager* manager, PlatformWindow* platformWindow, SwapChain* swapChain)
{
	LN_CHECK_ARG(manager != nullptr);
	LN_CHECK_ARG(platformWindow != nullptr);
	LN_CHECK_ARG(swapChain != nullptr);
	m_manager = manager;
	m_platformWindow = platformWindow;
	m_swapChain = swapChain;
}

//------------------------------------------------------------------------------
void UIFrameWindow::BeginRendering()
{
	Details::Renderer* renderer = m_manager->GetGraphicsManager()->GetRenderer();
	renderer->Begin();
	renderer->SetRenderTarget(0, m_swapChain->GetBackBuffer());
	renderer->SetDepthBuffer(m_swapChain->GetBackBufferDepth());
	renderer->Clear(ClearFlags::All, ColorF::Black);
}

//------------------------------------------------------------------------------
void UIFrameWindow::EndRendering()
{
	Details::Renderer* renderer = m_manager->GetGraphicsManager()->GetRenderer();
	m_manager->GetGraphicsManager()->SwitchActiveContext(nullptr);
	renderer->End();
	m_swapChain->Present();
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
	LN_CHECK_ARG(manager != nullptr);
	UIFrameWindow::Initialize(manager, platformWindow, manager->GetGraphicsManager()->GetMainSwapChain());

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
void UIMainWindow::RenderUI()
{
	GraphicsContext* g = GetManager()->GetGraphicsManager()->GetGraphicsContext();


	g->Clear(ClearFlags::Depth, ColorF::White);	// TODO
	g->Set2DRenderingMode(-1, 1);	// TODO

	// TODO: このへんで、このウィンドウが持っている SwapChain のバックバッファを g にセットする

	m_mainUIContext->GetMainWindowView()->Render(g);
}


//==============================================================================
// UINativeHostWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UINativeHostWindow, UIFrameWindow)

//------------------------------------------------------------------------------
UINativeHostWindowPtr UINativeHostWindow::Create(void* windowHandle)
{
	auto ptr = UINativeHostWindowPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance(), windowHandle);
	return ptr;
}

//------------------------------------------------------------------------------
UINativeHostWindow::UINativeHostWindow()
	: m_platformWindow(nullptr)
{
}

//------------------------------------------------------------------------------
UINativeHostWindow::~UINativeHostWindow()
{
	LN_SAFE_RELEASE(m_platformWindow);
}

//------------------------------------------------------------------------------
void UINativeHostWindow::Initialize(detail::UIManager* manager, void* windowHandle)
{
	LN_CHECK_ARG(manager != nullptr);
	LN_CHECK_ARG(windowHandle != nullptr);

	WindowCreationSettings ws;
	//ws.title;		// TODO
	//Size	clientSize = Size(640, 480);	// クライアント領域のピクセルサイズ
	//bool	fullscreen = false;				// フルスクリーンモードで作成するかどうか
	//bool	resizable = true;				// 可変ウィンドウとして作成するかどうか
	ws.userWindow = windowHandle;
	m_platformWindow = manager->GetPlatformManager()->GetWindowManager()->CreateSubWindow(ws);
}

//------------------------------------------------------------------------------
void UINativeHostWindow::Render()
{
	BeginRendering();
	EndRendering();
}

LN_NAMESPACE_END
