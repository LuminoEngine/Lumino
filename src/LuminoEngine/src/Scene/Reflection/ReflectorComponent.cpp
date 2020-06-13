
#include "../Internal.hpp"
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Scene/Reflection/ReflectorComponent.hpp>

namespace ln {

//=============================================================================
// ReflectorComponent

LN_OBJECT_IMPLEMENT(ReflectorComponent, VisualComponent) {}

ReflectorComponent::ReflectorComponent()
    : m_material(nullptr)
    , m_size(10.0f, 10.0f)
{
}

bool ReflectorComponent::init()
{
	if (!VisualComponent::init()) return false;
	return true;
}

void ReflectorComponent::onRender(RenderingContext* context)
{
    context->setMaterial(m_material);

    auto uv1 = Vector2(1.0, 1.0);

    context->drawPlane(m_size.x, m_size.y, Vector2::Zero, uv1, Color::White);
}

} // namespace ln

