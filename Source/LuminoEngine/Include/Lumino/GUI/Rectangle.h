
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		Shape 要素の基本クラスです。
*/
class Shape
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();	// TODO: LN_UI_ELEMENT_SUBCLASS_DECL にまとめたい
	LN_UI_ELEMENT_SUBCLASS_DECL(Shape);
public:
	LN_PROPERTY(Brush*, FillBrushProperty);	/**< FillBrush プロパティの識別子 */

public:

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/** 図形内部の塗りつぶしに使用するブラシを設定します。*/
	void SetFillBrush(Brush* size) { SetTypedPropertyValue<Brush*>(SizeProperty, size); }

	/** 図形内部の塗りつぶしに使用するブラシを取得します。*/
	Brush* GetFillBrush() const { return GetTypedPropertyValue<Brush*>(SizeProperty); }

	/** @} */

protected:
	Shape(GUIManagerImpl* manager);
	virtual ~Shape();

protected:
	RefPtr<Brush>	m_fillBrush;
};

/**
	@brief		四角形を描画する GUI 要素です。
*/
class Rectangle
	: public Shape
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Rectangle);

public:
	/**
		@brief	Rectangle を作成します。
		@return	作成されたインスタンスのポインタ。使い終えたら Release() を呼び出して、参照を解放してください。	
	*/
	Rectangle* Create();

protected:
	Rectangle(GUIManagerImpl* manager);
	virtual ~Rectangle();
	virtual void OnRender(RenderingContext* painter);

public:
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
