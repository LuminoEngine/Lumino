
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Viewport.h>
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/Rendering/SceneRenderer.h>
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
#include "../Graphics/CoreGraphicsRenderFeature.h"
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
	setSpecialElementType(UISpecialElementType::FrameWindow);
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
	m_platformWindow->attachEventListener(this, 0);
	initialize_UIRenderer();
	m_manager->addFrameWindow(this);
}

//------------------------------------------------------------------------------
void UIFrameWindow::initialize()
{
	m_manager = detail::EngineDomain::getUIManager();

	WindowCreationSettings ws;
	RefPtr<PlatformWindow> window(m_manager->getPlatformManager()->getWindowManager()->createSubWindow(ws), false);
	auto swapChain = RefPtr<SwapChain>::makeRef();
	swapChain->initializeSub(m_manager->getGraphicsManager(), window);

	initialize(window, swapChain, UIContext::getMainContext());
}

//------------------------------------------------------------------------------
void UIFrameWindow::Dispose()
{
	if (m_manager != nullptr)
	{
		m_platformWindow->detachEventListener(this);
		m_swapChain.safeRelease();
		m_platformWindow.safeRelease();
		m_manager->removeFrameWindow(this);
		m_manager = nullptr;
	}
}

//------------------------------------------------------------------------------
PlatformWindow* UIFrameWindow::getPlatformWindow() const
{
	return m_platformWindow;
}

//------------------------------------------------------------------------------
SwapChain* UIFrameWindow::getSwapChain() const
{
	return m_swapChain;
}

//------------------------------------------------------------------------------
DrawingContext* UIFrameWindow::getDrawingContext() const
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
void UIFrameWindow::onPresentRenderingContexts()
{
	executeDrawList_UIRenderer();
}

//------------------------------------------------------------------------------
//void UIFrameWindow::setSize(const SizeI& size)
//{
//	if (LN_CHECK_STATE(m_platformWindow != nullptr)) return;
//	m_platformWindow->setSize(size);
//}

//------------------------------------------------------------------------------
void UIFrameWindow::renderContents()
{
	m_drawingContext->beginMakeElements();
	m_drawingContext->clear(ClearFlags::Depth | ClearFlags::Stencil, Color::Transparency);
	m_drawingContext->setBlendMode(BlendMode::Alpha);
	UILayoutView::render(m_drawingContext);
}

//------------------------------------------------------------------------------
void UIFrameWindow::presentRenderingContexts()
{
	// render
	{
		//detail::CoreGraphicsRenderFeature* renderer = m_manager->getGraphicsManager()->getRenderer();
		//renderer->begin();

		onPresentRenderingContexts();

		m_manager->getGraphicsManager()->switchActiveContext(nullptr);
		//renderer->end();
	}

	// present
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
			m_swapChain->present();
		}

		// ウィンドウサイズとバックバッファサイズを合わせる
		m_swapChain->MightResizeAndDeviceReset(m_platformWindow->getSize());
	}
}

//------------------------------------------------------------------------------
void UIFrameWindow::initialize_UIRenderer()
{
	auto* manager = getOwnerContext()->getManager();

	m_drawingContext = newObject<DrawingContext>();

	// lighting disabled.
	// TODO: newObject
	auto internalRenderer = RefPtr<detail::NonShadingRenderer>::makeRef();
	internalRenderer->initialize(manager->getGraphicsManager());
	m_internalRenderer = internalRenderer;

	m_drawElementListSet = newObject<RenderView>();
	m_drawElementListSet->m_lists.add(m_drawingContext->getDrawElementList());
	m_drawElementListSet->setSceneRenderer(m_internalRenderer);

	m_renderDiag = newObject<RenderDiag>();
}

//------------------------------------------------------------------------------
void UIFrameWindow::executeDrawList_UIRenderer()
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
		viewPixelSize = getPlatformWindow()->getSize().toFloatSize();
	}

	m_renderDiag->clear();
	
	//detail::CameraInfo cameraInfo;
	m_drawElementListSet->m_cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(this);
	m_drawElementListSet->m_cameraInfo.viewPixelSize = viewPixelSize;
	m_drawElementListSet->m_cameraInfo.viewPosition = Vector3::Zero;
	m_drawElementListSet->m_cameraInfo.viewDirection = Vector3::UnitZ;
	m_drawElementListSet->m_cameraInfo.viewMatrix = Matrix::Identity;
	m_drawElementListSet->m_cameraInfo.projMatrix = Matrix::makePerspective2DLH(m_drawElementListSet->m_cameraInfo.viewPixelSize.width, m_drawElementListSet->m_cameraInfo.viewPixelSize.height, 0, 1);
	m_drawElementListSet->m_cameraInfo.viewProjMatrix = m_drawElementListSet->m_cameraInfo.viewMatrix * m_drawElementListSet->m_cameraInfo.projMatrix;
	m_drawElementListSet->m_cameraInfo.viewFrustum = ViewFrustum(m_drawElementListSet->m_cameraInfo.projMatrix);
	m_drawElementListSet->m_cameraInfo.zSortDistanceBase = ZSortDistanceBase::NodeZ;
	m_internalRenderer->render(
		m_drawElementListSet,
		//m_drawingContext->getDrawElementList(),
		//m_drawElementListSet->m_cameraInfo,
		renderTarget,
		depthBuffer,
		m_renderDiag,
		false,
		Color::White);
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
	//RefPtr<UILayoutPanel> panel = getLayoutPanel();
	//setLayoutPanel(nullptr);

	m_mainUIViewport = newObject<UIViewport>();
	m_mainUIViewport->setBackbufferSize(platformWindow->getSize().width, platformWindow->getSize().height);	// TODO: EngineSettings からもらう
	m_mainUIViewport->setAnchor(AlignmentAnchor::LeftOffsets | AlignmentAnchor::TopOffsets | AlignmentAnchor::RightOffsets | AlignmentAnchor::BottomOffsets);
																											//addVisualChild(m_mainUIViewport);
	addChild(m_mainUIViewport);

	m_cameraViewportLayer3D = newObject<CameraViewportLayer2>(defaultWorld3D, defaultWorld3D->getMainCamera()->getCameraComponent());
	m_mainUIViewport->addViewportLayer(m_cameraViewportLayer3D);
	m_cameraViewportLayer2D = newObject<CameraViewportLayer2>(defaultWorld2D, defaultWorld2D->getMainCamera()->getCameraComponent());
	m_mainUIViewport->addViewportLayer(m_cameraViewportLayer2D);
	m_uiLayer = newObject<UILayoutLayer>();
	m_mainUIViewport->addViewportLayer(m_uiLayer);

	//setLayoutPanel(panel);


	setSizeInternal(platformWindow->getSize().toFloatSize());

	// SwapChain のサイズを Viewport へ通知
	updateViewportTransform();
}

//------------------------------------------------------------------------------
UIViewport* UIMainWindow::getViewport() const
{
	return m_mainUIViewport;
}

//------------------------------------------------------------------------------
//void UIMainWindow::injectElapsedTime(float elapsedTime)
//{
//	m_mainUIContext->injectElapsedTime(elapsedTime);
//}

//------------------------------------------------------------------------------
void UIMainWindow::updateLayout(const Size& viewSize)
{
	UIFrameWindow::updateLayout(viewSize);
}

//------------------------------------------------------------------------------
void UIMainWindow::renderUI()
{
}

//------------------------------------------------------------------------------
Size UIMainWindow::arrangeOverride(const Size& finalSize)
{
	Size renderSize = UIFrameWindow::arrangeOverride(finalSize);

	//m_mainUIViewport->arrangeLayout(Rect(0, 0, renderSize));

	return renderSize;
}

void UIMainWindow::onPresentRenderingContexts()
{
	UIFrameWindow::onPresentRenderingContexts();
}

void UIMainWindow::presentRenderingContexts()
{
	UIFrameWindow::presentRenderingContexts();

	// SwapChain のサイズを Viewport へ通知
	// ※ SwapChain のサイズが「本当に」変わるタイミングは、描画コマンドが確定する present の後。
	//    フレーム更新の最初で行ってもよいが、この時点で行ってもよい。
	updateViewportTransform();
}

void UIMainWindow::updateViewportTransform()
{
	//Size viewSize;

	//SwapChain* swapChain = getSwapChain();
	//if (swapChain != nullptr)
	//{
	//	const SizeI& bbSize = getSwapChain()->getBackBuffer()->GetSize();
	//	viewSize.width = bbSize;
	//	viewSize.height = bbSize;
	//}

	//SizeI size = getPlatformWindow()->GetSize();
	//m_mainViewport->UpdateLayersTransform(size.toFloatSize());
}

//------------------------------------------------------------------------------
CameraViewportLayer2* UIMainWindow::getDefaultCameraViewportLayer2D() const
{
	return m_cameraViewportLayer2D;
}

//------------------------------------------------------------------------------
CameraViewportLayer2* UIMainWindow::getDefaultCameraViewportLayer3D() const
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
	RefPtr<PlatformWindow> window(manager->getPlatformManager()->getWindowManager()->createSubWindow(ws), false);

	auto swap = RefPtr<SwapChain>::makeRef();
	swap->initializeSub(manager->getGraphicsManager(), window);


	m_mainUIContext = LN_NEW UIContext();
	m_mainUIContext->initialize(manager);

	UIFrameWindow::initialize(window, swap, m_mainUIContext);
}

LN_NAMESPACE_END
