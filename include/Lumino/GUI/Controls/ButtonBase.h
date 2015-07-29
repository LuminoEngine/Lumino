
#pragma once
#include "../ContentControl.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class ButtonBase
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ButtonBase);
public:
	ButtonBase(GUIManager* manager);
	virtual ~ButtonBase();
};


} // namespace GUI
} // namespace Lumino
