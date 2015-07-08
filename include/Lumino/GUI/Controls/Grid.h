
#pragma once
#include "Panel.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class Grid
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Grid);
public:
	Grid(GUIManager* manager);
	virtual ~Grid();

protected:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
};


} // namespace GUI
} // namespace Lumino
