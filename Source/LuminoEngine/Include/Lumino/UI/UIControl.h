
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN
class UIElementCollection;
class UILayoutPanel;
class UIContextMenu;


/** ボタンのクリックイベントを発生させるタイミングを表します。*/
enum class ClickMode
{
	/** ボタンを離したときにイベントを発生させます。*/
	Release,

	/** ボタンを押したときにイベントを発生させます。*/
	Press,
};

/**
	@brief		
*/
class UIControl
	: public UIElement
	, public tr::IUIElementCollectionOwner
{
	LN_OBJECT;
public:
	HAlignment	HContentAlignment;
	VAlignment	VContentAlignment;

	//static const String NormalState;
	//static const String MouseOverState;
	//static const String PressedState;
	//static const String DisabledState;
	//static const String UnfocusedState;
	//static const String FocusedState;
	//static const String ValidState;
	//static const String InvalidState;

public:
	
	void setHContentAlignment(HAlignment value) { HContentAlignment = value; }
	HAlignment getHContentAlignment() const { return HContentAlignment; }

	void setVContentAlignment(VAlignment value) { VContentAlignment = value; }
	VAlignment getVContentAlignment() const { return VContentAlignment; }
	


	UIElementCollection* getItems() const;

	void addChild(UIElement* element);
	void removeChild(UIElement* element);
	void clearChildren();

	void setLayoutPanel(UILayoutPanel* panel);
	UILayoutPanel* getLayoutPanel() const;

	/** この要素に対してメニュー表示が要求された場合に表示するコンテキストメニューを設定します。 */
	void setContextMenu(UIContextMenu* menu);

	/** この要素に対してメニュー表示が要求された場合に表示するコンテキストメニューを取得します。 */
	UIContextMenu* getContextMenu() const;

	/** この要素へ決定操作を通知します。 */
	void submit();

	/** onSubmit イベントの通知を受け取るコールバックを登録します。*/
	LN_METHOD(Event)
	EventConnection connectOnSubmit(UIEventHandler handler);

protected:
	void setLogicalChildrenPresenter(UILayoutPanel* presenter);
	UILayoutPanel* getLogicalChildrenPresenter() const;

	virtual void onSubmit(UIEventArgs* e);
	virtual void onMouseClick(UIMouseEventArgs* e);

	// UIElement interface
	//virtual int getVisualChildrenCount() const override;
	//virtual ILayoutElement* getVisualChild(int index) const override;
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual const VAlignment* getPriorityContentVAlignment() override;
	virtual const HAlignment* getPriorityContentHAlignment() override;
	virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual void onGotFocus(UIEventArgs* e) override;
	virtual void onLostFocus(UIEventArgs* e) override;
	virtual void onMouseDown(UIMouseEventArgs* e) override;
	virtual void onMouseUp(UIMouseEventArgs* e) override;
	virtual void onMouseEnter(UIMouseEventArgs* e) override;
	virtual void onMouseLeave(UIMouseEventArgs* e) override;

	virtual void onLogicalChildrenPresenterChanged(UILayoutPanel* presenter);

	// IUIElementCollectionOwner interface
	virtual void onChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

LN_CONSTRUCT_ACCESS:
	UIControl();
	virtual ~UIControl();
	void initialize();

LN_INTERNAL_ACCESS:
	int getLayoutChildrenCount() const { return getVisualChildrenCount(); }
	ILayoutElement* getLayoutChild(int index) const { return getVisualChild(index); }

	//void SetVisualTreeRoot(UIElement* element);
	//UIElement* GetVisualTreeRoot() { return m_visualTreeRoot; }

private:
	Ref<UIElementCollection>		m_items;
	Ref<UILayoutPanel>				m_itemsHostPanel;		// TODO: 後で ContentControl 作ってそっちに持っていこう
	Ref<UILayoutPanel>				m_logicalChildrenPresenter;	// 子要素の実際の追加先
	Ref<UIContextMenu>				m_contextMenu;

	ClickMode						m_clickMode;
	bool							m_isPressed;

	UIEventHandler::EventType		m_onSubmit;

	//bool							m_invalidateItemsHostPanel;

	//UIElement*	m_visualTreeRoot;
};


/**
	@brief	複数のコントロールを組み合わせたコントロールを作成するためのクラスです。
*/
class UIUserControl
	: public UIControl
{
	LN_OBJECT;
	
public:

	/** UIUserControl のインスタンスを作成します。 */
	static Ref<UIUserControl> create();

LN_CONSTRUCT_ACCESS:
	UIUserControl();
	virtual ~UIUserControl();
	void initialize();
};


LN_NAMESPACE_END
