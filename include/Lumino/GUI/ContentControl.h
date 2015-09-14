
#pragma once
#include "Control.h"
#include "ContentPresenter.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		1 つのコンテンツを持つコントロールを表します。
*/
class ContentControl
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ContentControl);
public:
	ContentControl(GUIManagerImpl* manager);
	virtual ~ContentControl();


	/// このコントロールのコンテンツを設定します。
	void SetContent(Variant value);

	//virtual void Render();

protected:
	//virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	////virtual void ApplyTemplate(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalLocalRect);
	//virtual void OnRender();
	//virtual bool OnEvent(EventType type, RoutedEventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

	virtual void PollingTemplateChildCreated(UIElement* newElement);

protected:
	// override UIElement
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

private:
	ContentPresenter*	m_contentPresenter;
	Variant		m_content;
	RefPtr<UIElement>	m_childElement;	///< m_content が UIElement であればそれを指す
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
