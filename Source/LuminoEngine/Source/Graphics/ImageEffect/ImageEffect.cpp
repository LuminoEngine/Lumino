
#include "../Internal.h"
#include <Lumino/Graphics/ImageEffect/ImageEffect.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// ImageEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ImageEffect, Object);

//------------------------------------------------------------------------------
ImageEffect::ImageEffect()
	: m_manager(nullptr)
{
}

//------------------------------------------------------------------------------
ImageEffect::~ImageEffect()
{
}

//------------------------------------------------------------------------------
bool ImageEffect::initialize(detail::GraphicsManager* manager)
{
	LN_BASE_INITIALIZE(Object);
	if (LN_CHECK_ARG(manager != nullptr)) return false;
	m_manager = manager;
	return true;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
