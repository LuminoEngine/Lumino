
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN

/**
	@brief		1つ以上の子 UIElement を持つ UI 要素のベースクラスです。
*/
class UIPanel
	: public UIElement
	, public tr::IUIElementCollectionOwner
{
	LN_UI_TYPEINFO_DECLARE();
public:
	void AddChild(UIElement* element);
	void RemoveChild(UIElement* element);

LN_PROTECTED_INTERNAL_ACCESS:
	UIPanel();
	virtual ~UIPanel();
	void Initialize(detail::UIManager* manager);
	UIElementCollection* GetChildren() const { return m_children; }

	// UIElement interface
	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChildOrderd(int index) const override;
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

private:
	RefPtr<UIElementCollection>	m_children;
};

LN_NAMESPACE_END
