#pragma once
#include "UIControl.hpp"
#include "UILayoutPanel.hpp"

namespace ln {
class UIThumb;



class UISliderField
	: public UIControl
{
public:
	/** スクロール位置に対する値を設定します。*/
	void setValue(float value);

	/** スクロール位置に対する値を取得します。(default: 0.0) */
	float value() const;

	/** 指定可能な最小値を設定します。*/
	void setMinimum(float value);

	/** 指定可能な最小値を取得します。(default: 0.0) */
	float minimum() const;

	/** 指定可能な最大値を設定します。*/
	void setMaximum(float value);

	/** 指定可能な最大値を取得します。(default: 1.0) */
	float maximum() const;

protected:
	/** 値が変更されたときに呼び出されます。*/
	virtual void onValueChanged(float oldValue, float newValue);

	/** 指定可能な最小値が変更されたときに呼び出されます。*/
	virtual void onMinimumChanged(float oldMinimum, float newMinimum);

	/** 指定可能な最大値が変更されたときに呼び出されます。*/
	virtual void onMaximumChanged(float oldMaximum, float newMaximum);
	
	virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual void onRender(UIRenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UISliderField();
	void init();

private:
	float m_value;
	float m_minimum;
	float m_maximum;
	bool m_dragging;
};


} // namespace ln

