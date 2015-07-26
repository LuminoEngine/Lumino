
#pragma once
#include "../Imaging/Font.h"
#include "../../src/Imaging/TextLayoutEngine.h"	// TODO
#include "../Graphics/GraphicsManager.h"

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
	DocumentsRenderer();
	virtual ~DocumentsRenderer();

	virtual void OnDrawGlyphRun(Imaging::GlyphRun* glyphRun, const Point& point) = 0;
};

class RenderTargetDocumentsRenderer
	: public DocumentsRenderer
{
public:
	RenderTargetDocumentsRenderer();
	virtual ~RenderTargetDocumentsRenderer();

	virtual void OnDrawGlyphRun(Imaging::GlyphRun* glyphRun, const Point& point);
};

} // namespace Documents
} // namespace Lumino
