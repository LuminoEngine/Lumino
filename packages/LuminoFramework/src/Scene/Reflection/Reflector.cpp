#include "../Internal.hpp"
#include <LuminoFramework/Scene/Reflection/ReflectorComponent.hpp>
#include <LuminoFramework/Scene/Reflection/Reflector.hpp>

namespace ln {

//==============================================================================
// Reflector

LN_OBJECT_IMPLEMENT(Reflector, VisualObject) {}

Ref<Reflector> Reflector::create()
{
	return makeObject_deprecated<Reflector>();
}

Reflector::Reflector()
	: m_component(nullptr)
{
}

bool Reflector::init()
{
	if (!VisualObject::init()) return false;
    m_component = makeObject_deprecated<ReflectorComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
	return true;
}

} // namespace ln

