
#pragma once
#include "ContentControl.h"
#include "ButtonChrome.h"

namespace Lumino
{
namespace Animation { class FloatAnimationCurve; }

namespace GUI
{
	
/**
	@brief
*/
class TextArea
	: public Control
{
};

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

	/** 現在の選択箇所にクリップボードの内容を貼り付けます。*/
	void Paste();

protected:
	TextBox(GUIManager* manager);
	virtual ~TextBox();
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual void OnRender(Graphics::Painter* painter);

private:
	void set_Text(const String& string);
	const String& get_Text() const;
	void Handler_KeyDown(KeyEventArgs* e);
	void Handler_TextInput(KeyEventArgs* e);
	void Handler_MouseDown(MouseEventArgs* e);
	void Handler_MouseUp(MouseEventArgs* e);
	void Handler_MouseMove(MouseEventArgs* e);

private:
	class Document;
	class LineSegment;
	class Selection;
	Document*	m_document;
	Selection*	m_selection;
	Animation::FloatAnimationCurve*	m_caretAnimation;

	int		m_dragStartContentIndex;
	bool	m_isDragging;

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
