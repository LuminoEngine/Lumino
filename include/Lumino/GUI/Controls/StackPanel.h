
#pragma once
#include "Panel.h"

namespace Lumino
{
namespace GUI
{



/**
	@brief		
*/
class StackPanel
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(StackPanel);
public:
	LN_PROPERTY_BEGIN;
	LN_PROPERTY(GUI::Orientation, Orientation);
	LN_PROPERTY_END;

public:
	static StackPanel* Create(GUIManager* manager);

	StackPanel(GUIManager* manager);
	virtual ~StackPanel();

public:
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }
	Orientation GetOrientation() const { return m_orientation; }

protected:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

private:
	Orientation	m_orientation;
};


} // namespace GUI
} // namespace Lumino
