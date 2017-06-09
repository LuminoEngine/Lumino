
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Viewport.h>
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/UI/UIViewport.h>
#include <Lumino/UI/UILayoutPanel.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/World.h>
#include <Lumino/Scene/SceneGraph.h>
#include "UIManager.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RendererImpl.h"
#include "../Graphics/RenderingThread.h"
#include "../Platform/PlatformManager.h"
#include "../Platform/PlatformWindowManagerBase.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIFrameWindow
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIFrameWindow, UILayoutView)

//------------------------------------------------------------------------------
UIFrameWindow::UIFrameWindow()
	: m_manager(nullptr)
	, m_platformWindow(nullptr)
	, m_swapChain(nullptr)
	, m_delayedRenderingSkip(false)
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
void UIFrameWindow::Initialize(PlatformWindow* platformWindow, SwapChain* swapChain, UIContext* context)
{
	if (LN_CHECK_ARG(platformWindow != nullptr)) return;
	m_manager = detail::EngineDomain::GetUIManager();
	LN_REFOBJ_SET(m_platformWindow, platformWindow);
	LN_REFOBJ_SET(m_swapChain, swapChain);

	UILayoutView::Initialize(context, platformWindow);
	m_platformWindow->AttachEventListener(this, 0);
	Initialize_UIRenderer();
}

//------------------------------------------------------------------------------
//void UIFrameWindow::Initialize()
//{
//	m_manager = detail::EngineDomain::GetUIManager();
//
//
//
//	WindowCreationSettings ws;
//	RefPtr<PlatformWindow> window(m_manager->GetPlatformManager()->GetWindowManager()->CreateSubWindow(ws), false);
//	auto swap = RefPtr<SwapChain>::MakeRef();
//	swap->InitializeSub(m_manager->GetGraphicsManager(), window);
//
//	UILayoutView::Initialize(UIContext::GetMainContext(), window);
//	m_platformWindow->AttachEventListener(this, 0);
//	Initialize_UIRenderer();
//}

//------------------------------------------------------------------------------
DrawingContext* UIFrameWindow::GetDrawingContext() const
{
	return m_drawingContext;
}

//------------------------------------------------------------------------------
RenderDiag* UIFrameWindow::GetRenderDiagnostic() const
{
	return m_renderDiag;
}

//------------------------------------------------------------------------------
bool UIFrameWindow::OnEvent(const PlatformEventArgs& e)
{
	return false;
}

//------------------------------------------------------------------------------
void UIFrameWindow::OnRenderContents()
{
}

//------------------------------------------------------------------------------
void UIFrameWindow::OnPresentRenderingContexts()
{
	ExecuteDrawList_UIRenderer();
}

//------------------------------------------------------------------------------
//void UIFrameWindow::SetSize(const SizeI& size)
//{
//	if (LN_CHECK_STATE(m_platformWindow != nullptr)) return;
//	m_platformWindow->SetSize(size);
//}

//------------------------------------------------------------------------------
void UIFrameWindow::RenderContents()
{
	Render_UIRenderer();
}

//------------------------------------------------------------------------------
void UIFrameWindow::PresentRenderingContexts()
{
	// Render
	{
		Details::Renderer* renderer = m_manager->GetGraphicsManager()->GetRenderer();
		renderer->Begin();

		OnPresentRenderingContexts();

		m_manager->GetGraphicsManager()->SwitchActiveContext(nullptr);
		renderer->End();
	}

	// Present
	if (m_swapChain != nullptr)
	{
		bool skip = false;
		if (m_delayedRenderingSkip)
		{
			if (m_manager->GetGraphicsManager()->GetRenderingType() == GraphicsRenderingType::Threaded)
			{
				if (m_manager->GetGraphicsManager()->GetRenderingThread()->IsRunning())
				{
					skip = true;
				}
			}
			else
			{
				// TODO: 上からフレーム遅延しているかをもらうのがいいかも。
				LN_NOTIMPLEMENTED();
			}
		}

		if (!skip)
		{
			m_swapChain->Present();
		}

		// ウィンドウサイズとバックバッファサイズを合わせる
		m_swapChain->MightResizeAndDeviceReset(m_platformWindow->GetSize());
	}
}

//------------------------------------------------------------------------------
void UIFrameWindow::Initialize_UIRenderer()
{
	auto* manager = GetOwnerContext()->GetManager();

	m_drawingContext = NewObject<DrawingContext>();

	// lighting disabled.
	// TODO: NewObject
	auto internalRenderer = RefPtr<detail::NonShadingRenderer>::MakeRef();
	internalRenderer->Initialize(manager->GetGraphicsManager());
	m_internalRenderer = internalRenderer;

	m_drawElementListSet = RefPtr<RenderView>::MakeRef();
	m_drawElementListSet->m_lists.Add(m_drawingContext->GetDrawElementList());

	m_renderDiag = NewObject<RenderDiag>();
}

//------------------------------------------------------------------------------
void UIFrameWindow::Render_UIRenderer()
{
	m_drawingContext->BeginMakeElements();
	m_drawingContext->SetBlendMode(BlendMode::Alpha);
	OnRenderContents();
	UILayoutView::Render(m_drawingContext);
}

//------------------------------------------------------------------------------
void UIFrameWindow::ExecuteDrawList_UIRenderer()
{
	RenderTargetTexture* renderTarget = nullptr;
	DepthBuffer* depthBuffer = nullptr;
	Size viewPixelSize;

	if (m_swapChain != nullptr)
	{
		renderTarget = m_swapChain->GetBackBuffer();
		viewPixelSize = renderTarget->GetSize().ToFloatSize();
	}
	else
	{
		// TODO: 暫定。バックバッファサイズが望ましい。
		// 描画スレッドを使わないなら、Device から直接とっても良い。
		// 描画スレッドを使うなら、Lumino を使用するフレームワークからサイズをもらわなければならない。
		//		この時もらうサイズは、次の描画スレッドの描画で使用されるバックバッファのサイズである。
		viewPixelSize = GetPlatformWindow()->GetSize().ToFloatSize();
	}

	m_renderDiag->Clear();
	
	//detail::CameraInfo cameraInfo;
	m_drawElementListSet->m_cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(this);
	m_drawElementListSet->m_cameraInfo.viewPixelSize = viewPixelSize;
	m_drawElementListSet->m_cameraInfo.viewPosition = Vector3::Zero;
	m_drawElementListSet->m_cameraInfo.viewMatrix = Matrix::Identity;
	m_drawElementListSet->m_cameraInfo.projMatrix = Matrix::MakePerspective2DLH(m_drawElementListSet->m_cameraInfo.viewPixelSize.width, m_drawElementListSet->m_cameraInfo.viewPixelSize.height, 0, 1);
	m_drawElementListSet->m_cameraInfo.viewProjMatrix = m_drawElementListSet->m_cameraInfo.viewMatrix * m_drawElementListSet->m_cameraInfo.projMatrix;
	m_drawElementListSet->m_cameraInfo.viewFrustum = ViewFrustum(m_drawElementListSet->m_cameraInfo.projMatrix);
	m_drawElementListSet->m_cameraInfo.zSortDistanceBase = ZSortDistanceBase::NodeZ;
	m_internalRenderer->Render(
		m_drawElementListSet,
		//m_drawingContext->GetDrawElementList(),
		//m_drawElementListSet->m_cameraInfo,
		renderTarget,
		depthBuffer,
		m_renderDiag);
}



//==============================================================================
// UIMainWindow
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMainWindow, UIFrameWindow)

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
void UIMainWindow::Initialize(PlatformWindow* platformWindow, World2D* defaultWorld2D, World3D* defaultWorld3D)
{
	auto* manager = detail::EngineDomain::GetUIManager();

	m_mainUIContext = LN_NEW UIContext();
	m_mainUIContext->Initialize(manager);

	UIFrameWindow::Initialize(platformWindow, manager->GetGraphicsManager()->GetMainSwapChain(), m_mainUIContext);



	// TODO: m_mainUIViewport を VisualTree の先頭に入れたい対策。InsertVisualTree とかほしいなぁ。。。
	RefPtr<UILayoutPanel> panel = GetLayoutPanel();
	SetLayoutPanel(nullptr);

	m_mainUIViewport = NewObject<UIViewport>();
	m_mainUIViewport->SetBackbufferSize(platformWindow->GetSize().width, platformWindow->GetSize().height);	// TODO: EngineSettings からもらう
	AddVisualChild(m_mainUIViewport);

	m_cameraViewportLayer3D = NewObject<CameraViewportLayer2>(defaultWorld3D, defaultWorld3D->GetMainCamera()->GetCameraComponent());
	m_mainUIViewport->AddViewportLayer(m_cameraViewportLayer3D);
	m_cameraViewportLayer2D = NewObject<CameraViewportLayer2>(defaultWorld2D, defaultWorld2D->GetMainCamera()->GetCameraComponent());
	m_mainUIViewport->AddViewportLayer(m_cameraViewportLayer2D);
	m_uiLayer = NewObject<UILayoutLayer>();
	m_mainUIViewport->AddViewportLayer(m_uiLayer);

	SetLayoutPanel(panel);


	SetSizeInternal(platformWindow->GetSize().ToFloatSize());

	// SwapChain のサイズを Viewport へ通知
	UpdateViewportTransform();
}

//------------------------------------------------------------------------------
UIViewport* UIMainWindow::GetViewport() const
{
	return m_mainUIViewport;
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
Size UIMainWindow::ArrangeOverride(const Size& finalSize)
{
	Size renderSize = UIFrameWindow::ArrangeOverride(finalSize);

	m_mainUIViewport->ArrangeLayout(Rect(0, 0, renderSize));

	return renderSize;
}

void UIMainWindow::OnRenderContents()
{
}

void UIMainWindow::OnPresentRenderingContexts()
{
	UIFrameWindow::OnPresentRenderingContexts();
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
	//Size viewSize;

	//SwapChain* swapChain = GetSwapChain();
	//if (swapChain != nullptr)
	//{
	//	const SizeI& bbSize = GetSwapChain()->GetBackBuffer()->GetSize();
	//	viewSize.width = bbSize;
	//	viewSize.height = bbSize;
	//}

	//SizeI size = GetPlatformWindow()->GetSize();
	//m_mainViewport->UpdateLayersTransform(size.ToFloatSize());
}

//------------------------------------------------------------------------------
CameraViewportLayer2* UIMainWindow::GetDefaultCameraViewportLayer2D() const
{
	return m_cameraViewportLayer2D;
}

//------------------------------------------------------------------------------
CameraViewportLayer2* UIMainWindow::GetDefaultCameraViewportLayer3D() const
{
	return m_cameraViewportLayer3D;
}

//------------------------------------------------------------------------------
UILayoutLayer* UIMainWindow::GetDefaultUILayer() const
{
	return m_uiLayer;
}

//==============================================================================
// UINativeHostWindow
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UINativeHostWindow, UIFrameWindow)

//------------------------------------------------------------------------------
UINativeHostWindowPtr UINativeHostWindow::Create(intptr_t windowHandle)
{
	auto ptr = UINativeHostWindowPtr::MakeRef();
	ptr->Initialize(windowHandle);
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
void UINativeHostWindow::Initialize(intptr_t windowHandle)
{
	if (LN_CHECK_ARG(windowHandle != 0)) return;

	auto* manager = detail::EngineDomain::GetUIManager();

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

	UIFrameWindow::Initialize(window, swap, m_mainUIContext);
}

LN_NAMESPACE_END
