
#include "Internal.h"
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/DrawingContext.h>
#include "Graphics\GraphicsManager.h"
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

//=============================================================================
// EngineDiagRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EngineDiagRenderer::EngineDiagRenderer()
	: m_diag(nullptr)
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
void EngineDiagRenderer::Initialize(GraphicsManager* manager, EngineDiagCore* diag)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	LN_CHECK_STATE(m_font == nullptr);

	m_font = Font::CreateBuiltInBitmapFontInternal(manager->GetFontManager(), 7);
	m_windowRect.Set(640 - 8 - 300, 8, 300, 256);	// TODO
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EngineDiagRenderer::Render(GraphicsContext* g, const Vector2& viewSize)
{
	PointF location(m_windowRect.X, m_windowRect.Y);
	g->SetOpacity(0.5f);

	//g->SetBrush(ColorBrush::DimGray);
	g->DrawRectangle(RectF(0, 0, 100, 200), ColorF(1, 0, 0, 0.5));

	g->Flush();

	//LocalPainter painter(SizeF(viewSize.X, viewSize.Y), m_manager);
	////painter.SetProjection(SizeF(viewSize.X, viewSize.Y), 0, 1000);
	//painter.SetOpacity(0.5f);
	//painter.SetFont(m_font);

	//// ウィンドウ背景
	//painter.SetBrush(ColorBrush::DimGray);
	//painter.DrawRectangle(m_windowRect);

	//// キャプションバー
	//painter.SetBrush(ColorBrush::Black);
	//painter.DrawRectangle(RectF(m_windowRect.GetTopLeft(), m_windowRect.Width, 20));


	//painter.SetBrush(ColorBrush::White);
	//painter.SetOpacity(1.0f);
	//painter.DrawString(_T("Statistics"), -1, m_windowRect, StringFormatFlags::CenterAlignment);

	//location.Y += 24;
}

LN_NAMESPACE_END
