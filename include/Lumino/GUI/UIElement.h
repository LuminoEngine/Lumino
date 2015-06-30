
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include "Common.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "BitmapBrush.h"	// for button
#include "RoutedEvent.h"

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
typedef String	PropertyID;
typedef String	EventID;


	struct Enum
	{
		int m_value;
		Enum() : m_value(0) {}
		Enum(int value) : m_value(value) {}
		operator int() const { return m_value; }
	};

	struct HorizontalAlignment : public Enum
	{
		enum
		{
			Left = 0,
			Center,
			Right,
			Stretch,
		};

		// WPF の Button の場合、初期値は Stretch だった

		HorizontalAlignment() : Enum() {}
		HorizontalAlignment(int value) : Enum(value) {}

	};

	struct VerticalAlignment : public Enum
	{
		enum
		{
			Bottom = 0,		///< 子要素を、親のレイアウト スロットの下端に揃えて配置します。
			Center,			///< 子要素を、親のレイアウト スロットの中央に揃えて配置します。
			Stretch,		///< 子要素を、親のレイアウト スロット全体に引き伸ばします。
			Top,			///< 子要素を、親のレイアウト スロットの上端に揃えて配置します。
		};

		// WPF の Button の場合、初期値は Stretch だった

		VerticalAlignment() : Enum() {}
		VerticalAlignment(int value) : Enum(value) {}

	};

	struct Orientation : public Enum
	{
		enum
		{
			Vertical = 0,
			Horizontal,
		};

		Orientation() : Enum() {}
		Orientation(int value) : Enum(value) {}

	};

	//enum Orientation
	//{
	//	Horizontal = 0,
	//	Orientation_Vertical,
	//};


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
public:
	static const String	SizeProperty;
	static const String	HorizontalAlignmentProperty;
	static const String	VerticalAlignmentProperty;
	static const String	IsHitTestProperty;

	static const String	MouseMoveEvent;
	static const String	MouseLeaveEvent;
	static const String	MouseEnterEvent;

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
	void SetHitTest(bool enabled) { SetValue(IsHitTestProperty, Variant(enabled)); }

	/// ヒットテストの有無を取得します。
	bool IsHitTest() const { return GetValue(IsHitTestProperty).GetBool(); }




	Event02<CoreObject*, MouseEventArgs*>	MouseMove;
	Event02<CoreObject*, MouseEventArgs*>	MouseEnter;
	Event02<CoreObject*, MouseEventArgs*>	MouseLeave;

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
	void SetTemplateBinding(Property* thisProp, const String& srcPropPath, UIElement* rootLogicalParent);

	/// (サイズの自動計算が有効になっている要素に対しては呼び出しても効果はありません)
	void UpdateLayout();
	virtual void Render();


	/// 現在のテンプレートからビジュアルツリーを再構築します。
	/// この関数は必要なタイミングでレイアウトシステムから呼び出されます。通常、明示的に呼び出す必要はありません。
	void ApplyTemplate();

public:
	const SizeF& GetDesiredSize() const {
		return m_desiredSize;
	}


	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);

	/// 現在のテンプレートからビジュアルツリーが再構築された後に呼び出されます。
	/// 派生クラスは localResource に対してキー値からリソースを取得することができます。
	virtual void OnApplyTemplate(CombinedLocalResource* localResource) {}
	virtual void OnRender() {}
	virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

public:	// internal
	void SetParent(UIElement* parent) { m_parent = parent; }
	//virtual void AddChildToVisualTree(UIElement* element) { LN_THROW(0, InvalidOperationException); }

protected:
	virtual const String& GetTypeID() const = 0;

	friend class Decorator;
	friend class Control;
	friend class ContentControl;
	friend class Panel;
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

	friend class UIElementFactory;
	friend class ControlTemplate;
	friend class DataTemplate;
	virtual void AddVisualChild(UIElement* child);	///< AddChild() は論理ツリーへの追加、AddVisualChild() はビジュアルツリーへの追加。

private:
	// TypedRoutedEvent からコールバックされる。On～とは別に、単にイベントを発生させるコールバクとして必要。(TypedRoutedEvent は状態を持てないので)
	// どんな手を使っても、結局 TypedRoutedEvent から Event02 を呼べる手段が必要なので、これらは必ず必要になる。
	void CallMouseMoveEvent(CoreObject* sender, MouseEventArgs* e) { MouseMove.Raise(sender, e); }
	void CallMouseEnterEvent(CoreObject* sender, MouseEventArgs* e) { MouseEnter.Raise(sender, e); }
	void CallMouseLeaveEvent(CoreObject* sender, MouseEventArgs* e) { MouseLeave.Raise(sender, e); }

protected:
	void RegisterRoutedEvent(RoutedEvent* ev) {
		m_routedEventList.Add(ev->GetName(), ev);
	}

	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEvent(const String& eventName, CoreObject* sender, EventArgs* e)
	{
		LN_FOREACH(RoutedEventList::Pair& pair, m_routedEventList)
		{
			if (pair.first == eventName) {
				pair.second->CallEvent(this, sender, e);
				break;	// ev と同じイベントは1つしかリスト内に無いはず
			}
		}

		// bubble
		if (!e->Handled && m_parent != NULL)
		{

			LN_FOREACH(RoutedEventList::Pair& pair, m_parent->m_routedEventList)
			{
				if (pair.first == eventName) {
					pair.second->Raise(m_parent, sender, e);	// 親のOn～ が呼ばれる
					break;	// ev と同じイベントは1つしかリスト内に無いはず
				}
			}

			//m_parent->RaiseEvent(eventName, sender, e);
		}
		
	}

	// サブクラスはできるだけ Event にハンドラを登録するのではなく、On～をオーバーライドすることでイベントをハンドリングする。
	// ハンドリングしたら e->Handled を true にする。そして super を呼び出す。こうすることで、RaiseEvent() でのイベント検索や
	// ルーティングを行わないので負荷軽減ができる。
	virtual void OnMouseMove(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }


	GUIManager*				m_manager;
	UIElement*				m_parent;				///< 論理ツリー上の親要素

	VisualStateInstance*	m_visualStateInstance;

	SizeF					m_desiredSize;			///< MeasureLayout() で決定されるこのコントロールの最終要求サイズ
	RectF					m_finalRect;			///< 描画に使用する最終境界矩形 (グローバル座標系=RootPane のローカル座標系)

	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;

	// 削除予定
	typedef SortedArray<String, RefObject*>	EventDataStore;
	EventDataStore	m_eventDataStore;
	//Event02<CoreObject*, MouseEventArgs*> m_eventMouseMove;

	typedef SortedArray<String, RoutedEvent*>	RoutedEventList;
	RoutedEventList	m_routedEventList;


	// Property
	SizeF				m_size;
	HorizontalAlignment	m_horizontalAlignment;
	VerticalAlignment	m_verticalAlignment;

private:

	void TemplateBindingSource_PropertyChanged(CoreObject* sender, PropertyChangedEventArgs* e);


	struct TemplateBindingInfo
	{
		Property*	ThisProp;
		String		SourcePropPath;
	};

	typedef Array<TemplateBindingInfo>	TemplateBindingInfoList;
	TemplateBindingInfoList		m_templateBindingInfoList;

	UIElement*		m_rootLogicalParent;	///< テンプレートを適用した要素。TemplateBinding のソースオブジェクト。これが NULL でなければ、this は VisualTree 要素である。

protected:
	/// 直接の子 Visual リスト。
	/// このリストに論理要素は直接含まない。論理要素は これらの Visual の下にある ContentPresenter または ItemsPresenter の子として追加される。
	/// 例えば Button::SetContent() で セットされた UIElement は m_visualChildren から辿れる ContentPresenter に追加される。
	Array< RefPtr<UIElement> >	m_visualChildren;	// TOOD: List にしなくてもいいかも？
	//UIElement*		m_visualChildren;
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
	static const String	IsMouseOverProperty;	///< IsMouseOver プロパティの識別子
	static const String	FrameWidthProperty;		///< FrameWidth プロパティの識別子

public:
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
	virtual void SetValue(const String& propertyName, const Variant& value);
	virtual Variant GetValue(const String& propertyName) const;

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
	ContentPresenter(GUIManager* manager);
	virtual ~ContentPresenter();

protected:
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


public:
	Control(GUIManager* manager);
	virtual ~Control();

protected:
	// override UIElement
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

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
	//virtual void ArrangeLayout(const RectF& finalRect);
	//virtual void OnRender();
	//virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	// override UIElement
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

private:
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
