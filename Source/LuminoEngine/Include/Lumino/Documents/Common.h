
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

class IDocumentsRenderer
{
public:
	virtual ~IDocumentsRenderer() = default;
	virtual void onDrawGlyphRun(const Matrix& transform, Brush* forground, GlyphRun* glyphRun, const Point& point) = 0;
};

} // namespace detail
LN_NAMESPACE_END

