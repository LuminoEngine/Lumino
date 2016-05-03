
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
	static UIContext* GetMainContext();

public:
	UILayoutView* GetMainWindowView() { return m_mainWindowView; }
	UIStyleTable* GetRootStyleTable() { return m_rootStyleTable; }

LN_INTERNAL_ACCESS:
	UIContext();
	virtual ~UIContext();
	void Initialize(detail::UIManager* manager);

	void SetFocusElement(UIElement* element);
	UIElement* SetFocusElement() { return m_focusElement; }
	void InjectElapsedTime(float elapsedTime);
	//void AddElement(UIElement* element) { m_allElementList.Add(element); }
	//void RemoveElement(UIElement* element) { m_allElementList.Remove(element); }
	detail::UIManager* GetManager() { return m_manager; }

private:

	detail::UIManager*	m_manager;
	UIStyleTable*		m_rootStyleTable;
	UILayoutView*		m_mainWindowView;
	UIElement*			m_focusElement;
	//Array<UIElement*>	m_allElementList;
};

LN_NAMESPACE_END
