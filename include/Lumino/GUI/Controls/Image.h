
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class Image
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Image);
public:


public:
	Image(GUIManager* manager);
	virtual ~Image();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	// ImageSource Source
	// Stretch Stretch

	/** @} */

	//-------------------------------------------------------------------------
	/** @name Events */
	/** @{ */
	

	/** @} */

protected:
	virtual SizeF MeasureOverride(const SizeF& availableSize);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void OnRender();

private:

};


} // namespace GUI
} // namespace Lumino
