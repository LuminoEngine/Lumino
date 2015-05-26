
#pragma once

#include <Lumino/Base/Array.h>
#include <Lumino/Graphics/Painter.h>
#include "Common.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class UIElement
	: public CoreObject
{
public:
	static const String	SizeProperty;

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
	void SetSize(const SizeF& size) { SetValue(SizeProperty, size); }
	
	/// 要素のサイズを取得します。
	const SizeF& GetSize() const { return GetValue(SizeProperty).GetSizeF(); }

	/// この UIElement にプロパティを登録します。
	void RegisterProperty(const String& propertyName, const Variant& defaultValue);

	/// プロパティの値を設定します。
	void SetValue(const String& propertyName, const Variant& value);

	/// プロパティの値を取得します。
	Variant GetValue(const String& propertyName) const;

	virtual void Render();

protected:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
	virtual void OnRender() {}
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	typedef SortedArray<String, Variant>	PropertyDataStore;

	GUIManager*			m_manager;
	PropertyDataStore	m_propertyDataStore;
	SizeF				m_desiredSize;			///< MeasureLayout() で決定されるこのコントロールの最終要求サイズ
	RectF				m_finalRect;			///< 描画に使用する最終境界矩形
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
	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

protected:
	virtual void OnRender();

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	int								m_bgMargin;	///< 背景イメージを描画する時に縮小するピクセル数
};

/**
	@brief		ユーザー操作によって特定の動作を行う UI 要素の基本クラスです。
*/
class Control
	: public UIElement
{
public:
	Control(GUIManager* manager);
	virtual ~Control();
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
	virtual void OnRender();

private:
	Variant		m_content;		
	UIElement*	m_childElement;	///< m_content が UIElement であればそれを指す
};

/**
	@brief		ルート要素。
*/
class Workbench
	: public ContentControl
{
public:
	Workbench(GUIManager* manager);
	virtual ~Workbench();

	//virtual void Render();

protected:
	//virtual void OnRender() {}

private:
};

/**
	@brief		
*/
class Button
	: public ContentControl
{
public:
	Button(GUIManager* manager);
	virtual ~Button();

public:


	//virtual void Render();

protected:
	virtual void OnRender();

private:
	RefPtr<ButtonChrome>	m_chrome;	// TODO: 仮。ちゃんとテーマから読みだす
};

} // namespace GUI
} // namespace Lumino
