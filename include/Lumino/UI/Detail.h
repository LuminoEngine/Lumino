
#pragma once
#include "Common.h"
#include "../Graphics/Brush.h"

LN_NAMESPACE_BEGIN
namespace detail
{

struct BorderInfo
{
	RefPtr<Pen>		left;
	RefPtr<Pen>		right;
	RefPtr<Pen>		top;
	RefPtr<Pen>		bottom;
};

} // namespace detail
LN_NAMESPACE_END
