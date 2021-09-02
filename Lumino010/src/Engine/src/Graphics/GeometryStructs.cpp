
#include "Internal.hpp"
#include <limits>
#include <algorithm>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Graphics/GeometryStructs.hpp>

namespace ln {

//==============================================================================
// Point

const Point Point::Zero(0, 0);

//==============================================================================
// Size

const Size Size::Zero(0, 0);
const Size Size::MinValue(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
const Size Size::MaxValue(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

void Size::serialize(Serializer2& ar)
{
    int size = 0;
    if (ar.isSaving())
        ar.beginWriteList();
    else {
        ar.beginReadList(&size);
        assert(size == 2);	// TODO: error handling
    }

    ar.process(width);
    ar.process(height);

    if (ar.isSaving())
        ar.endWriteList();
    else
        ar.endReadList();
}

//==============================================================================
// PointI

const PointI PointI::Zero(0, 0);

//==============================================================================
// SizeI

const SizeI SizeI::Zero(0, 0);

SizeI SizeI::fromFloatSize(const Size& size)
{
    return SizeI(static_cast<int>(size.width), static_cast<int>(size.height));
}

//==============================================================================
// RectI

const RectI RectI::Zero(0, 0, 0, 0);
const RectI RectI::Empty(0, 0, -1, -1);

//==============================================================================
// Rect

const Rect Rect::Zero(0, 0, 0, 0);
const Rect Rect::Empty(0, 0, -1, -1);

Rect Rect::makeInflate(const Thickness& thickness) const
{
	return Rect(
		x - thickness.left,
		y - thickness.top,
		std::max(width + thickness.width(), 0.0f),
		std::max(height + thickness.height(), 0.0f));
}

Rect Rect::makeDeflate(const Thickness& thickness) const
{
    return Rect(
        x + thickness.left,
        y + thickness.top,
        std::max(width - thickness.width(), 0.0f),
        std::max(height - thickness.height(), 0.0f));
}

void Rect::serialize(Archive& ar)
{
    int size = 0;
    ar.makeArrayTag(&size);
    if (ar.isLoading()) {
        assert(size == 4);	// TODO: error handling
    }
    ar.process(x);
    ar.process(y);
    ar.process(width);
    ar.process(height);
}

void Rect::serialize(Serializer2& ar)
{
    int size = 0;
    if (ar.isSaving())
        ar.beginWriteList();
    else {
        ar.beginReadList(&size);
        assert(size == 4);	// TODO: error handling
    }

    ar.process(x);
    ar.process(y);
    ar.process(width);
    ar.process(height);

    if (ar.isSaving())
        ar.endWriteList();
    else
        ar.endReadList();
}

//==============================================================================
// BoxI

const BoxI BoxI::Zero(0, 0, 0, 0, 0, 0);

//==============================================================================
// Thickness

const Thickness Thickness::Zero(0, 0, 0, 0);

//==============================================================================
// CornerRadius

const CornerRadius CornerRadius::Zero = CornerRadius(0, 0, 0, 0);

CornerRadius::CornerRadius()
    : topLeft(0)
    , topRight(0)
    , bottomRight(0)
    , bottomLeft(0)
{
}

CornerRadius::CornerRadius(float uniformRadius)
    : topLeft(uniformRadius)
    , topRight(uniformRadius)
    , bottomRight(uniformRadius)
    , bottomLeft(uniformRadius)
{
}

CornerRadius::CornerRadius(float topLeft_, float topRight_, float bottomRight_, float bottomLeft_)
    : topLeft(topLeft_)
    , topRight(topRight_)
    , bottomRight(bottomRight_)
    , bottomLeft(bottomLeft_)
{
}

} // namespace ln
