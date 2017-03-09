
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIControl
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(HAlignment,	HContentAlignment);		/**< HContentAlignment プロパティの識別子 */
	LN_TR_PROPERTY(VAlignment,	VContentAlignment);		/**< VContentAlignment プロパティの識別子 */
	tr::Property<HAlignment>	HContentAlignment;
	tr::Property<VAlignment>	VContentAlignment;

public:
	
	void SetHContentAlignment(HAlignment value) { tr::PropertyInfo::SetPropertyValueDirect<HAlignment>(this, HContentAlignmentId, value); }
	HAlignment GetHContentAlignment() const { return tr::PropertyInfo::GetPropertyValueDirect<HAlignment>(this, HContentAlignmentId); }

	void SetVContentAlignment(VAlignment value) { tr::PropertyInfo::SetPropertyValueDirect<VAlignment>(this, VContentAlignmentId, value); }
	VAlignment GetVContentAlignment() const { return tr::PropertyInfo::GetPropertyValueDirect<VAlignment>(this, VContentAlignmentId); }

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

LN_INTERNAL_ACCESS:
	int GetLayoutChildrenCount() const { return GetVisualChildrenCount(); }
	ILayoutElement* GetLayoutChild(int index) const { return GetVisualChild(index); }

	//void SetVisualTreeRoot(UIElement* element);
	//UIElement* GetVisualTreeRoot() { return m_visualTreeRoot; }

private:

	//UIElement*	m_visualTreeRoot;
};

LN_NAMESPACE_END
