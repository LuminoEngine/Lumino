
#include "Internal.h"
#include <Lumino/Platform/PlatformWindow.h>
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/EngineDiag.h>
#include "Graphics/GraphicsManager.h"
#include "Graphics/Text/FontManager.h"
#include "EngineManager.h"
#include "EngineDiagCore.h"
#include "EngineDiagViewer.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// EngineDiag
//==============================================================================

//------------------------------------------------------------------------------
void EngineDiag::setDisplayMode(EngineDiagDisplayMode mode)
{
	EngineManager::getInstance()->getEngineDiagViewer()->setDisplayMode(mode);
}

//------------------------------------------------------------------------------
int EngineDiag::getGraphicsDeviceDrawCount()
{
	return EngineDiagCore::Instance.getGraphicsDeviceDrawCount();
}

//------------------------------------------------------------------------------
int EngineDiag::getVisualNodeDrawCount()
{
	return EngineDiagCore::Instance.getVisualNodeDrawCount();
}

//==============================================================================
// EngineDiagCore
//==============================================================================

EngineDiagCore EngineDiagCore::Instance;

//------------------------------------------------------------------------------
EngineDiagCore::EngineDiagCore()
	: m_graphicsDeviceDrawCount(0)
{
}

//------------------------------------------------------------------------------
EngineDiagCore::~EngineDiagCore()
{
}

//------------------------------------------------------------------------------
void EngineDiagCore::initialize(EngineManager* manager)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;
	m_manager = manager;
}

//------------------------------------------------------------------------------
//void EngineDiagCore::reportCapability(int indent, const char* name, const char* value)
//{
//	StringA str(indent, '\t');
//	str = StringA::format("{0}{1} : {2}", str, name, value);
//	Logger::writeLine(str.c_str());
//}

//------------------------------------------------------------------------------
float EngineDiagCore::getMainFPS() const
{
	return m_manager->getFpsController().getFps();
}

//------------------------------------------------------------------------------
float EngineDiagCore::getMainFPSCapacity() const
{
	return m_manager->getFpsController().getCapacityFps();
}

//==============================================================================
// EngineDiagViewer
//==============================================================================

//------------------------------------------------------------------------------
EngineDiagViewer::EngineDiagViewer()
	: m_diagCore(nullptr)
	, m_mainWindow(nullptr)
	, m_font(nullptr)
	, m_displayMode(EngineDiagDisplayMode::Hide)
{
#ifdef LN_DEBUG
	m_displayMode = EngineDiagDisplayMode::FpsSummary;
#endif
}

//------------------------------------------------------------------------------
EngineDiagViewer::~EngineDiagViewer()
{
}

//------------------------------------------------------------------------------
void EngineDiagViewer::initialize(EngineManager* manager, EngineDiagCore* diagCore)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;
	m_diagCore = diagCore;
	m_mainWindow = manager->getPlatformManager()->getMainWindow();
	m_originalMainWindowTitle = m_mainWindow->getTitleText();

	m_font = manager->getGraphicsManager()->getFontManager()->getBuiltinFont(BuiltinFontSize::XXSmall);
	//m_windowRect.Set(640 - 8 - 300, 8, 300, 256);	// TODO
	m_windowRect.set(8, 8, 300, 300);
}

//------------------------------------------------------------------------------
void EngineDiagViewer::toggleDisplayMode()
{
	int d = (int)m_displayMode;
	d = (d + 1) % DisplayModeCycle;
	m_displayMode = (EngineDiagDisplayMode)d;
}

//------------------------------------------------------------------------------
void EngineDiagViewer::updateFrame()
{
	if (m_displayMode == EngineDiagDisplayMode::FpsSummary)
	{
		String str = String::format(_T("{0} - MainFPS:{1}/{2}"), m_originalMainWindowTitle, m_diagCore->getMainFPS(), m_diagCore->getMainFPSCapacity());
		m_mainWindow->setTitleText(str);
	}
}

//------------------------------------------------------------------------------
#if 0
void EngineDiagViewer::Render(DrawingContext* g, const Vector2& viewSize)
{
	if (m_displayMode != DisplayMode_Details) return;

	Point location(m_windowRect.x, m_windowRect.y);
	g->SetOpacity(0.5f);
	g->SetFont(m_font);

	//g->setBrush(ColorBrush::DimGray);

	// ウィンドウ背景
	g->DrawRectangle(m_windowRect);

	// キャプションバー
	g->DrawRectangle(RectF(m_windowRect.GetTopLeft(), m_windowRect.width, 20));


	g->SetBrush(ColorBrush::White);
	g->DrawText(_T("Statistics"), m_windowRect, StringFormatFlags::CenterAlignment);


	//LocalPainter painter(Size(viewSize.X, viewSize.Y), m_manager);
	////g->SetProjection(Size(viewSize.X, viewSize.Y), 0, 1000);
	//g->setOpacity(0.5f);
	//g->setFont(m_font);

	//// ウィンドウ背景
	//g->setBrush(ColorBrush::DimGray);
	//g->drawRectangle(m_windowRect);

	//// キャプションバー
	//g->setBrush(ColorBrush::Black);
	//g->drawRectangle(RectF(m_windowRect.getTopLeft(), m_windowRect.Width, 20));


	//g->setBrush(ColorBrush::White);
	//g->setOpacity(1.0f);
	//g->drawString(_T("Statistics"), -1, m_windowRect, StringFormatFlags::CenterAlignment);

	location.y += 24;

	//-----------------------------------------------------
	// Main info
	g->DrawText(_T("Main information:"), Point(location.x + 8, location.y));
	location.y += 16;
	location.x += 16;

	Char text[256] = { 0 };

	//StringTraits::SPrintf(text, 256, _T("Graphics API    : %s"), m_manager->getGraphicsAPI().ToString().c_str());
	//g->drawText(text, -1, location);
	location.y += 16;

	//StringTraits::SPrintf(text, 256, _T("Rendering type  : %s"), m_manager->getRenderingType().ToString().c_str());
	//g->drawText(text, -1, location);
	location.y += 16;

	StringTraits::SPrintf(text, 256, _T("Average FPS     : %.1f"), m_diagCore->GetMainFPS());
	g->DrawText(text, location);

	StringTraits::SPrintf(text, 256, _T(" / Capacity : %.1f"), m_diagCore->GetMainFPSCapacity());
	g->DrawText(text, Point(location.x + 150, location.y));
	location.y += 16;

	//StringTraits::SPrintf(text, 256, _T("Window Size     : %d x %d"), m_profiler->getCommitedMainWindowSize().Width, m_profiler->getCommitedMainWindowSize().Height);
	//g->drawText(text, -1, location);
	//location.Y += 16;

	//StringTraits::SPrintf(text, 256, _T("Backbuffer Size : %d x %d"), m_profiler->getCommitedMainBackbufferSize().Width, m_profiler->getCommitedMainBackbufferSize().Height);
	//g->drawText(text, -1, location);
	//location.Y += 24;
	//location.X -= 16;


	g->Flush();
}
#endif

LN_NAMESPACE_END
