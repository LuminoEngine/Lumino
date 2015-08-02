
#include "../Internal.h"
#include <Lumino/Documents/DocumentsRenderer.h>

namespace Lumino
{
namespace Documents
{

//=============================================================================
// RenderTargetDocumentsRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderTargetDocumentsRenderer::RenderTargetDocumentsRenderer(Graphics::Painter* painter)
	: m_painter(painter)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderTargetDocumentsRenderer::~RenderTargetDocumentsRenderer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderTargetDocumentsRenderer::OnDrawGlyphRun(Graphics::GlyphRun* glyphRun, const Point& point)
{
	m_painter->DrawGlyphRun(point, glyphRun);	// TODO: ˆÊ’u
}

} // namespace Documents
} // namespace Lumino

