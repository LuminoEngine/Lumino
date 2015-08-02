
#pragma once
#include <Lumino/Profiler.h>
#include <Lumino/Imaging/Font.h>
#include <Lumino/Graphics/GlyphRun.h>
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Graphics/Painter.h>

namespace Lumino
{
namespace Graphics
{

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
	RefPtr<Imaging::Font>	m_font;
	RefPtr<SpriteRenderer>	m_spriteRenderer;	// TODO: ‚¢‚ç‚È‚¢

	RectF					m_windowRect;
};

} // namespace Graphics
} // namespace Lumino
