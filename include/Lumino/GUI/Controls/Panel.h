
#pragma once
#include "../UIElement.h"
#include "../UIElementCollection.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		Panel 要素の基本クラスです。
*/
class Panel
	: public UIElement
{
	// [ContentProperty("Children")]
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Panel);
public:
	Panel(GUIManagerImpl* manager);
	virtual ~Panel();

	UIElementCollection* GetChildren() { return m_children; }


public:
#if 0
	// 子要素を持つ要素の override
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalLocalRect);
	virtual bool OnEvent(EventType type, RoutedEventArgs* args);
	virtual void Render();

	//virtual void ApplyTemplate(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalLocalRect);
#endif

	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }


private:
	void Children_ItemAdded(UIElement* item);
	void Children_ItemRemoved(UIElement* item);

LN_INTERNAL_ACCESS:
	virtual UIElement* GetVisualChildOrderd(int index) const;
	virtual void ActivateInternal(UIElement* child);

protected:
	RefPtr<UIElementCollection>		m_children;
private:
	Array<UIElement*>	m_orderdVisualChildren;
};

class PilePanel
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(PilePanel);
public:
	/**
		@brief	PilePanel を作成します。
		@return	作成されたインスタンスのポインタ。使い終えたら Release() を呼び出して、参照を解放してください。	
	*/
	static PilePanel* Create();

protected:
	PilePanel(GUIManagerImpl* manager);
	virtual ~PilePanel();
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
