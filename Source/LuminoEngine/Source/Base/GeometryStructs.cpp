﻿
#include "../Internal.h"
#include <Lumino/Base/GeometryStructs.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Point
//==============================================================================
const Point	Point::Zero(0, 0);

//==============================================================================
// Size
//==============================================================================
const Size Size::Zero(0, 0);
const Size Size::MinValue(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
const Size Size::MaxValue(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

//==============================================================================
// PointI
//==============================================================================
const PointI	PointI::Zero(0, 0);

//==============================================================================
// SizeI
//==============================================================================
const SizeI SizeI::Zero(0, 0);

SizeI SizeI::fromFloatSize(const Size& size)
{
	return SizeI(static_cast<int>(size.width), static_cast<int>(size.height));
}

//==============================================================================
// RectI
//==============================================================================
const RectI	RectI::Zero(0, 0, 0, 0);
//const RectI	RectI::Empty(0, 0, -1, -1);


//==============================================================================
// Rect
//==============================================================================
const Rect	Rect::Zero(0, 0, 0, 0);
const Rect	Rect::Empty(0, 0, -1, -1);

//==============================================================================
// Box32
//==============================================================================
const Box32 Box32::Zero(0, 0, 0, 0, 0, 0);

//==============================================================================
// ThicknessF
//==============================================================================
const ThicknessF	ThicknessF::Zero(0, 0, 0, 0);

//==============================================================================
// CornerRadius
//==============================================================================

//------------------------------------------------------------------------------
CornerRadius::CornerRadius()
	: topLeft(0)
	, topRight(0)
	, bottomRight(0)
	, bottomLeft(0)
{}

//------------------------------------------------------------------------------
CornerRadius::CornerRadius(float uniformRadius)
	: topLeft(uniformRadius)
	, topRight(uniformRadius)
	, bottomRight(uniformRadius)
	, bottomLeft(uniformRadius)
{}

//------------------------------------------------------------------------------
CornerRadius::CornerRadius(float topLeft_, float topRight_, float bottomRight_, float bottomLeft_)
	: topLeft(topLeft_)
	, topRight(topRight_)
	, bottomRight(bottomRight_)
	, bottomLeft(bottomLeft_)
{}

LN_NAMESPACE_END
