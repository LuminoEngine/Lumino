
#pragma once
#include "Decorator.h"

namespace Lumino
{
namespace GUI
{

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
	virtual void OnRender(Graphics::Painter* painter);

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	float							m_frameWidth;	///< 枠の幅 (描画で使用するためパフォーマンスを考慮し、GetValue() ではなくメンバに持っておく)
	bool							m_isMouseOver;
};

} // namespace GUI
} // namespace Lumino
