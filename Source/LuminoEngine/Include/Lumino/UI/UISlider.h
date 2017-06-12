
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
	static RefPtr<UISlider> Create();

	/**
		@brief		UISlider を作成します。
		@param[in]	value	: 初期値
		@param[in]	minimum	: 最小値
		@param[in]	maximum	: 最大値
	*/
	static RefPtr<UISlider> Create(float value, float minimum = 0.0f, float maximum = 1.0f);

public:

	/** Slider の方向を指定します。*/
	void SetOrientation(Orientation orientation);

	/** Slider の方向を取得します。規定値は Orientation::Horizontal です。*/
	Orientation GetOrientation() const;

protected:
	// UIElement interface
	virtual void OnRoutedEvent(UIEventArgs* e) override;
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

	// UIRangeBase interface
	virtual void OnValueChanged(float oldValue, float newValue) override;
	virtual void OnMinimumChanged(float oldMinimum, float newMinimum) override;
	virtual void OnMaximumChanged(float oldMaximum, float newMaximum) override;

LN_CONSTRUCT_ACCESS:
	UISlider();
	virtual ~UISlider();
	void initialize();
	void initialize(float value, float minimum, float maximum);

private:
	void UpdateValue(float value);

	RefPtr<UITrack>		m_track;
	float				m_dragStartValue;
};

LN_NAMESPACE_END
