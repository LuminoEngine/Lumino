
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@details	
*/
class UIContext
	: public RefObject
{
public:
	static UIContext* getMainContext();

public:
	//UILayoutView* GetMainWindowView() { return m_mainWindowView; }
	UIStyleTable* getRootStyleTable() { return m_rootStyleTable; }

LN_INTERNAL_ACCESS:
	UIContext();
	virtual ~UIContext();
	void initialize(detail::UIManager* manager);

	void setFocusElement(UIElement* element);
	UIElement* setFocusElement() { return m_focusElement; }	// キーボードフォーカス


	void injectElapsedTime(float elapsedTime);
	//void AddElement(UIElement* element) { m_allElementList.Add(element); }
	//void RemoveElement(UIElement* element) { m_allElementList.Remove(element); }
	detail::UIManager* getManager() { return m_manager; }

private:

	detail::UIManager*	m_manager;
	UIStyleTable*		m_rootStyleTable;
	//UILayoutView*		m_mainWindowView;
	UIElement*			m_focusElement;
	//List<UIElement*>	m_allElementList;
};

LN_NAMESPACE_END
