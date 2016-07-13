
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Viewport.h>
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
// UIViewportLayer
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIViewportLayer, ViewportLayer);

//------------------------------------------------------------------------------
UIViewportLayer::UIViewportLayer(UILayoutView* view)
	: m_view(view)
{
	assert(view != nullptr);
}

//------------------------------------------------------------------------------
UIViewportLayer::~UIViewportLayer()
{
}

//------------------------------------------------------------------------------
void UIViewportLayer::Render(RenderTarget* renderTarget)
{
	GraphicsContext* g = m_view->GetOwnerContext()->GetManager()->GetGraphicsManager()->GetGraphicsContext();


	g->Clear(ClearFlags::Depth, ColorF::White);	// TODO
	g->Set2DRenderingMode(-1, 1);	// TODO

	// TODO: このへんで、このウィンドウが持っている SwapChain のバックバッファを g にセットする

	m_view->Render(g);
}


//==============================================================================
// UIFrameWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIFrameWindow, Object)

//------------------------------------------------------------------------------
UIFrameWindow::UIFrameWindow()
	: m_manager(nullptr)
	, m_platformWindow(nullptr)
	, m_swapChain(nullptr)
	, m_mainViewport(nullptr)
{
}

//------------------------------------------------------------------------------
UIFrameWindow::~UIFrameWindow()
{
	LN_SAFE_RELEASE(m_mainViewport);
	LN_SAFE_RELEASE(m_swapChain);
	LN_SAFE_RELEASE(m_platformWindow);
}

//------------------------------------------------------------------------------
void UIFrameWindow::Initialize(detail::UIManager* manager, PlatformWindow* platformWindow, SwapChain* swapChain, UILayoutView* view)
{
	LN_CHECK_ARG(manager != nullptr);
	LN_CHECK_ARG(platformWindow != nullptr);
	LN_CHECK_ARG(swapChain != nullptr);
	m_manager = manager;
	LN_REFOBJ_SET(m_platformWindow, platformWindow);
	LN_REFOBJ_SET(m_swapChain, swapChain);

	// MainViewport
	m_mainViewport = LN_NEW Viewport();
	m_mainViewport->Initialize(m_manager->GetGraphicsManager(), m_swapChain->GetBackBuffer());

	// UI Layer
	m_uiLayer.Attach(LN_NEW UIViewportLayer(view), false);
	m_mainViewport->AddViewportLayer(m_uiLayer);
}

//------------------------------------------------------------------------------
void UIFrameWindow::Render()
{
	BeginRendering();
	RenderContents();
	EndRendering();
}

//------------------------------------------------------------------------------
void UIFrameWindow::BeginRendering()
{
	Details::Renderer* renderer = m_manager->GetGraphicsManager()->GetRenderer();
	renderer->Begin();

	GraphicsContext* g = m_manager->GetGraphicsManager()->GetGraphicsContext();
	g->SetRenderTarget(0, m_swapChain->GetBackBuffer());
	g->SetDepthBuffer(m_swapChain->GetBackBufferDepth());
	g->Clear(ClearFlags::All, ColorF::Black);
}

//------------------------------------------------------------------------------
void UIFrameWindow::EndRendering()
{
	Details::Renderer* renderer = m_manager->GetGraphicsManager()->GetRenderer();
	m_manager->GetGraphicsManager()->SwitchActiveContext(nullptr);
	renderer->End();
	m_swapChain->Present();
}

//------------------------------------------------------------------------------
void UIFrameWindow::RenderContents()
{
	m_mainViewport->Render();
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

	m_mainUIContext = LN_NEW UIContext();
	m_mainUIContext->Initialize(manager);

	UIFrameWindow::Initialize(manager, platformWindow, manager->GetGraphicsManager()->GetMainSwapChain(), m_mainUIContext->GetMainWindowView());


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
{
}

//------------------------------------------------------------------------------
UINativeHostWindow::~UINativeHostWindow()
{
	LN_SAFE_RELEASE(m_mainUIContext);
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
	RefPtr<PlatformWindow> window(manager->GetPlatformManager()->GetWindowManager()->CreateSubWindow(ws), false);

	auto swap = RefPtr<SwapChain>::MakeRef();
	swap->InitializeSub(manager->GetGraphicsManager(), window);


	m_mainUIContext = LN_NEW UIContext();
	m_mainUIContext->Initialize(manager);

	UIFrameWindow::Initialize(manager, window, swap, m_mainUIContext->GetMainWindowView());
}

LN_NAMESPACE_END
