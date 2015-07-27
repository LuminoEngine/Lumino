
#pragma once
#include "../Imaging/Font.h"
#include "../../src/Imaging/TextLayoutEngine.h"	// TODO
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
	virtual void OnDrawGlyphRun(Graphics::GlyphRun* glyphRun, const Point& point) = 0;
};

class RenderTargetDocumentsRenderer
	: public DocumentsRenderer
{
public:
	RenderTargetDocumentsRenderer(Graphics::Painter* painter);
	virtual ~RenderTargetDocumentsRenderer();

	virtual void OnDrawGlyphRun(Graphics::GlyphRun* glyphRun, const Point& point);

private:
	Graphics::Painter*	m_painter;
};

} // namespace Documents
} // namespace Lumino
