
#pragma once
#include "../Graphics/Font.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Painter.h"

namespace Lumino
{
namespace Documents
{

/**
	@brief
*/
class DocumentsRenderer
	: public RefObject
{
protected:
	DocumentsRenderer() {}
	virtual ~DocumentsRenderer() {}

public:
	virtual void OnDrawGlyphRun(GlyphRun* glyphRun, const Point& point) = 0;
};

class RenderTargetDocumentsRenderer
	: public DocumentsRenderer
{
public:
	RenderTargetDocumentsRenderer(RenderingContext* painter);
	virtual ~RenderTargetDocumentsRenderer();

	virtual void OnDrawGlyphRun(GlyphRun* glyphRun, const Point& point);

private:
	RenderingContext*	m_painter;
};

} // namespace Documents
} // namespace Lumino
