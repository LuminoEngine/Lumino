
#pragma once
#include "UIRangeBase.h"

LN_NAMESPACE_BEGIN
class UITrack;

/**
	@brief		任意の範囲の数値から1つをユーザーに選択させるコントロールです。
	@details
				## VisualState
				| Name           | Group       | Description                          |
				|----------------|-------------|--------------------------------------|
				| Horizontal     | Orientation | Orientation プロパティの値を示します。 |
				| Vertical       | Orientation | Orientation プロパティの値を示します。 |

				## SubControl
				| Name           | Description |
				|----------------|-------------|
				| Track          |             |
				| Thumb          |             |
				| DecreaseButton |             |
				| IncreaseButton |             |
*/
class UISlider
	: public UIRangeBase
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		UISlider を作成します。
	*/
	static RefPtr<UISlider> create();

	/**
		@brief		UISlider を作成します。
		@param[in]	value	: 初期値
		@param[in]	minimum	: 最小値
		@param[in]	maximum	: 最大値
	*/
	static RefPtr<UISlider> create(float value, float minimum = 0.0f, float maximum = 1.0f);

public:

	/** Slider の方向を指定します。*/
	void setOrientation(Orientation orientation);

	/** Slider の方向を取得します。規定値は Orientation::Horizontal です。*/
	Orientation getOrientation() const;

protected:
	// UIElement interface
	virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

	// UIRangeBase interface
	virtual void onValueChanged(float oldValue, float newValue) override;
	virtual void onMinimumChanged(float oldMinimum, float newMinimum) override;
	virtual void onMaximumChanged(float oldMaximum, float newMaximum) override;

LN_CONSTRUCT_ACCESS:
	UISlider();
	virtual ~UISlider();
	void initialize();
	void initialize(float value, float minimum, float maximum);

private:
	void updateValue(float value);

	RefPtr<UITrack>		m_track;
	float				m_dragStartValue;
};

LN_NAMESPACE_END
