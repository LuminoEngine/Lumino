
#pragma once
#include <Lumino/Profiler.h>
#include <Lumino/Imaging/Font.h>
#include <Lumino/Graphics/GlyphRun.h>
#include <Lumino/Graphics/SpriteRenderer.h>

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
public:
	ProfilerRenderer(GraphicsManager* manager, Profiler* profiler);
	virtual ~ProfilerRenderer();

public:
	void Render(const Vector2& viewSize);

private:
	GraphicsManager*		m_manager;
	Profiler*				m_profiler;
	RefPtr<Imaging::Font>	m_font;
	RefPtr<SpriteRenderer>	m_spriteRenderer;
};

} // namespace Graphics
} // namespace Lumino
