
#pragma once
#include "UITextElement.h"

LN_NAMESPACE_BEGIN
namespace tr { class Document; }
namespace tr { class DocumentView; }
class UITextBox;
using UITextBoxPtr = RefPtr<UITextBox>;
class UITextArea;

/**
	@brief		
*/
class UITextBox
	: public UITextElement
{
	LN_UI_TYPEINFO_DECLARE();
public:

	static UITextBoxPtr Create();

public:
	void SetText(const StringRef& text);

	virtual bool IsFocusable() const;

protected:
	virtual void OnMouseDown(UIMouseEventArgs* e) override;
	virtual void OnTextInput(UIKeyEventArgs* e) override;

LN_PROTECTED_INTERNAL_ACCESS:
	UITextBox();
	virtual ~UITextBox();
	void Initialize(detail::UIManager* manager);

	virtual Size MeasureOverride(const Size& availableSize) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual void OnRender(DrawList* g) override;

private:
	//RefPtr<tr::Document>		m_document;
	//RefPtr<tr::DocumentView>	m_documentView;
	RefPtr<UITextArea>			m_textArea;
};

LN_NAMESPACE_END
