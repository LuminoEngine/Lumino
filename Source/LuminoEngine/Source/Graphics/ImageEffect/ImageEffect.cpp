
#include "../Internal.h"
#include <Lumino/Graphics/ImageEffect/ImageEffect.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// ImageEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ImageEffect, AnimatableObject);

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
void ImageEffect::Initialize(detail::GraphicsManager* manager)
{
	if (LN_CHECK_ARG(manager != nullptr)) return;
	m_manager = manager;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
