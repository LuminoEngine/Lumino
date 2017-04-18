
#include "../Internal.h"
#include <Lumino/Base/GeometryStructs.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// PointI
//==============================================================================
const PointI	PointI::Zero(0, 0);

//==============================================================================
// PointF
//==============================================================================
const GenericPoint<float>	GenericPoint<float>::Zero(0, 0);

//==============================================================================
// Size
//==============================================================================

const Size Size::Zero(0, 0);
const Size Size::MinValue(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
const Size Size::MaxValue(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

const SizeI SizeI::Zero(0, 0);

//==============================================================================
// RectI
//==============================================================================
const RectI	RectI::Zero(0, 0, 0, 0);
//const RectI	RectI::Empty(0, 0, -1, -1);


//==============================================================================
// RectF
//==============================================================================
const RectF	RectF::Zero(0, 0, 0, 0);
const RectF	RectF::Empty(0, 0, -1, -1);

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
