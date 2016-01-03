
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
	UILayoutView* GetMainWindowView() { return m_mainWindowView; }

LN_INTERNAL_ACCESS:
	void SetFocusElement(UIElement* element);
	UIElement* SetFocusElement() { return m_focusElement; }
	void InjectElapsedTime(float elapsedTime);
	//void AddElement(UIElement* element) { m_allElementList.Add(element); }
	//void RemoveElement(UIElement* element) { m_allElementList.Remove(element); }
	void Render();
	detail::UIManager* GetManager() { return m_manager; }

private:
	friend class detail::UIManager;
	UIContext();
	virtual ~UIContext();
	void Initialize(detail::UIManager* manager);

	detail::UIManager*	m_manager;
	UILayoutView*		m_mainWindowView;
	UIElement*			m_focusElement;
	//Array<UIElement*>	m_allElementList;
};

LN_NAMESPACE_END
