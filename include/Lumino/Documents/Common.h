
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

class IDocumentsRenderer
{
public:
	virtual ~IDocumentsRenderer() = default;
	virtual void OnDrawGlyphRun(const Matrix& transform, GlyphRun* glyphRun, const Point& point) = 0;
};

} // namespace detail
LN_NAMESPACE_END

