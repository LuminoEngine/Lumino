
#pragma once
#include <functional>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/Common.h>

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
namespace Internal
{

class GUIPainter
	: public RenderingContext
{
public:
	GUIPainter(GraphicsManager* manager);
	virtual ~GUIPainter();

public:
	//void SetRenderTarget(Graphics::Texture* renderTarget);

private:

};

} // namespace Internal
LN_NAMESPACE_GUI_END
} // namespace Lumino
