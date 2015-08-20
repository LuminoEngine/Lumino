
#pragma once
#include <functional>
#include <Lumino/Graphics/Painter.h>

namespace Lumino
{
namespace GUI
{
namespace Internal
{

class GUIPainter
	: public Graphics::Painter
{
public:
	GUIPainter(Graphics::GraphicsManager* manager);
	virtual ~GUIPainter();

public:
	//void SetRenderTarget(Graphics::Texture* renderTarget);

private:

};

} // namespace Internal
} // namespace GUI
} // namespace Lumino
