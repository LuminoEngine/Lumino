
#pragma once
#include "ButtonBase.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class RepeatButton
	: public ButtonBase
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RepeatButton);
public:
	RepeatButton(GUIManager* manager);
	virtual ~RepeatButton();
};


} // namespace GUI
} // namespace Lumino
