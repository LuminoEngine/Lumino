
#pragma once
#include <Lumino/Profiler.h>
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/Text/GlyphRun.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief		
*/
class ProfilerRenderer
	: public RefObject
{
	static const int ThreadNameColumnWidth = 80;
	static const int SafeAreaWidth = 160;
	static const int rowHeight = 32;

public:
	ProfilerRenderer(detail::GraphicsManager* manager, Profiler* profiler);
	virtual ~ProfilerRenderer();

public:
	void render(const Vector2& viewSize);

private:
#if 0
	void DrawGroupList(GraphicsContext& painter, const RectF& listRect);
	void DrawSectionGraphBar(GraphicsContext& painter, const Profiler::CommitedGroup& group, float x, float y, const RectF& listRect);
#endif

private:
	detail::GraphicsManager*	m_manager;
	Profiler*					m_profiler;
	RefPtr<RawFont>				m_font;
	Rect						m_windowRect;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
