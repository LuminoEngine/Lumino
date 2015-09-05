
#pragma once
#include "ContentControl.h"
#include "ButtonChrome.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class TextBox
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(TextBox);
public:
	LN_PROPERTY(String, TextProperty);

public:

	/**
		@brief	TextBox を作成します。
	*/
	static TextBox* Create(GUIManager* manager = NULL);

public:

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/** */
	void SetText(const String& value) { SetTypedPropertyValue<String>(TextProperty, value); }

	/** */
	String GetText() const { return GetTypedPropertyValue<String>(TextProperty); }

	/** @} */

	void Select(int start, int length);

protected:
	TextBox(GUIManager* manager);
	virtual ~TextBox();
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual void OnRender(Graphics::Painter* painter);

private:
	void set_Text(const String& string);
	const String& get_Text() const;

private:
	class Document;
	class LineSegment;
	class Selection;
	Document*	m_document;
	Selection*	m_selection;

	//RefPtr<Documents::Document>	m_document;
	//Array<TextBoxLineSegment*>		m_lineSegments;

	/*
	CanRedo
	CanUndo
	CaretBrush
	IsReadOnly
	SelectionBrush
	UndoLimit

	IScrollInfo

	BeginChange
	EndChange
	Copy
	Cut
	Paste
	Undo
	Redo
	SelectAll
	LineDown
	LineLeft
	LineRight
	LineUp
	PageDown
	ScrollTo...
	Select	// サロゲートペアも考慮した「文字数」
	*/
};

} // namespace GUI
} // namespace Lumino
