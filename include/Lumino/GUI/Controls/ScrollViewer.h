
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class ScrollViewer
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ScrollViewer);
public:
	ScrollViewer(GUIManager* manager);
	virtual ~ScrollViewer();
};


} // namespace GUI
} // namespace Lumino
