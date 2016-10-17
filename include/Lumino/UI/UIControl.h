
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
	LN_TR_PROPERTY(HAlignment,	HContentAlignmentProperty);		/**< HContentAlignment プロパティの識別子 */
	LN_TR_PROPERTY(VAlignment,	VContentAlignmentProperty);		/**< VContentAlignment プロパティの識別子 */
	
public:
	
	void SetHContentAlignment(HAlignment value) { tr::Property::SetPropertyValueDirect<HAlignment>(this, HContentAlignmentProperty, value); }
	HAlignment GetHContentAlignment() const { return tr::Property::GetPropertyValueDirect<HAlignment>(this, HContentAlignmentProperty); }

	void SetVContentAlignment(VAlignment value) { tr::Property::SetPropertyValueDirect<VAlignment>(this, VContentAlignmentProperty, value); }
	VAlignment GetVContentAlignment() const { return tr::Property::GetPropertyValueDirect<VAlignment>(this, VContentAlignmentProperty); }

protected:
	UIControl();
	virtual ~UIControl();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChildOrderd(int index) const override;
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;
	virtual VAlignment* GetPriorityContentVAlignment() override;
	virtual HAlignment* GetPriorityContentHAlignment() override;

LN_INTERNAL_ACCESS:
	void SetVisualTreeRoot(UIElement* element);
	UIElement* GetVisualTreeRoot() { return m_visualTreeRoot; }

private:
	VAlignment	m_verticalContentAlignment;
	HAlignment	m_horizontalContentAlignment;

	UIElement*	m_visualTreeRoot;
};

LN_NAMESPACE_END
