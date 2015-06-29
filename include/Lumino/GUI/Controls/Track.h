
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class Track
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Track);
public:
	Track(GUIManager* manager);
	virtual ~Track();
};


} // namespace GUI
} // namespace Lumino
