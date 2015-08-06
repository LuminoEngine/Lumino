
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include "ProfilerRenderer.h"

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// ProfilerRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ProfilerRenderer::ProfilerRenderer(GraphicsManager* manager, Profiler* profiler)
	: m_manager(manager)
	, m_profiler(profiler)
{
	m_font.Attach(Font::CreateBuiltInBitmapFont(m_manager->GetFontManager(), 7));
	m_windowRect.Set(8, 8, 300, 256);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ProfilerRenderer::~ProfilerRenderer()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ProfilerRenderer::Render(const Vector2& viewSize)
{
	PointF location(m_windowRect.X, m_windowRect.Y);

	Painter painter(m_manager);
	painter.SetProjection(SizeF(viewSize.X, viewSize.Y), 0, 1000);
	painter.SetOpacity(0.5f);
	painter.SetFont(m_font);

	// ウィンドウ背景
	painter.SetBrush(ColorBrush::Black);
	painter.DrawRectangle(m_windowRect);

	// キャプションバー
	painter.SetBrush(ColorBrush::Black);
	painter.DrawRectangle(RectF(m_windowRect.GetTopLeft(), m_windowRect.Width, 20));


	painter.SetBrush(ColorBrush::White);
	painter.SetOpacity(1.0f);
	painter.DrawString(_T("Statistics"), -1, m_windowRect, StringFormatFlags::CenterAlignment);

	location.Y += 24;

	//-----------------------------------------------------
	// Main info
	painter.DrawString(_T("Main information:"), -1, PointF(location.X + 8, location.Y));
	location.Y += 16;
	location.X += 16;

	TCHAR text[256] = { 0 };

	StringTraits::SPrintf(text, 256, _T("Graphics API    : %s"), m_manager->GetGraphicsAPI().ToString().GetCStr());
	painter.DrawString(text, -1, location);
	location.Y += 16;

	StringTraits::SPrintf(text, 256, _T("Average FPS     : %.1f"), m_profiler->GetCommitedMainFPS());
	painter.DrawString(text, -1, location);

	StringTraits::SPrintf(text, 256, _T(" / Capacity : %.1f"), m_profiler->GetCommitedMainFPSCapacity());
	painter.DrawString(text, -1, PointF(location.X + 150, location.Y));
	location.Y += 16;

	StringTraits::SPrintf(text, 256, _T("Window Size     : %d x %d"), m_profiler->GetCommitedMainWindowSize().Width, m_profiler->GetCommitedMainWindowSize().Height );
	painter.DrawString(text, -1, location);
	location.Y += 16;

	StringTraits::SPrintf(text, 256, _T("Backbuffer Size : %d x %d"), m_profiler->GetCommitedMainBackbufferSize().Width, m_profiler->GetCommitedMainBackbufferSize().Height);
	painter.DrawString(text, -1, location);
	location.Y += 24;
	location.X -= 16;

	//-----------------------------------------------------
	// Threads performance
	painter.DrawString(_T("Threads performance:"), -1, PointF(location.X + 8, location.Y));
	location.Y += 16;

	// グループリストの描画
	RectF listRect(location.X + 16, location.Y, m_windowRect.Width - 32, m_windowRect.Height - location.Y);
	DrawGroupList(painter, listRect);

	return;



	//Matrix proj = Matrix::Perspective2DLH(viewSize.X, viewSize.Y, 0.f, 1000.f);
	//m_spriteRenderer->SetViewProjMatrix(Matrix::Identity, proj);
	//m_spriteRenderer->SetViewPixelSize(Size(viewSize.X, viewSize.Y));		// TODO Vector2 の set があってもいいかも

	//float WindowWidth = 256;
	//float WindowHeight = 256;
	//Vector3 location(viewSize.X - 8 - WindowWidth, 8, 0);

	//// ウィンドウ背景
	//m_spriteRenderer->DrawRequest2D(
	//	Vector3(location.X, location.Y, 0),
	//	Vector3::Zero,
	//	Vector2(WindowWidth, WindowHeight),
	//	NULL,
	//	RectF(0, 0, WindowWidth, WindowHeight),
	//	ColorF(1.0f, 1.0f, 1.0f, 0.5f));

	//m_spriteRenderer->Flash();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ProfilerRenderer::DrawGroupList(Painter& painter, const RectF& listRect)
{
	int groupCount = m_profiler->GetCommitedGroups().GetCount();

	// 名前カラム背景色
	painter.SetBrush(ColorBrush::Blue);
	painter.SetOpacity(0.2f);
	painter.DrawRectangle(RectF(listRect.GetLocation(), ThreadNameColumnWidth, groupCount * rowHeight));

	// バーのNG領域背景色
	RectF ngArea(
		listRect.X + (ThreadNameColumnWidth + SafeAreaWidth),
		listRect.Y,
		listRect.Width - (ThreadNameColumnWidth + SafeAreaWidth),
		groupCount * rowHeight);
	painter.SetBrush(ColorBrush::Red);
	painter.DrawRectangle(ngArea);

	// リスト縦罫線
	painter.SetBrush(ColorBrush::Black);
	painter.SetOpacity(0.7f);
	painter.DrawRectangle(RectF(listRect.X + ThreadNameColumnWidth - 1, listRect.Y, 1, groupCount * rowHeight));

	int iGrout;
	const auto& groups = m_profiler->GetCommitedGroups();
	for (iGrout = 0; iGrout < groupCount; iGrout++)
	{
		// row 上部罫線
		painter.SetBrush(ColorBrush::Black);
		painter.SetOpacity(0.7f);
		painter.DrawRectangle(listRect.X, listRect.Y + (iGrout * rowHeight), listRect.Width, 1.0f);

		// グループ名
		painter.SetBrush(ColorBrush::White);
		painter.SetOpacity(1.0f);
		PointF pt(listRect.X + 4, listRect.Y + (iGrout * rowHeight) + 1);
		painter.DrawString(groups[iGrout].Name, pt);

		// ms
		TCHAR fps[256] = { 0 };
		StringTraits::SPrintf(fps, 256, _T("%.1f ms"), (groups[iGrout].TotalTime / 1000000.0));	// us → ms の後
		pt.Y += 16;
		painter.DrawString(fps, -1, pt);

		// セクションを積み上げ棒グラフで表示
		DrawSectionGraphBar(painter, groups[iGrout], listRect.X + ThreadNameColumnWidth, listRect.Y + (iGrout * rowHeight), listRect);
	}

	// row 下部罫線
	painter.SetBrush(ColorBrush::Black);
	painter.SetOpacity(0.7f);
	painter.DrawRectangle(listRect.X, listRect.Y + (groupCount * rowHeight), listRect.Width, 1.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ProfilerRenderer::DrawSectionGraphBar(Painter& painter, const Profiler::CommitedGroup& group, float x, float y, const RectF& listRect)
{
	float nextX = 0;
	int index = 0;
	for (const auto section : group.Sections)
	{
		if (group.LimitElapsedTime > 0.0)
		{
			float ratio = ((float)section.ElapsedTime) / group.LimitElapsedTime;	//BaseFPS

			RectF srcRect(x + nextX, y + 5, ratio * SafeAreaWidth, rowHeight - 10);
			if (listRect.X + listRect.Width < srcRect.X + srcRect.Width)
			{
				srcRect.Width = listRect.X + listRect.Width - srcRect.X;
			}

			HSVColor hsv(70 * index, 192, 255, 160);
			painter.SetSolidColor(hsv.ToColorF());
			painter.DrawRectangle(srcRect);

			nextX += ratio * SafeAreaWidth;
			if (listRect.X + listRect.Width <= nextX) { break; }
		}
		++index;
	}
}


} // namespace Graphics
} // namespace Lumino
