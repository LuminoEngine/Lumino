
#include "../Internal.h"
#include <Lumino/Scene/Fog.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Fog
//==============================================================================

Ref<Fog> Fog::create()
{
	return newObject<Fog>();
}

Ref<Fog> Fog::create(const Color& color, float density)
{
	return newObject<Fog>(color, density);
}

Fog::Fog()
	: Object()
	, m_fogColor(Color(1, 1, 1, 1))
	, m_fogDensity(0.00025)
{
}

Fog::~Fog()
{
}

void Fog::initialize()
{
	Object::initialize();
}

void Fog::initialize(const Color& color, float density)
{
	Object::initialize();
	setColor(color);
	setDensity(density);
}

LN_NAMESPACE_END
