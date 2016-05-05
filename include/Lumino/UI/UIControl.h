
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@note		コントロールは VisualTree のルートを持つことができる。
*/
class UIControl
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static const String MouseOverStateName;		/**< "MouseOver" */

public:

protected:
	UIControl();
	virtual ~UIControl();
	void Initialize(detail::UIManager* manager);

	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChildOrderd(int index) const override;
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;

LN_INTERNAL_ACCESS:
	void SetVisualTreeRoot(UIElement* element);
	UIElement* GetVisualTreeRoot() { return m_visualTreeRoot; }

private:
	void EventHandler_MouseEnter(UIMouseEventArgs* e);
	void EventHandler_MouseLeave(UIMouseEventArgs* e);

	UIElement*	m_visualTreeRoot;
};

LN_NAMESPACE_END
