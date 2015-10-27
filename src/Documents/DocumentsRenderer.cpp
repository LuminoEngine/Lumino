
#include "../Internal.h"
#include <Lumino/Documents/DocumentsRenderer.h>

LN_NAMESPACE_BEGIN
namespace Documents
{

//=============================================================================
// RenderTargetDocumentsRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderTargetDocumentsRenderer::RenderTargetDocumentsRenderer(RenderingContext* painter)
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
void RenderTargetDocumentsRenderer::OnDrawGlyphRun(GlyphRun* glyphRun, const Point& point)
{
	m_painter->DrawGlyphRun(point, glyphRun);	// TODO: 位置
}

} // namespace Documents
LN_NAMESPACE_END

