
#pragma once
#include "UIButton.h"

LN_NAMESPACE_BEGIN
class UITrack;

/**
	@brief		
*/
class UIRangeBase
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:

	/** スクロール位置に対する値を設定します。*/
	void SetValue(float value);

	/** スクロール位置に対する値を取得します。規定値は 0 です。*/
	float GetValue() const;

	/** 指定可能な最小値を設定します。*/
	void SetMinimum(float value);

	/** 指定可能な最小値を取得します。規定値は 0 です。*/
	float GetMinimum() const;

	/** 指定可能な最大値を設定します。*/
	void SetMaximum(float value);

	/** 指定可能な最大値を取得します。規定値は 1 です。*/
	float GetMaximum() const;

protected:

	/** 値が変更されたときに呼び出されます。*/
	virtual void OnValueChanged(float oldValue, float newValue);

	/** 指定可能な最大値が変更されたときに呼び出されます。*/
	virtual void OnMaximumChanged(float oldMaximum, float newMaximum);

	/** 指定可能な最小値が変更されたときに呼び出されます。*/
	virtual void OnMinimumChanged(float oldMinimum, float newMinimum);

LN_CONSTRUCT_ACCESS:
	UIRangeBase();
	virtual ~UIRangeBase();
	void Initialize();

private:
	float	m_value;
	float	m_minimum;
	float	m_maximum;
};

/**
	@brief		
*/
class UISlider
	: public UIRangeBase
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UISlider> Create();

	/** Slider の方向を指定します。*/
	void SetOrientation(Orientation orientation);

	/** Slider の方向を取得します。規定値は Orientation::Horizontal です。*/
	Orientation GetOrientation() const;

protected:
	// UIElement interface
	virtual void OnRoutedEvent(UIEventArgs* e) override;
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UISlider();
	virtual ~UISlider();
	void Initialize();

private:
	void UpdateValue(float value);

	RefPtr<UITrack>		m_track;
	float				m_dragStartValue;
};


LN_NAMESPACE_END
