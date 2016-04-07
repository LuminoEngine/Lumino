
#pragma once
#include <Lumino/Profiler.h>
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include <Lumino/Graphics/SpriteRenderer.h>

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
	ProfilerRenderer(GraphicsManager* manager, Profiler* profiler);
	virtual ~ProfilerRenderer();

public:
	void Render(const Vector2& viewSize);

private:
#if 0
	void DrawGroupList(RenderingContext& painter, const RectF& listRect);
	void DrawSectionGraphBar(RenderingContext& painter, const Profiler::CommitedGroup& group, float x, float y, const RectF& listRect);
#endif

private:
	GraphicsManager*		m_manager;
	Profiler*				m_profiler;
	RefPtr<Font>			m_font;
	RectF					m_windowRect;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
