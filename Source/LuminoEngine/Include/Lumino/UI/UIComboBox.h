
#pragma once
#include "UIItemsControl.h"
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN
class UIScrollViewer;
class UIComboBoxItem;
class UIComboBox;
using UIComboBoxItemPtr = RefPtr<UIComboBoxItem>;
using UIComboBoxPtr = RefPtr<UIComboBox>;

/**
	@brief
*/
class UIPopup
	: public UIElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	void SetContent(UIElement* element);

	void open();

protected:
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIPopup();
	virtual ~UIPopup();
	void initialize();

LN_INTERNAL_ACCESS:
	void UpdateLayoutForInPlacePopup(const Size& viewSize);

private:
	RefPtr<UIElement>	m_content;
};


/**
	@brief
*/
class UIComboBoxItem
	: public UIContentControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

LN_CONSTRUCT_ACCESS :
	UIComboBoxItem();
	virtual ~UIComboBoxItem();
	void initialize();
};

/**
	@brief
*/
class UIComboBox
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static UIComboBoxPtr create();

public:
	UIComboBoxItemPtr AddTextItem(const String& text);
	UIComboBoxItemPtr AddItem(UIElement* item);

protected:
	virtual void OnMouseDown(UIMouseEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UIComboBox();
	virtual ~UIComboBox();
	void initialize();

private:
	RefPtr<UIScrollViewer>	m_scrollViewer;
	RefPtr<UIPopup>			m_popup;
};

LN_NAMESPACE_END
