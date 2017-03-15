
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN
class UIElementCollection;
class UILayoutPanel;

/**
	@brief		
*/
class UIControl
	: public UIElement
	, public tr::IUIElementCollectionOwner
{
	LN_UI_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(HAlignment,	HContentAlignment);		/**< HContentAlignment プロパティの識別子 */
	LN_TR_PROPERTY(VAlignment,	VContentAlignment);		/**< VContentAlignment プロパティの識別子 */
	tr::Property<HAlignment>	HContentAlignment;
	tr::Property<VAlignment>	VContentAlignment;

	static const String NormalState;
	static const String MouseOverState;
	static const String PressedState;
	static const String DisabledState;
	static const String UnfocusedState;
	static const String FocusedState;
	static const String ValidState;
	static const String InvalidState;

public:
	
	void SetHContentAlignment(HAlignment value) { tr::PropertyInfo::SetPropertyValueDirect<HAlignment>(this, HContentAlignmentId, value); }
	HAlignment GetHContentAlignment() const { return tr::PropertyInfo::GetPropertyValueDirect<HAlignment>(this, HContentAlignmentId); }

	void SetVContentAlignment(VAlignment value) { tr::PropertyInfo::SetPropertyValueDirect<VAlignment>(this, VContentAlignmentId, value); }
	VAlignment GetVContentAlignment() const { return tr::PropertyInfo::GetPropertyValueDirect<VAlignment>(this, VContentAlignmentId); }
	
	virtual bool IsFocusable() const;



	UIElementCollection* GetItems() const;

	void AddChild(UIElement* element);
	void RemoveChild(UIElement* element);

	void SetLayoutPanel(UILayoutPanel* panel);
	UILayoutPanel* GetLayoutPanel() const;

protected:
	UIControl();
	virtual ~UIControl();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	//virtual int GetVisualChildrenCount() const override;
	//virtual ILayoutElement* GetVisualChild(int index) const override;
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;
	virtual const VAlignment* GetPriorityContentVAlignment() override;
	virtual const HAlignment* GetPriorityContentHAlignment() override;
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
	virtual void OnGotFocus(UIEventArgs* e) override;
	virtual void OnLostFocus(UIEventArgs* e) override;


	virtual void OnLayoutPanelChanged(UILayoutPanel* newPanel);

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;


LN_INTERNAL_ACCESS:
	int GetLayoutChildrenCount() const { return GetVisualChildrenCount(); }
	ILayoutElement* GetLayoutChild(int index) const { return GetVisualChild(index); }

	//void SetVisualTreeRoot(UIElement* element);
	//UIElement* GetVisualTreeRoot() { return m_visualTreeRoot; }

private:
	RefPtr<UIElementCollection>		m_items;
	RefPtr<UILayoutPanel>			m_itemsHostPanel;

	//UIElement*	m_visualTreeRoot;
};

LN_NAMESPACE_END
