
#pragma once
#include "UITextElement.h"
#include "UIControl.h"

LN_NAMESPACE_BEGIN
namespace tr { class Document; }
namespace tr { class DocumentView; }
class UITextBox;
using UITextBoxPtr = RefPtr<UITextBox>;
class UITextArea;
class UISimpleTextArea;

/**
	@brief		
*/
class UITextBox
	: public UITextElement
{
	LN_OBJECT();
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
	//RefPtr<tr::Document>		m_document;
	//RefPtr<tr::DocumentView>	m_documentView;
	RefPtr<UITextArea>			m_textArea;
};




/**
	@brief		
*/
class UITextField
	: public UIControl
{
	LN_OBJECT();
public:
	static RefPtr<UITextField> create();

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
	RefPtr<UISimpleTextArea>			m_textArea;
};

LN_NAMESPACE_END
