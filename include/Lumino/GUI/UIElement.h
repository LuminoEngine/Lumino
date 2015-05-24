
#pragma once

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
	UIElement(GUIManager* manager);
	virtual ~UIElement();

	virtual void Render();

protected:
	virtual void OnRender() {}
	virtual void AddChild(const Variant& value) { LN_THROW(0, InvalidOperationException); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	GUIManager*	m_manager;
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
