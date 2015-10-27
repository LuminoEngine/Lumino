
#pragma once
#include "Panel.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		
*/
class StackPanel
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(StackPanel);
public:
	LN_PROPERTY(Orientation, OrientationProperty);

public:
	static StackPanel* Create();

	StackPanel(GUIManagerImpl* manager);
	virtual ~StackPanel();

public:
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }
	Orientation GetOrientation() const { return m_orientation; }

protected:
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

private:
	Orientation	m_orientation;
};


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
