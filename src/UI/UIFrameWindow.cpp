
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Viewport.h>
#include <Lumino/Graphics/Rendering.h>
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
	SetZIndex(DefaultZIndex);
}

//------------------------------------------------------------------------------
UIViewportLayer::~UIViewportLayer()
{
}

//------------------------------------------------------------------------------
void UIViewportLayer::Initialize()
{
	auto* manager = m_view->GetOwnerContext()->GetManager();

	// TODO: DrawList は UI Scene 側につくべき
	m_renderingContext = RefPtr<DrawList>::MakeRef();
	m_renderingContext->Initialize(manager->GetGraphicsManager());

	// lighting disabled.
	auto internalRenderer = RefPtr<detail::NonShadingRenderer>::MakeRef();
	internalRenderer->Initialize(manager->GetGraphicsManager());
	m_internalRenderer = internalRenderer;
}

//------------------------------------------------------------------------------
void UIViewportLayer::Render(RenderingContext* context)
{
	m_renderingContext->BeginMakeElements();


	DrawingContext* g = m_view->GetOwnerContext()->GetManager()->GetGraphicsManager()->GetDrawingContext();

	// TODO: ステートリセットもほしいかも？
	g->InheritStatus(context);
	g->SetBlendMode(BlendMode::Normal);
	g->Clear(ClearFlags::Depth, Color::White);	// TODO
	g->Set2DRenderingMode(-1, 1);	// TODO

	// TODO: このへんで、このウィンドウが持っている SwapChain のバックバッファを g にセットする

	m_view->Render(g);
}

//------------------------------------------------------------------------------
void UIViewportLayer::OnEndFrameRender(RenderTarget* renderTarget, DepthBuffer* depthBuffer)
{
	m_renderingContext->EndMakeElements();

	detail::CameraInfo cameraInfo;
	cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(this);
	cameraInfo.viewPixelSize = GetViewportSize();
	cameraInfo.viewMatrix = Matrix::Identity;
	cameraInfo.projMatrix = Matrix::MakePerspective2DLH(cameraInfo.viewPixelSize.width, cameraInfo.viewPixelSize.height, 0, 1);
	cameraInfo.viewProjMatrix = cameraInfo.viewMatrix * cameraInfo.projMatrix;
	cameraInfo.viewFrustum = ViewFrustum(cameraInfo.projMatrix);
	m_internalRenderer->Render(
		m_renderingContext->GetDrawElementList(),
		cameraInfo,
		renderTarget,
		depthBuffer);
	m_renderingContext->EndFrame();
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
	m_uiLayer->Initialize();
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
	m_swapChain->BeginRendering();

	Details::Renderer* renderer = m_manager->GetGraphicsManager()->GetRenderer();
	renderer->Begin();

	m_manager->GetGraphicsManager()->GetRenderingContext()->InitializeFrame(m_swapChain->GetBackBuffer());

	DrawingContext* g = m_manager->GetGraphicsManager()->GetDrawingContext();
	g->InitializeFrame(m_swapChain->GetBackBuffer());
	g->SetRenderTarget(0, m_swapChain->GetBackBuffer());
	g->SetDepthBuffer(m_swapChain->GetBackBufferDepth());	// TODO: InitializeFrame に混ぜたほうがいいかも
	g->Clear(ClearFlags::All, Color::Black);
}

//------------------------------------------------------------------------------
void UIFrameWindow::EndRendering()
{
	m_mainViewport->EndFrameRender();

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
void UIMainWindow::UpdateLayout(const Size& viewSize)
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
