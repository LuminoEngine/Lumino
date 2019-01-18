#pragma once
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
namespace detail {

class ILayoutElement
{
protected:
    ILayoutElement();

	//virtual int getChildLayoutItemCount() const = 0;
	//virtual ILayoutElement* getChildLayoutItem(int index) const = 0;

private:
};

class ILayoutPanel
{
protected:
    ILayoutPanel();

private:
};

} // namespace detail
} // namespace ln

