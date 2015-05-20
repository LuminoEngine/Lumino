
#include "../Internal.h"
#include "../../include/Lumino/Graphics/SamplerState.h"

namespace Lumino
{
namespace Graphics
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SamplerState::SamplerState()
	: FilterMode(TextureFilterMode_Point)
	, WrapMode(TextureWrapMode_Repeat)
{}

} // namespace Graphics
} // namespace Lumino
