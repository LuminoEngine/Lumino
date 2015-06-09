
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include "Common.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "BitmapBrush.h"	// for button

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

	/// ヒットテストの有無を設定します。
	void SetHitTest(bool enabled) { SetValue(IsHitTestProperty, Variant(enabled)); }

	/// ヒットテストの有無を取得します。
	bool IsHitTest() const { return GetValue(IsHitTestProperty).GetBool(); }

	

	void AddMouseMoveHandler(const Delegate02<CoreObject*, MouseEventArgs*>& handler) { AddHandler(MouseMoveEvent, handler); }
	void RemoveMouseMoveHandler(const Delegate02<CoreObject*, MouseEventArgs*>& handler) { RemoveHandler(MouseMoveEvent, handler); }

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
	friend class ContentControl;
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

public:
	template<typename TArgs>
	void AddHandler(const String& eventName, const Delegate02<CoreObject*, TArgs*>& handler)
	{
		RefObject* ev;
		if (m_eventDataStore.TryGetValue(eventName, &ev)) {
			static_cast<Event02<CoreObject*, TArgs*>*>(ev)->AddHandler(handler);
		}
		else {
			LN_THROW(0, ArgumentException);
		}
	}
	template<typename TArgs>
	void RemoveHandler(const String& eventName, const Delegate02<CoreObject*, TArgs*>& handler)
	{
		RefObject* ev;
		if (m_eventDataStore.TryGetValue(eventName, &ev)) {
			static_cast<Event02<CoreObject*, TArgs*>*>(ev)->RemoveHandler(handler);
		}
		else {
			LN_THROW(0, ArgumentException);
		}
	}
	template<typename TArgs>
	void RaiseEvent(const String& eventName, CoreObject* sender, TArgs* args)
	{
		RefObject* ev;
		if (m_eventDataStore.TryGetValue(eventName, &ev)) {
			args->HandlerOwner = this;
			static_cast<Event02<CoreObject*, TArgs*>*>(ev)->Raise(sender, args);
		}
		else {
			LN_THROW(0, ArgumentException);
		}
	}



protected:


	GUIManager*				m_manager;
	UIElement*				m_parent;				///< 論理ツリー上の親要素

	VisualStateInstance*	m_visualStateInstance;

	SizeF					m_desiredSize;			///< MeasureLayout() で決定されるこのコントロールの最終要求サイズ
	RectF					m_finalRect;			///< 描画に使用する最終境界矩形 (グローバル座標系=RootPane のローカル座標系)

	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;


	typedef SortedArray<String, RefObject*>	EventDataStore;
	EventDataStore	m_eventDataStore;
	//Event02<CoreObject*, MouseEventArgs*> m_eventMouseMove;



	// Property
	SizeF	m_size;

private:

	void TemplateBindingSource_PropertyChanged(CoreObject* sender, PropertyChangedEventArgs* e);


	struct TemplateBindingInfo
	{
		Property*	ThisProp;
		String		SourcePropPath;
	};

	typedef ArrayList<TemplateBindingInfo>	TemplateBindingInfoList;
	TemplateBindingInfoList		m_templateBindingInfoList;

	UIElement*		m_rootLogicalParent;	///< テンプレートを適用した要素。TemplateBinding のソースオブジェクト。これが NULL でなければ、this は VisualTree 要素である。
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
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);

	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }


protected:
	// override UIElement
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);;
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

	virtual void Render();

protected:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	//virtual void ApplyTemplate(CombinedLocalResource* parent);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
	virtual void OnRender();
	virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	// override UIElement
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

private:
	Variant		m_content;		
	UIElement*	m_childElement;	///< m_content が UIElement であればそれを指す
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
