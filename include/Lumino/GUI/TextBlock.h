﻿
#pragma once
#include "../Documents/Paragraph.h"
#include "UITextElement.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		
*/
class TextBlock
	: public UITextElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(TextBlock);
public:
	LN_PROPERTY(String,		TextProperty);				/**< Text プロパティの識別子 */


public:

	/**
		@brief	TextBlock を作成します。
		@return	作成された TextBlock。このオブジェクトは AutoReleasePool で管理されています。
	*/
	static TextBlock* Create(/*GUIManagerImpl* manager*/);

	TextBlock(GUIManagerImpl* manager);
	virtual ~TextBlock();

public:
	void SetText(const String& text) { SetTypedPropertyValue<String>(TextProperty, text); }
	String GetText() const { return GetTypedPropertyValue<String>(TextProperty); }

protected:
	virtual SizeF MeasureOverride(const SizeF& availableSize);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void OnRender(RenderingContext* painter);

private:
	void OnTextPropertyChanged(PropertyChangedEventArgs* e);

private:
	String	m_text;
	RefPtr<Documents::Paragraph>	m_paragraph;
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
