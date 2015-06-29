
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"
#include "Panel.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class ScrollBar
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollBar);
public:
	ScrollBar(GUIManager* manager);
	virtual ~ScrollBar();
};


} // namespace GUI
} // namespace Lumino
