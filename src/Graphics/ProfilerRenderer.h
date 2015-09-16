
#pragma once
#include <Lumino/Profiler.h>
#include <Lumino/Graphics/Font.h>
#include <Lumino/Graphics/GlyphRun.h>
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Graphics/Painter.h>

namespace Lumino
{
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
	void DrawGroupList(Painter& painter, const RectF& listRect);
	void DrawSectionGraphBar(Painter& painter, const Profiler::CommitedGroup& group, float x, float y, const RectF& listRect);

private:
	GraphicsManager*		m_manager;
	Profiler*				m_profiler;
	RefPtr<Font>			m_font;
	RectF					m_windowRect;
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
