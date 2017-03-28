
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Viewport.h>
#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/World.h>
#include <Lumino/Scene/SceneGraph.h>
#include "UIManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RendererImpl.h"
#include "../Platform/PlatformManager.h"
#include "../Platform/PlatformWindowManagerBase.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIFrameWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIFrameWindow, UILayoutView)

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
	m_platformWindow->DetachEventListener(this);
	LN_SAFE_RELEASE(m_swapChain);
	LN_SAFE_RELEASE(m_platformWindow);
}

//------------------------------------------------------------------------------
void UIFrameWindow::Initialize(detail::UIManager* manager, PlatformWindow* platformWindow, SwapChain* swapChain, UIContext* context)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;
	if (LN_CHECK_ARG(platformWindow != nullptr)) return;
	if (LN_CHECK_ARG(swapChain != nullptr)) return;
	m_manager = manager;
	LN_REFOBJ_SET(m_platformWindow, platformWindow);
	LN_REFOBJ_SET(m_swapChain, swapChain);

	UILayoutView::Initialize(context, platformWindow);

	m_platformWindow->AttachEventListener(this, 0);

	Initialize_UIRenderer();
}

//------------------------------------------------------------------------------
bool UIFrameWindow::OnEvent(const PlatformEventArgs& e)
{
	return false;
}

//------------------------------------------------------------------------------
void UIFrameWindow::OnPresentRenderingContexts()
{
}

//------------------------------------------------------------------------------
void UIFrameWindow::SetSize(const SizeI& size)
{
	if (LN_CHECK_STATE(m_platformWindow != nullptr)) return;
	m_platformWindow->SetSize(size);
}

//------------------------------------------------------------------------------
void UIFrameWindow::BeginRendering()
{

}

//------------------------------------------------------------------------------
void UIFrameWindow::RenderContents()
{
	Render_UIRenderer();
}

//------------------------------------------------------------------------------
void UIFrameWindow::PresentRenderingContexts()
{
	Details::Renderer* renderer = m_manager->GetGraphicsManager()->GetRenderer();
	renderer->Begin();

	ExecuteDrawList_UIRenderer();

	OnPresentRenderingContexts();

	//Details::Renderer* renderer = m_manager->GetGraphicsManager()->GetRenderer();

	m_manager->GetGraphicsManager()->SwitchActiveContext(nullptr);
	renderer->End();
	m_swapChain->Present();

	// ウィンドウサイズとバックバッファサイズを合わせる
	m_swapChain->MightResizeAndDeviceReset(m_platformWindow->GetSize());

}

//------------------------------------------------------------------------------
void UIFrameWindow::Initialize_UIRenderer()
{
	auto* manager = GetOwnerContext()->GetManager();

	m_drawingContext = NewObject<DrawingContext>();

	// lighting disabled.
	auto internalRenderer = RefPtr<detail::NonShadingRenderer>::MakeRef();
	internalRenderer->Initialize(manager->GetGraphicsManager());
	m_internalRenderer = internalRenderer;
}

//------------------------------------------------------------------------------
void UIFrameWindow::Render_UIRenderer()
{
	m_drawingContext->BeginMakeElements();

	UILayoutView::Render(m_drawingContext);
}

//------------------------------------------------------------------------------
void UIFrameWindow::ExecuteDrawList_UIRenderer()
{
	RenderTargetTexture* renderTarget = m_swapChain->GetBackBuffer();
	DepthBuffer* depthBuffer = nullptr;
	
	detail::CameraInfo cameraInfo;
	cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(this);
	cameraInfo.viewPixelSize = renderTarget->GetSize().ToFloatSize();
	cameraInfo.viewPosition = Vector3::Zero;
	cameraInfo.viewMatrix = Matrix::Identity;
	cameraInfo.projMatrix = Matrix::MakePerspective2DLH(cameraInfo.viewPixelSize.width, cameraInfo.viewPixelSize.height, 0, 1);
	cameraInfo.viewProjMatrix = cameraInfo.viewMatrix * cameraInfo.projMatrix;
	cameraInfo.viewFrustum = ViewFrustum(cameraInfo.projMatrix);
	cameraInfo.zSortDistanceBase = ZSortDistanceBase::NodeZ;
	m_internalRenderer->Render(
		m_drawingContext->GetDrawElementList(),
		cameraInfo,
		renderTarget,
		depthBuffer);
	m_drawingContext->EndFrame();
}



//==============================================================================
// UIMainWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIMainWindow, UIFrameWindow)

//------------------------------------------------------------------------------
UIMainWindow::UIMainWindow()
	: m_mainUIContext(nullptr)
	, m_mainViewport(nullptr)
	//, m_uiLayer(nullptr)
{
}

//------------------------------------------------------------------------------
UIMainWindow::~UIMainWindow()
{
	m_mainViewport->RemoveViewportLayer(m_default2DCameraViewportLayer);
	m_mainViewport->RemoveViewportLayer(m_default3DCameraViewportLayer);
	LN_SAFE_RELEASE(m_mainUIContext);
}

//------------------------------------------------------------------------------
void UIMainWindow::Initialize(detail::UIManager* manager, PlatformWindow* platformWindow, World2D* defaultWorld2D, World3D* defaultWorld3D)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;

	m_mainUIContext = LN_NEW UIContext();
	m_mainUIContext->Initialize(manager);

	UIFrameWindow::Initialize(manager, platformWindow, manager->GetGraphicsManager()->GetMainSwapChain(), m_mainUIContext);

	// MainViewport
	m_mainViewport = RefPtr<Viewport>::MakeRef();
	m_mainViewport->Initialize(GetManager()->GetGraphicsManager(), platformWindow->GetSize());

	m_default3DCameraViewportLayer = RefPtr<CameraViewportLayer>::MakeRef();
	m_default3DCameraViewportLayer->Initialize(detail::EngineDomain::GetSceneGraphManager(), defaultWorld3D, defaultWorld3D->GetSceneGraph3D()->GetMainCamera());
	m_mainViewport->AddViewportLayer(m_default3DCameraViewportLayer);

	m_default2DCameraViewportLayer = RefPtr<CameraViewportLayer>::MakeRef();
	m_default2DCameraViewportLayer->Initialize(detail::EngineDomain::GetSceneGraphManager(), defaultWorld2D, defaultWorld2D->GetSceneGraph2D()->GetMainCamera());
	m_mainViewport->AddViewportLayer(m_default2DCameraViewportLayer);

	// UI Layer
	//m_uiLayer.Attach(LN_NEW UIViewportLayer(m_mainUIContext->GetMainWindowView()), false);
	//m_uiLayer->Initialize();
	//m_mainViewport->AddViewportLayer(m_uiLayer);

	// SwapChain のサイズを Viewport へ通知
	UpdateViewportTransform();
}

CameraViewportLayer* UIMainWindow::GetDefault2DCameraViewportLayer()
{
	return m_default2DCameraViewportLayer;
}
CameraViewportLayer* UIMainWindow::GetDefault3DCameraViewportLayer()
{
	return m_default3DCameraViewportLayer;
}

//------------------------------------------------------------------------------
void UIMainWindow::InjectElapsedTime(float elapsedTime)
{
	m_mainUIContext->InjectElapsedTime(elapsedTime);
}

//------------------------------------------------------------------------------
void UIMainWindow::UpdateLayout(const Size& viewSize)
{
	UIFrameWindow::UpdateLayout(viewSize);
}

//------------------------------------------------------------------------------
void UIMainWindow::RenderUI()
{
}

//------------------------------------------------------------------------------
bool UIMainWindow::OnEvent(const PlatformEventArgs& e)
{
	return m_mainViewport->DoPlatformEvent(e);
}
void UIMainWindow::BeginRendering()
{
	UIFrameWindow::BeginRendering();
}
void UIMainWindow::RenderContents()
{
	Details::Renderer* renderer = GetManager()->GetGraphicsManager()->GetRenderer();
	m_mainViewport->Render(renderer);

	UIFrameWindow::RenderContents();
}

void UIMainWindow::OnPresentRenderingContexts()
{
	UIFrameWindow::OnPresentRenderingContexts();

	Details::Renderer* renderer = GetManager()->GetGraphicsManager()->GetRenderer();
	m_mainViewport->PresentRenderingContexts(renderer, GetSwapChain()->GetBackBuffer());
}

void UIMainWindow::PresentRenderingContexts()
{
	UIFrameWindow::PresentRenderingContexts();

	// SwapChain のサイズを Viewport へ通知
	// ※ SwapChain のサイズが「本当に」変わるタイミングは、描画コマンドが確定する Present の後。
	//    フレーム更新の最初で行ってもよいが、この時点で行ってもよい。
	UpdateViewportTransform();
}

void UIMainWindow::UpdateViewportTransform()
{
	const SizeI& bbSize = GetSwapChain()->GetBackBuffer()->GetSize();
	Size viewSize((float)bbSize.width, (float)bbSize.height);
	m_mainViewport->UpdateLayersTransform(viewSize);
}

//==============================================================================
// UINativeHostWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UINativeHostWindow, UIFrameWindow)

//------------------------------------------------------------------------------
UINativeHostWindowPtr UINativeHostWindow::Create(intptr_t windowHandle)
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
void UINativeHostWindow::Initialize(detail::UIManager* manager, intptr_t windowHandle)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;
	if (LN_CHECK_ARG(windowHandle != 0)) return;

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

	UIFrameWindow::Initialize(manager, window, swap, m_mainUIContext);
}

//------------------------------------------------------------------------------
//void UINativeHostWindow::Render()
//{
//	UIFrameWindow::BeginRendering();
//	UIFrameWindow::RenderContents();
//	UIFrameWindow::PresentRenderingContexts();
//}

LN_NAMESPACE_END
