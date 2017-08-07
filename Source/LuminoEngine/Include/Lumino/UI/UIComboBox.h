
#pragma once
#include "UIItemsControl.h"
#include "UIContentsControl.h"

LN_NAMESPACE_BEGIN
class UIScrollViewer;
class UIComboBoxItem;
class UIComboBox;
using UIComboBoxItemPtr = Ref<UIComboBoxItem>;
using UIComboBoxPtr = Ref<UIComboBox>;


/**
	@brief
*/
class UIAdorner
	: public UIElement
{
	LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
	UIAdorner();
	virtual ~UIAdorner();
	void initialize();
};

/**
	@brief
*/
class UIAdornerLayer
	: public UIElement
{
	LN_OBJECT;
public:
	void add(UIAdorner* adorner);
	void remove(UIAdorner* adorner);

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIAdornerLayer();
	virtual ~UIAdornerLayer();
	void initialize();

private:
	List<Ref<UIAdorner>>	m_adorners;
};

/**
	@brief
*/
class UIPopup
	//: public UIElement
	: public UIAdorner
{
	LN_OBJECT;
public:
	void setContent(UIElement* element);

	void open(UIElement* owner);
	void close();

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onGotFocus(UIEventArgs* e) override;
	virtual void onLostFocus(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UIPopup();
	virtual ~UIPopup();
	void initialize();

LN_INTERNAL_ACCESS:
	void updateLayoutForInPlacePopup(const Size& viewSize);

private:
	Ref<UIElement>	m_content;
	UILayoutView*	m_layoutView;
};


/**
	@brief
*/
class UIComboBoxItem
	: public UIControl
{
	LN_OBJECT;
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
	LN_OBJECT;
public:
	static UIComboBoxPtr create();

public:
	UIComboBoxItemPtr addTextItem(const String& text);
	UIComboBoxItemPtr addItem(UIElement* item);

protected:
	virtual void onMouseDown(UIMouseEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
	UIComboBox();
	virtual ~UIComboBox();
	void initialize();

private:
	Ref<UIScrollViewer>	m_scrollViewer;
	Ref<UIPopup>			m_popup;
};

LN_NAMESPACE_END
