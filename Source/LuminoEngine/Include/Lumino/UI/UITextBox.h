
#pragma once
#include "UITextElement.h"
#include "UIControl.h"

LN_NAMESPACE_BEGIN
namespace tr { class Document; }
namespace tr { class DocumentView; }
class UITextBox;
using UITextBoxPtr = Ref<UITextBox>;
class UITextArea;
class UISimpleTextArea;

/**
	@brief		
*/
class UITextBox
	: public UITextElement
{
	LN_OBJECT;
public:

	static UITextBoxPtr create();

public:
	void setText(const StringRef& text);

protected:
	virtual void onMouseDown(UIMouseEventArgs* e) override;
	virtual void onKeyDown(UIKeyEventArgs* e) override;
	virtual void onTextInput(UIKeyEventArgs* e) override;

LN_PROTECTED_INTERNAL_ACCESS:
	UITextBox();
	virtual ~UITextBox();
	void initialize();

	virtual Size measureOverride(const Size& availableSize) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRender(DrawingContext* g) override;

private:
	//Ref<tr::Document>		m_document;
	//Ref<tr::DocumentView>	m_documentView;
	Ref<UITextArea>			m_textArea;
};




/**
	@brief		
*/
class UITextField
	: public UIControl
{
	LN_OBJECT;
public:
	static Ref<UITextField> create();

public:
	void setText(const StringRef& text);


protected:

LN_PROTECTED_INTERNAL_ACCESS:
	UITextField();
	virtual ~UITextField();
	void initialize();

	virtual Size measureOverride(const Size& availableSize) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

private:
	Ref<UISimpleTextArea>			m_textArea;
};

LN_NAMESPACE_END
