
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include "Common.h"
#include "EventArgs.h"
#include "DependencyObject.h"
#include "BitmapBrush.h"	// for button

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

public:
	UIElement(GUIManager* manager);
	virtual ~UIElement();

	//
	//void SetWidth(float width) { m_size.Width = width; }

	///// 要素の幅を取得します。
	//float GetWidth() { return m_size.Width; }

	///// 要素の高さを設定します。規定値は NAN で、自動的にサイズを計算します。
	//void SetHeight(float width) { m_size.Width = width; }

	//

	/// 要素のサイズを設定します。規定値は NAN で、自動的にサイズを計算します。
	void SetSize(const SizeF& size) { SetValue(SizeProperty, size); }	// TODO: レイアウト更新中は頻繁にアクセスするのでできれば メンバ変数にしたい・・・
	
	/// 要素のサイズを取得します。
	const SizeF& GetSize() const { return GetValue(SizeProperty).GetSizeF(); }	// TODO: 危ない。参照で返すとスタックの Variant で消える


	/// ヒットテストの有無を設定します。
	void SetHitTest(bool enabled) { SetValue(IsHitTestProperty, Variant(enabled)); }

	/// ヒットテストの有無を取得します。
	bool IsHitTest() const { return GetValue(IsHitTestProperty).GetBool(); }

	

	/// (サイズの自動計算が有効になっている要素に対しては呼び出しても効果はありません)
	void UpdateLayout();
	virtual void Render();

	virtual void ApplyTemplate(CombinedLocalResource* parent);

public:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);

	virtual void OnApplyTemplate() {}
	virtual void OnRender() {}
	virtual bool OnEvent(EventType type, EventArgs* args) { return false; }

	// IAddChild
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:

	GUIManager*			m_manager;
	SizeF				m_desiredSize;			///< MeasureLayout() で決定されるこのコントロールの最終要求サイズ
	RectF				m_finalRect;			///< 描画に使用する最終境界矩形 (グローバル座標系=RootPane のローカル座標系)

	ResourceDictionary*		m_localResource;
	CombinedLocalResource*	m_combinedLocalResource;
};

/**
	@brief		1 つの子要素の周囲を装飾する UI 要素の基本クラスです。
*/
class Decorator
	: public UIElement
{
public:
	Decorator(GUIManager* manager);
	virtual ~Decorator();

public:
	void SetChild(UIElement* element);
	UIElement* GetChild() const;

public:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplate(CombinedLocalResource* parent);

	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

private:
	RefPtr<UIElement>	m_child;
};

/**
	@brief		Button コントロールの外観です。
*/
class ButtonChrome
	: public Decorator
{
public:
	static const String TypeName;	///< "ButtonChrome"

public:
	static CoreObject* CreateInstance(GUIManager* manager) { return LN_NEW ButtonChrome(manager); }

	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

protected:
	virtual void OnApplyTemplate();
	virtual void OnRender();

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	int								m_bgMargin;	///< 背景イメージを描画する時に縮小するピクセル数
};


/**
	@brief	ContentControl のコンテンツを表示します。
*/
class ContentPresenter
	: public UIElement
{
public:
	static const String TypeName;	///< "ContentPresenter"

	static CoreObject* CreateInstance(GUIManager* manager) { return LN_NEW ContentPresenter(manager); }

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
public:
	static const String ControlTemplateTypeName;


public:
	Control(GUIManager* manager);
	virtual ~Control();


	virtual void ApplyTemplate(CombinedLocalResource* parent);

	virtual bool ApplyTemplateInternal();
	bool ApplyTemplateInternalMain(const String& typeName);
};

/**
	@brief		1 つのコンテンツを持つコントロールを表します。
*/
class ContentControl
	: public Control
{
public:
	ContentControl(GUIManager* manager);
	virtual ~ContentControl();


	/// このコントロールのコンテンツを設定します。
	void SetContent(Variant value);

	virtual void Render();

protected:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplate(CombinedLocalResource* parent);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
	virtual void OnRender();
	virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

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
public:
	static const String ControlTemplateTypeName;	///< "Button"

	Button(GUIManager* manager);
	virtual ~Button();

public:


	//virtual void Render();

protected:
	virtual void OnClick();
	virtual bool OnEvent(EventType type, EventArgs* args);
	virtual void OnRender();

	virtual bool ApplyTemplateInternal()
	{
		if (ApplyTemplateInternalMain(ControlTemplateTypeName)) { return true; }
		return ContentControl::ApplyTemplateInternal();	// base
	}

private:
	RefPtr<ButtonChrome>	m_chrome;	// TODO: 仮。ちゃんとテーマから読みだす
};

} // namespace GUI
} // namespace Lumino
