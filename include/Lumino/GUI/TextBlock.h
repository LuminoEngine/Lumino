
#pragma once
#include "../Documents/Paragraph.h"
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
	static TextBlock* Create(GUIManager* manager);

	TextBlock(GUIManager* manager);
	virtual ~TextBlock();

public:
	void SetText(const String& text);

protected:
	virtual SizeF MeasureOverride(const SizeF& availableSize);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void OnRender(Graphics::Painter* painter);

private:
	//String	m_text;
	RefPtr<Documents::Paragraph>	m_paragraph;
};

} // namespace GUI
} // namespace Lumino
