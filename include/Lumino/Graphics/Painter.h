
#pragma once

namespace Lumino
{
namespace Graphics
{

/**
	@brief		
*/
class Painter
	: public RefObject
{
public:
	Painter();
	virtual ~Painter();

public:
	void Create(GraphicsManager* manager);
	void Begin();
	void End();

	void SetBrush();
	void DrawFrameRectangle(const RectF& rect, float frameWidth);

private:
};

} // namespace Graphics
} // namespace Lumino
