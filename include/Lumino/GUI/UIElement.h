
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/Property.h>
#include "Common.h"
#include "../RoutedEvent.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "BitmapBrush.h"	// for button
#include "ControlTemplate.h"

#define LN_UI_ELEMENT_SUBCLASS_DECL(subClassName) \
public: \
	static const String TypeID; \
	static CoreObject* CreateInstance(GUIManager* manager) { return LN_NEW subClassName(manager); } \
	virtual const String& GetTypeID() const { return TypeID; }

#define LN_UI_ELEMENT_SUBCLASS_IMPL(subClassName) \
	const String subClassName::TypeID(_T(#subClassName))

namespace Lumino
{
namespace GUI
{
//typedef String	PropertyID;
//typedef String	EventID;

class CanExecuteRoutedCommandEventArgs;
class ExecuteRoutedCommandEventArgs;
class RoutedCommandTypeContext;

LN_ENUM(HorizontalAlignment)
{
	Left = 0,
	Center,
	Right,
	Stretch,
};
LN_ENUM_DECLARE(HorizontalAlignment);

LN_ENUM(VerticalAlignment)
{
	Bottom = 0,		///< 子要素を、親のレイアウト スロットの下端に揃えて配置します。
	Center,			///< 子要素を、親のレイアウト スロットの中央に揃えて配置します。
	Stretch,		///< 子要素を、親のレイアウト スロット全体に引き伸ばします。
	Top,			///< 子要素を、親のレイアウト スロットの上端に揃えて配置します。
};
LN_ENUM_DECLARE(VerticalAlignment);
	
LN_ENUM(Orientation)
{
	Vertical = 0,
	Horizontal,
};
LN_ENUM_DECLARE(Orientation);



class IAddChild
{
public:
	virtual void AddChild(const Variant& value) = 0;
	virtual void AddText(const String& text) = 0;
};

/**
	@brief		
*/
class UIElement
	: public CoreObject
	, public IAddChild
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	static const Property*		SizeProperty;
	static const Property*		HorizontalAlignmentProperty;
	static const Property*		VerticalAlignmentProperty;
	static const Property*		IsHitTestProperty;

	static const RoutedEvent*	MouseMoveEvent;
	static const RoutedEvent*	MouseLeaveEvent;
	static const RoutedEvent*	MouseEnterEvent;
	static const RoutedEvent*	MouseDownEvent;
	static const RoutedEvent*	MouseUpEvent;

	static const RoutedEvent*	CanExecuteRoutedCommandEvent;	///< このイベントは内部用であるため、ユーザーはハンドルすることはできない
	static const RoutedEvent*	ExecuteRoutedCommandEvent;		///< このイベントは内部用であるため、ユーザーはハンドルすることはできない

public:
	UIElement(GUIManager* manager);
	virtual ~UIElement();

	/// 論理ツリー上の親要素を取得します。
	UIElement* GetParent() const { return m_parent; }

public:
	// Property

	//
	//void SetWidth(float width) { m_size.Width = width; }

	///// 要素の幅を取得します。
	//float GetWidth() { return m_size.Width; }

	///// 要素の高さを設定します。規定値は NAN で、自動的にサイズを計算します。
	//void SetHeight(float width) { m_size.Width = width; }

	//

	/// 要素のサイズを設定します。規定値は NAN で、自動的にサイズを計算します。
	void SetSize(const SizeF& size) { m_size = size; }// { SetValue(SizeProperty, size); }	// TODO: レイアウト更新中は頻繁にアクセスするのでできれば メンバ変数にしたい・・・
	
	/// 要素のサイズを取得します。
	const SizeF& GetSize() const { return m_size; }// { return GetValue(SizeProperty).GetSizeF(); }	// TODO: 危ない。参照で返すとスタックの Variant で消える


	void SetHorizontalAlignment(HorizontalAlignment value) { m_horizontalAlignment = value; }
	HorizontalAlignment GetHorizontalAlignment() const { return m_horizontalAlignment; }
	void SetVerticalAlignment(VerticalAlignment value) { m_verticalAlignment = value; }
	VerticalAlignment GetVerticalAlignment() const { return m_verticalAlignment; }

	/// ヒットテストの有無を設定します。
	void SetHitTest(bool enabled) { SetPropertyValue(IsHitTestProperty, Variant(enabled)); }

	/// ヒットテストの有無を取得します。
	bool IsHitTest() const { return GetPropertyValue(IsHitTestProperty).GetBool(); }


	void CaptureMouse();
	void ReleaseMouseCapture();








	Event01<MouseEventArgs*>	MouseMove;
	Event01<MouseEventArgs*>	MouseEnter;
	Event01<MouseEventArgs*>	MouseLeave;
	Event01<MouseEventArgs*>	MouseDown;
	Event01<MouseEventArgs*>	MouseUp;

	// イベントの扱い方は WPF とは少し違う。
	// WPF の ButtonBase.Click は、
	// public event RoutedEventHandler Click { add { AddHandler(ClickEvent, value); } remove { RemoveHandler(ClickEvent, value); } }
	// のようになっている。
	// つまり、event Click は単なるお飾り。Click(...) のように、これが直接呼ばれることはない。
	// RaiseEvent(ClickEvent) から呼ばれる。
	// RaiseEvent(ClickEvent) は ButtonBase.OnClick() で呼んでいる。
	

	//void AddMouseMoveHandler(const Delegate02<CoreObject*, MouseEventArgs*>& handler) { AddHandler(MouseMoveEvent, handler); }
	//void RemoveMouseMoveHandler(const Delegate02<CoreObject*, MouseEventArgs*>& handler) { RemoveHandler(MouseMoveEvent, handler); }

	// ↓この2つは C_API から使用する
	virtual bool AddHandlerInternal(const String& key, void* nativeCFuncPtr) { LN_THROW(0, ArgumentException); return false; }
	virtual bool RemoveHandlerInternal(const String& key, void* nativeCFuncPtr) { LN_THROW(0, ArgumentException); return false; }


	/// rootLogicalParent : テンプレートを適用した要素。TemplateBinding のソースオブジェクト。
	void SetTemplateBinding(const Property* thisProp, const String& srcPropPath/*, UIElement* rootLogicalParent*/);

	/// (サイズの自動計算が有効になっている要素に対しては呼び出しても効果はありません)
	void UpdateLayout();
	void UpdateTransformHierarchy();
	virtual void Render();


	/// 現在のテンプレートからビジュアルツリーを再構築します。
	/// この関数は必要なタイミングでレイアウトシステムから呼び出されます。通常、明示的に呼び出す必要はありません。
	///		というか、呼び出しちゃダメ。必ずルートから再帰的に更新しないと、もし親がまだ ApplyTemplate() してない状態でこれを呼ぶと
	///		ローカルリソースが正しく更新されない。
	///		TODO: もしかしたら、SetParent した瞬間にローカルリソースを更新したほうが良いかも？
	///		そうすればいつ ApplyTemplate() を呼び出しても良いが… 需要は無いか。
	void ApplyTemplate();

	/// internal  別コントロールの子に追加されたとき等に true がセットされる。
	/// パフォーマンス的な理由もあるが、本来 ApplyTemplate するべきタイミングでは親要素のローカルリソースが確定していないことがあるため。
	/// true がセットされたら、親へ伝播していく。
	void SetChildTemplateModified(bool modified) { m_childTemplateModified = modified; if (modified && m_parent != NULL) { m_parent->SetChildTemplateModified(true); } }
	void SetTemplateModified(bool modified) { m_templateModified = modified; if (modified && m_parent != NULL) { m_parent->SetChildTemplateModified(true); } }

public:
	const SizeF& GetDesiredSize() const {
		return m_desiredSize;
	}


	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize) { return finalSize; }

	/// 現在のテンプレートからビジュアルツリーが再構築された後に呼び出されます。
	/// 派生クラスは localResource に対してキー値からリソースを取得することができます。
	virtual void OnApplyTemplate(CombinedLocalResource* localResource) {}
	virtual void OnRender() {}
	virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }	// 論理要素の追加。オーバーライドする。
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

public:	// internal
	void SetParent(UIElement* parent) { m_parent = parent; }
	//virtual void AddChildToVisualTree(UIElement* element) { LN_THROW(0, InvalidOperationException); }

	// コンストラクタの後呼び出す。
	void InitializeComponent();

	/// 
	void UpdateTemplateLogicalParentHierarchy(UIElement* logicalParent);

protected:
	virtual const String& GetTypeID() const = 0;

	// override
	virtual void OnPropertyChanged(const String& name, const Variant& newValue);

	//friend class Decorator;
	//friend class Control;
	//friend class ContentControl;
	//friend class Panel;
public:	// internal
	virtual void ApplyTemplateHierarchy(/*CombinedLocalResource* parent*/);
	void UpdateTemplateHierarchy();	// こちらは modified マークされている要素のテンプレートを更新する


	GUIManager* GetManager() const { return m_manager; }

private:
	friend class UIElementFactory;
	friend class ControlTemplate;
	friend class DataTemplate;
	void SetTemplateChild(UIElement* child);
	//virtual void AddVisualChild(UIElement* child);	///< AddChild() は論理ツリーへの追加、AddVisualChild() はビジュアルツリーへの追加。

	void UpdateLocalResource();
protected:
	virtual void PollingTemplateChildCreated(UIElement* element) {}

private:
	// TypedRoutedEvent からコールバックされる。On～とは別に、単にイベントを発生させるコールバクとして必要。(TypedRoutedEvent は状態を持てないので)
	// どんな手を使っても、結局 TypedRoutedEvent から Event02 を呼べる手段が必要なので、これらは必ず必要になる。
	//void CallMouseMoveEvent(CoreObject* sender, MouseEventArgs* e) { MouseMove.Raise(sender, e); }
	//void CallMouseEnterEvent(CoreObject* sender, MouseEventArgs* e) { MouseEnter.Raise(sender, e); }
	//void CallMouseLeaveEvent(CoreObject* sender, MouseEventArgs* e) { MouseLeave.Raise(sender, e); }
	//void CallMouseDownEvent(CoreObject* sender, MouseEventArgs* e) { MouseDown.Raise(sender, e); }
	//void CallMouseUpEvent(CoreObject* sender, MouseEventArgs* e) { MouseUp.Raise(sender, e); }

protected:
	//void RegisterRoutedEvent(RoutedEvent* ev) {
	//	m_routedEventList.Add(ev->GetName(), ev);
	//}
	//void RegisterRoutedEventHandler(EventID id, RoutedEventHandler* handler) {
	//	m_routedEventHandlerList.Add(id, handler);
	//}
	void AddRoutedCommandTypeContext(RoutedCommandTypeContext* c) {
		if (!m_routedCommandTypeContextList.Contains(c)) {	// 同じものは登録しない
			m_routedCommandTypeContextList.Add(c);
		}
	}

public:
	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEvent(const RoutedEvent* ev, CoreObject* sender, EventArgs* e)
	{
		e->Sender = sender;
		RaiseEventInternal(ev, e);
	}

private:
	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEventInternal(const RoutedEvent* ev, EventArgs* e)
	{
		CoreObject::RaiseEventInternal(ev, e);

		// bubble
		if (!e->Handled && m_parent != NULL)
		{
			m_parent->RaiseEventInternal(ev, e);
		}
	}

	//bool RaiseCommand(RoutedCommand* command, const Variant& parameter, bool execute)
	//{
	//	if (execute)
	//	{
	//		for (auto context : m_commandOwnerClassContextList) {
	//			if (context->Execute(this, command, parameter)) { return true; }
	//		}
	//	}
	//	else
	//	{
	//		for (auto context : m_commandOwnerClassContextList) {
	//			bool r;
	//			if (context->CanExecute(this, command, parameter, &r)) { return r; }
	//		}
	//	}

	//}



protected:
	// サブクラスはできるだけ Event にハンドラを登録するのではなく、On～をオーバーライドすることでイベントをハンドリングする。
	// ハンドリングしたら e->Handled を true にする。そして super を呼び出す。こうすることで、RaiseEvent() でのイベント検索や
	// ルーティングを行わないので負荷軽減ができる。
	virtual void OnMouseMove(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }
	virtual void OnMouseDown(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseDownEvent, this, e); } }
	virtual void OnMouseUp(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseUpEvent, this, e); } }



	GUIManager*				m_manager;
	UIElement*				m_parent;				///< 親要素 (論理・ビジュアルは関係ない。RoutedEvent(Bubble) の通知先となる)

	VisualStateInstance*	m_visualStateInstance;

	SizeF					m_desiredSize;			///< MeasureLayout() で決定されるこのコントロールの最終要求サイズ
	RectF					m_finalLocalRect;			///< 描画に使用する最終境界矩形 (グローバル座標系=RootPane のローカル座標系)
	RectF					m_finalGlobalRect;


	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;

	// 削除予定
	typedef SortedArray<String, RefObject*>	EventDataStore;
	EventDataStore	m_eventDataStore;
	//Event02<CoreObject*, MouseEventArgs*> m_eventMouseMove;

	//typedef SortedArray<String, RoutedEvent*>	RoutedEventList;
	//RoutedEventList	m_routedEventList;

	//typedef SortedArray<EventID, RoutedEventHandler*>	RoutedEventHandlerList;
	//RoutedEventHandlerList	m_routedEventHandlerList;

	/// あるクラスに含まれる RoutedCommand のリストのリスト。このリストには基底クラスから順に詰まっている
	Array<RoutedCommandTypeContext*>	m_routedCommandTypeContextList;

	// Property
	SizeF				m_size;
	HorizontalAlignment	m_horizontalAlignment;
	VerticalAlignment	m_verticalAlignment;


public:	// TODO: private にしたい
	void Handler_CanExecuteRoutedCommandEvent(CanExecuteRoutedCommandEventArgs* e);
	void Handler_ExecuteRoutedCommandEvent(ExecuteRoutedCommandEventArgs* e);

private:
	void TemplateBindingSource_PropertyChanged(/*CoreObject* sender, */PropertyChangedEventArgs* e);


	struct TemplateBindingInfo
	{
		const Property*	ThisProp;
		String			SourcePropPath;
	};

	typedef Array<TemplateBindingInfo>	TemplateBindingInfoList;
	TemplateBindingInfoList		m_templateBindingInfoList;

	/// テンプレートを適用した要素。TemplateBinding のソースオブジェクト。これが NULL でなければ、this は VisualTree 要素である。
	/// 例えば Button にテンプレートを適用すると、Button よりしたのビジュアル要素.m_rootLogicalParent はすべて Button を指す。
	UIElement*		m_rootLogicalParent;
	Delegate01<PropertyChangedEventArgs*>	m_templateBindingHandler;

	bool			m_childTemplateModified;	///< 子要素のテンプレートを更新するべきか
	bool			m_templateModified;

protected:
	RefPtr<Style>		m_style;
	RefPtr<UIElement>	m_templateChild;
	UIElement*			m_templateParent;

	// このリストに追加された UIElement は、OnEvent、Render、ヒットテスト等がこのクラスより自動的に呼ばれる。
	// ただし、レイアウト関係はノータッチ。Measure や Arrange は呼ばれない。
	// 使い方としては、論理要素だろうがビジュアル要素だろうが関係なくまずはこのリストに追加しておく。
	// レイアウトは (メンドくさいが) 個々のコンテナ要素で面倒を見る。
	// ただし、m_templateChild は このクラスでも面倒を見てあげる。でも、凝ったレイアウトが必要な時は
	// オーバーライドする必要がある。
	Array<UIElement*>	m_visualChildren;
};

/**
	@brief		1 つの子要素の周囲を装飾する UI 要素の基本クラスです。
*/
class Decorator
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Decorator);
public:
	Decorator(GUIManager* manager);
	virtual ~Decorator();

public:
	void SetChild(UIElement* element);
	UIElement* GetChild() const;

public:
	//virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);

	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }
	//virtual void AddVisualChild(UIElement* child);


protected:
	// override UIElement
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);;
	//virtual void AddChildToVisualTree(UIElement* element);

private:
	RefPtr<UIElement>	m_child;
	//RefPtr<UIElement>	m_visualChild;
};

/**
	@brief		Button コントロールの外観です。
*/
class ButtonChrome
	: public Decorator
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ButtonChrome);
public:
	static const Property*	IsMouseOverProperty;	///< IsMouseOver プロパティの識別子
	static const Property*	FrameWidthProperty;		///< FrameWidth プロパティの識別子

public:
	static ButtonChrome* Create(GUIManager* manager);

	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

public:
	// Property
	void SetMouseOver(bool value) { m_isMouseOver = value; }
	bool IsMouseOver() const { return m_isMouseOver; }

	/// ボタンイメージの外枠の幅を設定します。
	void SetFrameWidth(float width) { m_frameWidth = width; }

	/// ボタンイメージの外枠の幅を取得します。
	float GetFrameWidth() const { return m_frameWidth; }

public:
	// override CoreObject
	//virtual void SetPropertyValue(const Property* prop, const Variant& value);
	//virtual Variant GetPropertyValue(const Property* prop) const;

protected:
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void OnRender();

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	float							m_frameWidth;	///< 枠の幅 (描画で使用するためパフォーマンスを考慮し、GetValue() ではなくメンバに持っておく)
	bool							m_isMouseOver;
};


/**
	@brief	ContentControl のコンテンツを表示します。
*/
class ContentPresenter
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ContentPresenter);
public:

public:
	static ContentPresenter* Create(GUIManager* manager);

	ContentPresenter(GUIManager* manager);
	virtual ~ContentPresenter();

	void SetContent(UIElement* content);

protected:
#if 1
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void Render();
#endif

private:
	RefPtr<UIElement>	m_content;
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// ContentPresenter は論理的な子要素の配置先をマークするメタデータのようなものなので、子要素は持たない。
};

/**
	@brief	ItemsControl のコンテンツを表示します。
*/
class ItemsPresenter
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsPresenter);
public:

public:
	static ItemsPresenter* Create(GUIManager* manager);

	ItemsPresenter(GUIManager* manager);
	virtual ~ItemsPresenter();

protected:
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// ItemsPresenter は論理的な子要素の配置先をマークするメタデータのようなものなので、子要素は持たない。
};

/**
	@brief		ユーザー操作によって特定の動作を行う UI 要素の基本クラスです。
*/
class Control
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Control);
//public:
//	static const String ControlTemplateTypeName;
	static const Property* TemplateProperty;

public:
	Control(GUIManager* manager);
	virtual ~Control();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	void SetTemplate(ControlTemplate* controlTemplate);
	ControlTemplate* GetTemplate() const { return m_controlTemplate; }

	/** @} */

protected:
	// override UIElement
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

	//friend class UIElementFactory;

	//friend class ControlTemplate;
	//friend class DataTemplate;
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// Contro 自体は子要素を持たない。サブクラスでさらにオーバーライドされる。

	//virtual void ApplyTemplate(CombinedLocalResource* parent);

//	virtual bool ApplyTemplateInternal();
//	bool ApplyTemplateInternalMain(const String& typeName);
//
//
//protected:
//	// override UIElement
//	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
private:
	RefPtr<ControlTemplate>	m_controlTemplate;
};

/**
	@brief		1 つのコンテンツを持つコントロールを表します。
*/
class ContentControl
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ContentControl);
public:
	ContentControl(GUIManager* manager);
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
	//virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

	virtual void PollingTemplateChildCreated(UIElement* element);

protected:
	// override UIElement
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

private:
	ContentPresenter*	m_contentPresenter;
	Variant		m_content;		
	RefPtr<UIElement>	m_childElement;	///< m_content が UIElement であればそれを指す
};

/**
	@brief		ルート要素。
*/
class RootPane
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RootPane);
public:
	static RootPane* Create(GUIManager* manager);

	RootPane(GUIManager* manager);
	virtual ~RootPane();

	//virtual void Render();

protected:
	//virtual void OnRender() {}

	friend class GUIManager;

private:
};

/**
	@brief		
*/
class Button
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Button);
public:
	static const String	IsMouseOverProperty;	///< IsMouseOver プロパティの識別子


	static const String ControlTemplateTypeName;	///< "Button"


	static Button* Create(GUIManager* manager);

	Button(GUIManager* manager);
	virtual ~Button();

public:
	bool IsMouseOver() const { return m_isMouseOver; }

	//virtual void Render();

protected:
	virtual void OnClick();
	virtual bool OnEvent(EventType type, EventArgs* args);
	virtual void OnRender();
	//virtual void Render();


	virtual void OnMouseMove(MouseEventArgs* e);
	//void UIElement_MouseMove(CoreObject* sender, MouseEventArgs* e);

protected:
	// override UIElement

	//virtual bool ApplyTemplateInternal()
	//{
	//	if (ApplyTemplateInternalMain(ControlTemplateTypeName)) { return true; }
	//	return ContentControl::ApplyTemplateInternal();	// base
	//}

private:
	RefPtr<ButtonChrome>	m_chrome;	// TODO: 仮。ちゃんとテーマから読みだす
	bool m_isMouseOver;
};

} // namespace GUI
} // namespace Lumino
