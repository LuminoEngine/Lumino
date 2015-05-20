
#include "../Internal.h"
#include <Lumino/Graphics/Color.h>

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// Color
//=============================================================================
const Color Color::Transparency = Color(0, 0, 0, 0);
const Color Color::TransparencyWhite = Color(255, 255, 255, 0);
const Color Color::Black = Color(0, 0, 0, 255);
const Color Color::White = Color(255, 255, 255, 255);
const Color Color::Gray = Color(127, 127, 127, 255);
const Color Color::Red = Color(255, 0, 0, 255);
const Color Color::Green = Color(0, 255, 0, 255);
const Color Color::Blue = Color(0, 0, 255, 255);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Color::Color(const ColorF& colorF)
{
	R = static_cast<uint8_t>(colorF.R * 255);
	G = static_cast<uint8_t>(colorF.G * 255);
	B = static_cast<uint8_t>(colorF.B * 255);
	A = static_cast<uint8_t>(colorF.A * 255);
}

//=============================================================================
// ColorF
//=============================================================================
const ColorF ColorF::Transparency = ColorF(0.0, 0.0, 0.0, 0.0);
const ColorF ColorF::TransparencyWhite = ColorF(1.0, 1.0, 1.0, 0.0);
const ColorF ColorF::Black = ColorF(0.0, 0.0, 0.0, 1.0);
const ColorF ColorF::White = ColorF(1.0, 1.0, 1.0, 1.0);
const ColorF ColorF::Gray = ColorF(0.5, 0.5, 0.5, 1.0);
const ColorF ColorF::Red = ColorF(1.0, 0.0, 0.0, 1.0);
const ColorF ColorF::Green = ColorF(0.0, 1.0, 0.0, 1.0);
const ColorF ColorF::Blue = ColorF(0.0, 0.0, 1.0, 1.0);


} // namespace Graphics
} // namespace Lumino
