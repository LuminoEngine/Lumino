
#pragma once
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class TextBlock
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(TextBlock);
public:
	static const Property*	TextProperty;

public:
	TextBlock(GUIManager* manager);
	virtual ~TextBlock();

public:
	void SetText(const String& text) { m_text = text; }

protected:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

private:
	String	m_text;
};

} // namespace GUI
} // namespace Lumino
