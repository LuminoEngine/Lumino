
#pragma once
#include "UIItemsControl.h"

LN_NAMESPACE_BEGIN
class UIPopup;

/**
	@brief
*/
class UIMenuItem
	: public UIHeaderedItemsControl
{
	LN_OBJECT;
public:

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UIMenuItem();
	virtual ~UIMenuItem();
	void initialize();

private:
};

/**
	@brief
*/
class UIMenuBase
	: public UIItemsControl
{
	LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
	UIMenuBase();
	virtual ~UIMenuBase();
	void initialize();

private:
};

/**
	@brief
*/
class UIContextMenu
	: public UIMenuBase
{
	LN_OBJECT;
public:

LN_CONSTRUCT_ACCESS:
	UIContextMenu();
	virtual ~UIContextMenu();
	void initialize();

private:
	Ref<UIPopup>	m_popup;
};

LN_NAMESPACE_END
