
#include "../Internal.h"
#include "../../include/Lumino/Graphics/SamplerState.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SamplerState::SamplerState()
	: FilterMode(TextureFilterMode_Point)
	, WrapMode(TextureWrapMode_Repeat)
{}

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
