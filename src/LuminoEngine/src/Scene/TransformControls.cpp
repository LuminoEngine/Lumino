
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Scene/TransformControls.hpp>

namespace ln {

//==============================================================================
// Sprite

TransformControls::TransformControls()
{
}

void TransformControls::init()
{
	WorldObject::init();
}

void TransformControls::onRender(RenderingContext* context)
{
	context->drawLine(Vector3(0, 0, 0), Color::Red, Vector3(1, 0, 0), Color::Red);
	context->drawLine(Vector3(0, 0, 0), Color::Green, Vector3(0, 1, 0), Color::Green);
	context->drawLine(Vector3(0, 0, 0), Color::Blue, Vector3(0, 0, -1), Color::Blue);
}

} // namespace ln

