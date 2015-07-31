
#include "../Internal.h"
#include <Lumino/Graphics/Painter.h>
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
	m_font.Attach(Imaging::Font::CreateBuiltInBitmapFont(m_manager->GetFontManager(), 7));
	m_spriteRenderer.Attach(SpriteRenderer::Create(256, m_manager));
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
	Painter painter(m_manager);
	painter.SetProjection(Size(viewSize.X, viewSize.Y), 0, 1000);

	painter.SetBrush(ColorBrush::Blue);
	painter.DrawRectangle2(RectF(10, 10, 30, 50));

	return;



	Matrix proj = Matrix::Perspective2DLH(viewSize.X, viewSize.Y, 0.f, 1000.f);
	m_spriteRenderer->SetViewProjMatrix(Matrix::Identity, proj);
	m_spriteRenderer->SetViewPixelSize(Size(viewSize.X, viewSize.Y));		// TODO Vector2 ‚Ì set ‚ª‚ ‚Á‚Ä‚à‚¢‚¢‚©‚à

	float WindowWidth = 256;
	float WindowHeight = 256;
	Vector3 location(viewSize.X - 8 - WindowWidth, 8, 0);

	// ƒEƒBƒ“ƒhƒE”wŒi
	m_spriteRenderer->DrawRequest2D(
		Vector3(location.X, location.Y, 0),
		Vector3::Zero,
		Vector2(WindowWidth, WindowHeight),
		NULL,
		RectF(0, 0, WindowWidth, WindowHeight),
		ColorF(1.0f, 1.0f, 1.0f, 0.5f));

	m_spriteRenderer->Flash();
}

} // namespace Graphics
} // namespace Lumino
