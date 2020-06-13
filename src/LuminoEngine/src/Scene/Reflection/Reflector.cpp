
#include "../Internal.hpp"
#include <LuminoEngine/Scene/Reflection/ReflectorComponent.hpp>
#include <LuminoEngine/Scene/Reflection/Reflector.hpp>

namespace ln {

//==============================================================================
// Reflector

LN_OBJECT_IMPLEMENT(Reflector, VisualObject) {}

Ref<Reflector> Reflector::create()
{
	return makeObject<Reflector>();
}

Reflector::Reflector()
	: m_component(nullptr)
{
}

bool Reflector::init()
{
	if (!VisualObject::init()) return false;
    m_component = makeObject<ReflectorComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
	return true;
}

} // namespace ln

