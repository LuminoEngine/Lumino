
#pragma once
#include "../../CoreObjectList.h"
#include "../UIElement.h"
#include "Panel.h"

namespace Lumino
{
namespace GUI
{
class ListBox;

/**
	@brief
*/
class ListBoxItem
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ListBoxItem);
public:
	ListBoxItem(GUIManager* manager);
	virtual ~ListBoxItem();
};


/**
	@brief
*/
class ListBoxItemList
	: public CoreObjectList<ListBoxItem>
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	ListBoxItemList(ListBox* owner) : m_owner(owner) {}
	virtual ~ListBoxItemList() {}

protected:
	virtual void OnItemAdded(ListBoxItem* item);
	virtual void OnItemRemoved(ListBoxItem* item);

private:
	ListBox*	m_owner;
};

/**
	@brief		ListBox コントロールの外観です。
*/
class ListBoxChrome
	: public Decorator
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ListBoxChrome);
public:
	//static const String	IsMouseOverProperty;	///< IsMouseOver プロパティの識別子
	//static const String	FrameWidthProperty;		///< FrameWidth プロパティの識別子

public:
	ListBoxChrome(GUIManager* manager);
	virtual ~ListBoxChrome();

public:
	// Property
	//void SetMouseOver(bool value) { m_isMouseOver = value; }
	//bool IsMouseOver() const { return m_isMouseOver; }

	///// ボタンイメージの外枠の幅を設定します。
	//void SetFrameWidth(float width) { m_frameWidth = width; }

	///// ボタンイメージの外枠の幅を取得します。
	//float GetFrameWidth() const { return m_frameWidth; }

protected:
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void OnRender();

private:
	RefPtr<Graphics::TextureBrush>	m_frameBrush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	float							m_frameWidth;
	bool							m_isMouseOver;
};



/**
	@brief		
*/
class ListBox
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ListBox);
public:
	ListBox(GUIManager* manager);
	virtual ~ListBox();

	ListBoxItemList* GetListBoxItems() { return m_listBoxItems; }

	// ユーティリティ
	void InsertItem(int index, UIElement* element);

	//ItemsPanel
protected:
#if 0
	// 子要素を持つ要素の override
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);
	virtual bool OnEvent(EventType type, EventArgs* args);
	virtual void Render();
#endif

	// ListBoxItemList callback
	friend class ListBoxItemList;
	void OnListBoxItemAdded(ListBoxItem* item);
	void OnListBoxItemRemoved(ListBoxItem* item);

	//virtual void Render();
	//virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	//virtual bool OnEvent(EventType type, EventArgs* args);
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	//virtual void ApplyTemplate(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalLocalRect);

protected:
	RefPtr<ListBoxItemList>	m_listBoxItems;		///< logical children
	RefPtr<Panel>			m_itemsPanel;		///< ListBoxItem が配置される Panel

	// ListBox のスタイルとテンプレート
	// https://msdn.microsoft.com/ja-jp/library/ms754242%28v=vs.110%29.aspx
};


} // namespace GUI
} // namespace Lumino
