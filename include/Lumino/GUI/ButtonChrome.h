
#pragma once
#include "Common.h"
#include "Decorator.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		Button コントロールの外観です。
*/
class ButtonChrome
	: public Decorator
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ButtonChrome);
public:
	LN_PROPERTY(bool,	IsMouseOverProperty);
	LN_PROPERTY(float,	FrameWidthProperty);

public:
	static ButtonChromePtr Create();

	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

public:
	// Property
	void SetMouseOver(bool value) { SetTypedPropertyValue<bool>(IsMouseOverProperty, value); }
	bool IsMouseOver() const { return GetTypedPropertyValue<bool>(IsMouseOverProperty); }

	/// ボタンイメージの外枠の幅を設定します。
	void SetFrameWidth(float value) { SetTypedPropertyValue<float>(FrameWidthProperty, value); }

	/// ボタンイメージの外枠の幅を取得します。
	float GetFrameWidth() const { return GetTypedPropertyValue<float>(FrameWidthProperty); }

public:
	// override CoreObject
	//virtual void SetPropertyValue(const Property* prop, const Variant& value);
	//virtual Variant GetPropertyValue(const Property* prop) const;

protected:
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void OnRender(Graphics::Painter* painter);

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	float							m_frameWidth;	///< 枠の幅 (描画で使用するためパフォーマンスを考慮し、GetValue() ではなくメンバに持っておく)
	bool							m_isMouseOver;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
