
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
	SetSpecialElementType(UISpecialElementType::FrameWindow);
}

//------------------------------------------------------------------------------
UIFrameWindow::~UIFrameWindow()
{
}

//------------------------------------------------------------------------------
void UIFrameWindow::initialize(PlatformWindow* platformWindow, SwapChain* swapChain, UIContext* context)
{
	if (LN_CHECK_ARG(platformWindow != nullptr)) return;
	m_manager = detail::EngineDomain::getUIManager();
	m_platformWindow = platformWindow;
	m_swapChain = swapChain;

	UILayoutView::initialize(context, platformWindow);
	m_platformWindow->AttachEventListener(this, 0);
	Initialize_UIRenderer();
	m_manager->AddFrameWindow(this);
}

//------------------------------------------------------------------------------
void UIFrameWindow::initialize()
{
	m_manager = detail::EngineDomain::getUIManager();

	WindowCreationSettings ws;
	RefPtr<PlatformWindow> window(m_manager->getPlatformManager()->GetWindowManager()->CreateSubWindow(ws), false);
	auto swapChain = RefPtr<SwapChain>::makeRef();
	swapChain->InitializeSub(m_manager->getGraphicsManager(), window);

	initialize(window, swapChain, UIContext::GetMainContext());
}

//------------------------------------------------------------------------------
void UIFrameWindow::Dispose()
{
	if (m_manager != nullptr)
	{
		m_platformWindow->DetachEventListener(this);
		m_swapChain.safeRelease();
		m_platformWindow.safeRelease();
		m_manager->RemoveFrameWindow(this);
		m_manager = nullptr;
	}
}

//------------------------------------------------------------------------------
PlatformWindow* UIFrameWindow::GetPlatformWindow() const
{
	return m_platformWindow;
}

//------------------------------------------------------------------------------
SwapChain* UIFrameWindow::GetSwapChain() const
{
	return m_swapChain;
}

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
bool UIFrameWindow::onEvent(const PlatformEventArgs& e)
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
//void UIFrameWindow::setSize(const SizeI& size)
//{
//	if (LN_CHECK_STATE(m_platformWindow != nullptr)) return;
//	m_platformWindow->setSize(size);
//}

//------------------------------------------------------------------------------
void UIFrameWindow::RenderContents()
{
	Render_UIRenderer();
}

//------------------------------------------------------------------------------
void UIFrameWindow::PresentRenderingContexts()
{
	// render
	{
		Details::Renderer* renderer = m_manager->getGraphicsManager()->getRenderer();
		renderer->begin();

		OnPresentRenderingContexts();

		m_manager->getGraphicsManager()->switchActiveContext(nullptr);
		renderer->end();
	}

	// Present
	if (m_swapChain != nullptr)
	{
		bool skip = false;
		if (m_delayedRenderingSkip)
		{
			if (m_manager->getGraphicsManager()->getRenderingType() == GraphicsRenderingType::Threaded)
			{
				if (m_manager->getGraphicsManager()->getRenderingThread()->isRunning())
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
		m_swapChain->MightResizeAndDeviceReset(m_platformWindow->getSize());
	}
}

//------------------------------------------------------------------------------
void UIFrameWindow::Initialize_UIRenderer()
{
	auto* manager = GetOwnerContext()->getManager();

	m_drawingContext = newObject<DrawingContext>();

	// lighting disabled.
	// TODO: newObject
	auto internalRenderer = RefPtr<detail::NonShadingRenderer>::makeRef();
	internalRenderer->initialize(manager->getGraphicsManager());
	m_internalRenderer = internalRenderer;

	m_drawElementListSet = RefPtr<RenderView>::makeRef();
	m_drawElementListSet->m_lists.add(m_drawingContext->GetDrawElementList());

	m_renderDiag = newObject<RenderDiag>();
}

//------------------------------------------------------------------------------
void UIFrameWindow::Render_UIRenderer()
{
	m_drawingContext->BeginMakeElements();
	m_drawingContext->setBlendMode(BlendMode::Alpha);
	OnRenderContents();
	UILayoutView::render(m_drawingContext);
}

//------------------------------------------------------------------------------
void UIFrameWindow::ExecuteDrawList_UIRenderer()
{
	RenderTargetTexture* renderTarget = nullptr;
	DepthBuffer* depthBuffer = nullptr;
	Size viewPixelSize;

	if (m_swapChain != nullptr)
	{
		renderTarget = m_swapChain->getBackBuffer();
		viewPixelSize = renderTarget->getSize().toFloatSize();
	}
	else
	{
		// TODO: 暫定。バックバッファサイズが望ましい。
		// 描画スレッドを使わないなら、Device から直接とっても良い。
		// 描画スレッドを使うなら、Lumino を使用するフレームワークからサイズをもらわなければならない。
		//		この時もらうサイズは、次の描画スレッドの描画で使用されるバックバッファのサイズである。
		viewPixelSize = GetPlatformWindow()->getSize().toFloatSize();
	}

	m_renderDiag->clear();
	
	//detail::CameraInfo cameraInfo;
	m_drawElementListSet->m_cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(this);
	m_drawElementListSet->m_cameraInfo.viewPixelSize = viewPixelSize;
	m_drawElementListSet->m_cameraInfo.viewPosition = Vector3::Zero;
	m_drawElementListSet->m_cameraInfo.viewMatrix = Matrix::Identity;
	m_drawElementListSet->m_cameraInfo.projMatrix = Matrix::makePerspective2DLH(m_drawElementListSet->m_cameraInfo.viewPixelSize.width, m_drawElementListSet->m_cameraInfo.viewPixelSize.height, 0, 1);
	m_drawElementListSet->m_cameraInfo.viewProjMatrix = m_drawElementListSet->m_cameraInfo.viewMatrix * m_drawElementListSet->m_cameraInfo.projMatrix;
	m_drawElementListSet->m_cameraInfo.viewFrustum = ViewFrustum(m_drawElementListSet->m_cameraInfo.projMatrix);
	m_drawElementListSet->m_cameraInfo.zSortDistanceBase = ZSortDistanceBase::NodeZ;
	m_internalRenderer->render(
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
void UIMainWindow::initialize(PlatformWindow* platformWindow, World2D* defaultWorld2D, World3D* defaultWorld3D)
{
	auto* manager = detail::EngineDomain::getUIManager();

	m_mainUIContext = LN_NEW UIContext();
	m_mainUIContext->initialize(manager);

	UIFrameWindow::initialize(platformWindow, manager->getGraphicsManager()->getMainSwapChain(), m_mainUIContext);



	// TODO: m_mainUIViewport を VisualTree の先頭に入れたい対策。InsertVisualTree とかほしいなぁ。。。
	RefPtr<UILayoutPanel> panel = GetLayoutPanel();
	SetLayoutPanel(nullptr);

	m_mainUIViewport = newObject<UIViewport>();
	m_mainUIViewport->SetBackbufferSize(platformWindow->getSize().width, platformWindow->getSize().height);	// TODO: EngineSettings からもらう
	AddVisualChild(m_mainUIViewport);

	m_cameraViewportLayer3D = newObject<CameraViewportLayer2>(defaultWorld3D, defaultWorld3D->getMainCamera()->GetCameraComponent());
	m_mainUIViewport->AddViewportLayer(m_cameraViewportLayer3D);
	m_cameraViewportLayer2D = newObject<CameraViewportLayer2>(defaultWorld2D, defaultWorld2D->getMainCamera()->GetCameraComponent());
	m_mainUIViewport->AddViewportLayer(m_cameraViewportLayer2D);
	m_uiLayer = newObject<UILayoutLayer>();
	m_mainUIViewport->AddViewportLayer(m_uiLayer);

	SetLayoutPanel(panel);


	SetSizeInternal(platformWindow->getSize().toFloatSize());

	// SwapChain のサイズを Viewport へ通知
	UpdateViewportTransform();
}

//------------------------------------------------------------------------------
UIViewport* UIMainWindow::GetViewport() const
{
	return m_mainUIViewport;
}

//------------------------------------------------------------------------------
//void UIMainWindow::InjectElapsedTime(float elapsedTime)
//{
//	m_mainUIContext->InjectElapsedTime(elapsedTime);
//}

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
Size UIMainWindow::arrangeOverride(const Size& finalSize)
{
	Size renderSize = UIFrameWindow::arrangeOverride(finalSize);

	m_mainUIViewport->arrangeLayout(Rect(0, 0, renderSize));

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
	//	const SizeI& bbSize = GetSwapChain()->getBackBuffer()->GetSize();
	//	viewSize.width = bbSize;
	//	viewSize.height = bbSize;
	//}

	//SizeI size = GetPlatformWindow()->GetSize();
	//m_mainViewport->UpdateLayersTransform(size.toFloatSize());
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
UILayoutLayer* UIMainWindow::getDefaultUILayer() const
{
	return m_uiLayer;
}

//==============================================================================
// UINativeHostWindow
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UINativeHostWindow, UIFrameWindow)

//------------------------------------------------------------------------------
UINativeHostWindowPtr UINativeHostWindow::create(intptr_t windowHandle)
{
	auto ptr = UINativeHostWindowPtr::makeRef();
	ptr->initialize(windowHandle);
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
void UINativeHostWindow::initialize(intptr_t windowHandle)
{
	if (LN_CHECK_ARG(windowHandle != 0)) return;

	auto* manager = detail::EngineDomain::getUIManager();

	WindowCreationSettings ws;
	//ws.title;		// TODO
	//Size	clientSize = Size(640, 480);	// クライアント領域のピクセルサイズ
	//bool	fullscreen = false;				// フルスクリーンモードで作成するかどうか
	//bool	resizable = true;				// 可変ウィンドウとして作成するかどうか
	ws.userWindow = windowHandle;
	RefPtr<PlatformWindow> window(manager->getPlatformManager()->GetWindowManager()->CreateSubWindow(ws), false);

	auto swap = RefPtr<SwapChain>::makeRef();
	swap->InitializeSub(manager->getGraphicsManager(), window);


	m_mainUIContext = LN_NEW UIContext();
	m_mainUIContext->initialize(manager);

	UIFrameWindow::initialize(window, swap, m_mainUIContext);
}

LN_NAMESPACE_END
