
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/Property.h>
#include "Common.h"
#include "../RoutedEvent.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "ControlTemplate.h"

#define LN_UI_ELEMENT_SUBCLASS_DECL(className) \
public: \
	static const String TypeID; \
	virtual const String& GetTypeID() const { return TypeID; } \
private: \
	friend class GUIManager;\
	friend class GUIHelper;\
	static className* internalCreateInstance(GUIManager* manager) \
	{ \
		auto obj = RefPtr<className>(LN_NEW className(manager)); \
		obj->InitializeComponent(); \
		obj.SafeAddRef(); \
		return obj; \
	}


#define LN_UI_ELEMENT_SUBCLASS_IMPL(className) \
	const String className::TypeID(_T(#className))

namespace Lumino
{
namespace GUI
{
class CanExecuteRoutedCommandEventArgs;
class ExecuteRoutedCommandEventArgs;
class RoutedCommandTypeContext;


/**
	@brief		
*/
class RoutedEventSlotBase
{
protected:
	RoutedEventSlotBase() {}
	virtual ~RoutedEventSlotBase() {}

private:
	friend class UIElement;
	virtual void Raise(RoutedEventArgs* e) = 0;
};


/**
	@brief		
	@details	RoutedEvent は UIElement 及びそのサブクラス内部からのみ発生させることが出来ます。
*/
template<class TArgs>
class RoutedEventSlot
	: public RoutedEventSlotBase
{
public:
	RoutedEventSlot() {}
	virtual ~RoutedEventSlot() {}

public:

	/**
		@brief	ルーティングイベントのハンドラを追加します。
	*/
	void AddHandler(const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Add(handler);
	}
	
	/**
		@brief	指定したハンドラに一致するハンドラを、このスロットから削除します。
	*/
	void RemoveHandler(const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Remove(handler);
	}
	
	/**
		@brief	ルーティングイベントのハンドラを追加します。
	*/
	void operator += (const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Add(handler);
	}
	
	/**
		@brief	指定したハンドラに一致するハンドラを、このスロットから削除します。
	*/
	void operator -= (const Delegate01<TArgs*>& handler)
	{
		m_handlerList.Remove(handler);
	}

private:
	Array< Delegate01<TArgs*> > m_handlerList;

	virtual void Raise(RoutedEventArgs* e)
	{
		for (Delegate01<TArgs*>& d : m_handlerList)
		{
			d.Call(static_cast<TArgs*>(e));
		}
	}

	// Lumino::EventXX のような使い方と同時に、
	// Property にたいする メンバ変数のような意味も持つ。
	// 即ち、ハンドラのリスト。
	// RoutedEventSlot は、いわゆる「動的なハンドラ」
};




/**
	@brief	GUI 要素のベースクラスです。
*/
class UIElement
	: public CoreObject
	, public IAddChild
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	LN_PROPERTY(SizeF,					SizeProperty);					/**< Size プロパティの識別子 */
	LN_PROPERTY(ThicknessF,				MarginProperty);				/**< Margin プロパティの識別子 */
	LN_PROPERTY(HorizontalAlignment,	HorizontalAlignmentProperty);	/**< HorizontalAlignment プロパティの識別子 */
	LN_PROPERTY(VerticalAlignment,		VerticalAlignmentProperty);		/**< VerticalAlignment プロパティの識別子 */
	LN_PROPERTY(bool,					IsHitTestProperty);				/**< IsHitTest プロパティの識別子 */

	LN_ROUTED_EVENT(MouseEventArgs,		MouseEnterEvent);				/**< MouseEnter ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseLeaveEvent);				/**< MouseLeave ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseMoveEvent);				/**< MouseMove ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseDownEvent);				/**< MouseDown ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(MouseEventArgs,		MouseUpEvent);					/**< MouseUp ルーティングイベントの識別子 */
	LN_ROUTED_EVENT(RoutedEventArgs,	CanExecuteRoutedCommandEvent);	/**< このイベントは内部用であるため、ユーザーはハンドルすることはできない */
	LN_ROUTED_EVENT(RoutedEventArgs,	ExecuteRoutedCommandEvent);		/**< このイベントは内部用であるため、ユーザーはハンドルすることはできない */

public:
	//-------------------------------------------------------------------------
	/** @name RoutedEvent */
	/** @{ */

	RoutedEventSlot<MouseEventArgs>		MouseEnter;		/**< マウスポインタがこの要素の境界内に入ったときに発生します。*/
	RoutedEventSlot<MouseEventArgs>		MouseLeave;		/**< マウスポインタがこの要素の境界から出たときに発生します。*/
	RoutedEventSlot<MouseEventArgs>		MouseMove;		/**< マウスポインタがこの要素上で移動すると発生します。 */
	RoutedEventSlot<MouseEventArgs>		MouseDown;		/**< マウスポインタがこの要素上にあるときに任意のマウスボタンが押されると発生します。*/
	RoutedEventSlot<MouseEventArgs>		MouseUp;		/**< マウスポインタがこの要素上にあるときに任意のマウスボタンが離されると発生します。*/

	/** @} */
	//-------------------------------------------------------------------------
	/** @name Property */
	/** @{ */

	/** 要素のサイズを設定します。規定値は NaN で、この値は自動サイズ変更が有効であることを示します。*/
	//void SetSize(const SizeF& size) { SetTypedPropertyValue(SizeProperty, size); }

	/** 要素のサイズを取得します。*/
	//const SizeF& GetSize() const { return GetTypedPropertyValue<const SizeF&>(SizeProperty); }

	/** 要素の外側の余白を設定します。*/
	//void SetMargin(const ThicknessF& margin) { SetTypedPropertyValue(MarginProperty, margin); }

	/** 要素の外側の余白を取得します。*/
	//const ThicknessF& GetMargin() const { return GetTypedPropertyValue<const ThicknessF&>(MarginProperty); }

	/** @} */

public:
	/** 論理上の親要素を取得します。*/
	UIElement* GetParent() const { return m_parent; }

	/** 要素の識別名を取得します。*/
	const String& GetKeyName() const { return m_keyName; }

	/** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
	const SizeF& GetDesiredSize() const { return m_desiredSize; }

	/** マウスキャプチャを設定します。*/
	void CaptureMouse();

	/** マウスキャプチャを解除します。*/
	void ReleaseMouseCapture();

	/** この要素内の子ビジュアル要素の数を取得します。*/
	virtual int GetVisualChildrenCount() const;

	/** この要素内の指定したインデックスにある子ビジュアル要素を取得します。*/
	virtual UIElement* GetVisualChild(int index) const;

	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

protected:
	/** この要素のレイアウトの更新が完了した時に呼び出されます。*/
	virtual void OnLayoutUpdated();

	/** 指定した要素をこの要素にビジュアル子要素として追加します。*/
	void AddVisualChild(UIElement* element);

	/** 指定した要素をこの要素のビジュアルツリーから削除します。*/
	void RemoveVisualChild(UIElement* element);
	
	/**
		@brief		この要素を表示するために必要なサイズを計測します。
		@params[in]	constraint	: この要素を配置できる領域の最大サイズ。通常は親要素のサイズが渡されます。
		@return		この要素のレイアウトの際に必要となる最低限のサイズ。この要素のサイズと、全ての子要素のサイズに基づき決定します。
		@details	constraint は、ScrollViewer 等のコンテンツとなった場合は Infinity が渡されることがあります。
	*/
	virtual SizeF MeasureOverride(const SizeF& constraint);

	/**
		@brief		子要素の配置を確定し、この要素の最終サイズを返します。
		@param[in]	finalSize	: 親要素がこの要素に対して割り当てた領域のサイズ。
		@return		要素の最終サイズ。要素の描画時にこのサイズを使用します。
		@details	派生クラスは finalSize よりも大きいサイズを返すと、描画時に見切れが発生します。
	*/
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

protected:
	UIElement(GUIManager* manager);
	virtual ~UIElement();

private:
	RoutedEventSlot<RoutedEventArgs>	CanExecuteRoutedCommand;
	RoutedEventSlot<RoutedEventArgs>	ExecuteRoutedCommand;

	ThicknessF		m_margin;























public:


	// Property

	//
	//void SetWidth(float width) { m_size.Width = width; }

	///// 要素の幅を取得します。
	//float GetWidth() { return m_size.Width; }

	///// 要素の高さを設定します。規定値は NAN で、自動的にサイズを計算します。
	//void SetHeight(float width) { m_size.Width = width; }

	//


	void SetHorizontalAlignment(HorizontalAlignment value) { m_horizontalAlignment = value; }
	HorizontalAlignment GetHorizontalAlignment() const { return m_horizontalAlignment; }
	void SetVerticalAlignment(VerticalAlignment value) { m_verticalAlignment = value; }
	VerticalAlignment GetVerticalAlignment() const { return m_verticalAlignment; }

	///// ヒットテストの有無を設定します。
	//void SetHitTest(bool enabled) { SetPropertyValue(IsHitTestProperty, Variant(enabled)); }

	///// ヒットテストの有無を取得します。
	//bool IsHitTest() const { return GetPropertyValue(IsHitTestProperty).GetBool(); }






public:

	/// この要素のレイアウトの更新が完了した時に発生します。このイベントはルーティングイベントではありません。
	//Event01<RoutedEventArgs*>			LayoutUpdated;

private:

public:

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


	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	


	/// 現在のテンプレートからビジュアルツリーが再構築された後に呼び出されます。
	/// 派生クラスは localResource に対してキー値からリソースを取得することができます。
	virtual void OnApplyTemplate(CombinedLocalResource* localResource) {}
	virtual void OnRender(Graphics::Painter* painter) {}
	virtual bool OnEvent(EventType type, RoutedEventArgs* args);

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
	virtual void OnPropertyChanged(PropertyChangedEventArgs* e);

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
	virtual void PollingTemplateChildCreated(UIElement* newElement) {}

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
	void RaiseEvent(const RoutedEvent* ev, CoreObject* sender, RoutedEventArgs* e)
	{
		e->Sender = sender;
		RaiseEventInternal(ev, e);
	}

protected:
	// 指定したイベントスロットへイベントを直接発行する。ルーティングは行わない
	void EmitEventSlot(RoutedEventSlotBase& slot, RoutedEventArgs* e)
	{
		LN_VERIFY_RETURN(e != NULL);
		slot.Raise(e);
	}

private:
	// 登録されているハンドラと、(Bubbleの場合)論理上の親へイベントを通知する
	void RaiseEventInternal(const RoutedEvent* ev, RoutedEventArgs* e)
	{
		CoreObject::RaiseEventInternal(ev, e);

		// bubble
		if (!e->Handled && m_parent != NULL)
		{
			m_parent->RaiseEventInternal(ev, e);
		}
	}


protected:
	// サブクラスはできるだけ Event にハンドラを登録するのではなく、On～をオーバーライドすることでイベントをハンドリングする。
	// ハンドリングしたら e->Handled を true にする。そして super を呼び出す。こうすることで、RaiseEvent() でのイベント検索や
	// ルーティングを行わないので負荷軽減ができる。
	virtual void OnMouseMove(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseMoveEvent, this, e); } }
	virtual void OnMouseDown(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseDownEvent, this, e); } }
	virtual void OnMouseUp(MouseEventArgs* e) { if (!e->Handled) { RaiseEvent(MouseUpEvent, this, e); } }


	friend class GUIHelper;
	GUIManager*				m_manager;
	String					m_keyName;
	UIElement*				m_parent;				///< 親要素 (論理・ビジュアルは関係ない。RoutedEvent(Bubble) の通知先となる)

	VisualStateInstance*	m_visualStateInstance;

	SizeF					m_desiredSize;			///< MeasureLayout() で決定されるこのコントロールの最終要求サイズ
	RectF					m_finalLocalRect;			///< 描画に使用する最終境界矩形 (グローバル座標系=RootFrame のローカル座標系)
	RectF					m_finalGlobalRect;


	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;

	// 削除予定
	typedef SortedArray<String, RefObject*>	EventDataStore;
	EventDataStore	m_eventDataStore;

	/// あるクラスに含まれる RoutedCommand のリストのリスト。このリストには基底クラスから順に詰まっている
	Array<RoutedCommandTypeContext*>	m_routedCommandTypeContextList;

	// Property
	SizeF				m_size;
	HorizontalAlignment	m_horizontalAlignment;
	VerticalAlignment	m_verticalAlignment;
	bool				m_isHitTest;


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
	Event01<PropertyChangedEventArgs*>		PropertyChangedForTemplateBindings;

	bool			m_childTemplateModified;	///< 子要素のテンプレートを更新するべきか
	bool			m_templateModified;

protected:
	RefPtr<Style>		m_style;
	RefPtr<UIElement>	m_templateChild;		///< TODO: Control に移動するべきかも。テンプレート子要素のルート要素。論理要素だけが持つ。
	UIElement*			m_templateParent;		///< テンプレートのルート要素。例えば ScrollViewer - Grid - ScrollContentPresenter という階層がある場合、ScrollContentPresenter.m_templateParent は ScrollViewer を指す。

private:
	// このリストに追加された UIElement は、OnEvent、Render、ヒットテスト等がこのクラスより自動的に呼ばれる。
	// ただし、レイアウト関係はノータッチ。Measure や Arrange は呼ばれない。
	// 使い方としては、論理要素だろうがビジュアル要素だろうが関係なくまずはこのリストに追加しておく。
	// レイアウトは (メンドくさいが) 個々のコンテナ要素で面倒を見る。
	// ただし、m_templateChild は このクラスでも面倒を見てあげる。でも、凝ったレイアウトが必要な時は
	// オーバーライドする必要がある。
	Array<UIElement*>	m_visualChildren;
};

} // namespace GUI
} // namespace Lumino
