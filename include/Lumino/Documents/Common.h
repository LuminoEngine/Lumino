
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

class IDocumentsRenderer
{
public:
	virtual ~IDocumentsRenderer() = default;
	virtual void OnDrawGlyphRun(const Matrix& transform, Brush* forground, GlyphRun* glyphRun, const PointF& point) = 0;
};

} // namespace detail
LN_NAMESPACE_END

