
#include "Internal.h"
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include "Graphics/GraphicsManager.h"
#include "EngineManager.h"
#include "EngineDiagCore.h"
#include "EngineDiagRenderer.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// EngineDiagCore
//=============================================================================

EngineDiagCore EngineDiagCore::Instance;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineDiagCore::EngineDiagCore()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineDiagCore::~EngineDiagCore()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineDiagCore::Initialize(EngineManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;
}

float EngineDiagCore::GetMainFPS() const
{
	return m_manager->GetFpsController().GetFps();
}

float EngineDiagCore::GetMainFPSCapacity() const
{
	return m_manager->GetFpsController().GetCapacityFps();
}

//=============================================================================
// EngineDiagRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineDiagRenderer::EngineDiagRenderer()
	: m_diagCore(nullptr)
	, m_font(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineDiagRenderer::~EngineDiagRenderer()
{
	LN_SAFE_RELEASE(m_font);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineDiagRenderer::Initialize(GraphicsManager* manager, EngineDiagCore* diagCore)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	LN_CHECK_STATE(m_font == nullptr);
	m_diagCore = diagCore;

	m_font = Font::CreateBuiltInBitmapFontInternal(manager->GetFontManager(), 7);
	//m_windowRect.Set(640 - 8 - 300, 8, 300, 256);	// TODO
	m_windowRect.Set(8, 8, 300, 300);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineDiagRenderer::Render(GraphicsContext* g, const Vector2& viewSize)
{
	PointF location(m_windowRect.x, m_windowRect.y);
	g->SetOpacity(0.5f);
	g->SetFont(m_font);

	//g->SetBrush(ColorBrush::DimGray);

	// ウィンドウ背景
	g->DrawRectangle(m_windowRect, Color::DimGray);

	// キャプションバー
	g->DrawRectangle(RectF(m_windowRect.GetTopLeft(), m_windowRect.width, 20), Color::Black);


	g->SetBrush(ColorBrush::White);
	g->DrawText(_T("Statistics"), m_windowRect, StringFormatFlags::CenterAlignment);


	//LocalPainter painter(SizeF(viewSize.X, viewSize.Y), m_manager);
	////g->SetProjection(SizeF(viewSize.X, viewSize.Y), 0, 1000);
	//g->SetOpacity(0.5f);
	//g->SetFont(m_font);

	//// ウィンドウ背景
	//g->SetBrush(ColorBrush::DimGray);
	//g->DrawRectangle(m_windowRect);

	//// キャプションバー
	//g->SetBrush(ColorBrush::Black);
	//g->DrawRectangle(RectF(m_windowRect.GetTopLeft(), m_windowRect.Width, 20));


	//g->SetBrush(ColorBrush::White);
	//g->SetOpacity(1.0f);
	//g->DrawString(_T("Statistics"), -1, m_windowRect, StringFormatFlags::CenterAlignment);

	location.y += 24;

	//-----------------------------------------------------
	// Main info
	g->DrawText(_T("Main information:"), PointF(location.x + 8, location.y));
	location.y += 16;
	location.x += 16;

	TCHAR text[256] = { 0 };

	//StringTraits::SPrintf(text, 256, _T("Graphics API    : %s"), m_manager->GetGraphicsAPI().ToString().c_str());
	//g->DrawText(text, -1, location);
	location.y += 16;

	//StringTraits::SPrintf(text, 256, _T("Rendering type  : %s"), m_manager->GetRenderingType().ToString().c_str());
	//g->DrawText(text, -1, location);
	location.y += 16;

	StringTraits::SPrintf(text, 256, _T("Average FPS     : %.1f"), m_diagCore->GetMainFPS());
	g->DrawText(text, location);

	StringTraits::SPrintf(text, 256, _T(" / Capacity : %.1f"), m_diagCore->GetMainFPSCapacity());
	g->DrawText(text, PointF(location.x + 150, location.y));
	location.y += 16;

	//StringTraits::SPrintf(text, 256, _T("Window Size     : %d x %d"), m_profiler->GetCommitedMainWindowSize().Width, m_profiler->GetCommitedMainWindowSize().Height);
	//g->DrawText(text, -1, location);
	//location.Y += 16;

	//StringTraits::SPrintf(text, 256, _T("Backbuffer Size : %d x %d"), m_profiler->GetCommitedMainBackbufferSize().Width, m_profiler->GetCommitedMainBackbufferSize().Height);
	//g->DrawText(text, -1, location);
	//location.Y += 24;
	//location.X -= 16;


	g->Flush();
}

LN_NAMESPACE_END
