
#pragma once
#include "../UIElement.h"
#include "../UIElementCollection.h"

namespace Lumino
{
namespace GUI
{

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
	Panel(GUIManager* manager);
	virtual ~Panel();

	UIElementCollection* GetChildren() { return m_children; }


public:
	// 子要素を持つ要素の override
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalRect);
	virtual bool OnEvent(EventType type, EventArgs* args);
	virtual void Render();

	//virtual void ApplyTemplate(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalRect);

	// IAddChild
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	RefPtr<UIElementCollection>		m_children;
};


} // namespace GUI
} // namespace Lumino
