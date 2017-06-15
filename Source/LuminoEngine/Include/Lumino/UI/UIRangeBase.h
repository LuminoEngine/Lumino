
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief	特定の範囲内の値を持つ要素を表します。
*/
class UIRangeBase
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/** スクロール位置に対する値を設定します。*/
	void setValue(float value);

	/** スクロール位置に対する値を取得します。規定値は 0 です。*/
	float getValue() const;

	/** 指定可能な最小値を設定します。*/
	void setMinimum(float value);

	/** 指定可能な最小値を取得します。規定値は 0 です。*/
	float getMinimum() const;

	/** 指定可能な最大値を設定します。*/
	void setMaximum(float value);

	/** 指定可能な最大値を取得します。規定値は 1 です。*/
	float getMaximum() const;

protected:

	/** 値が変更されたときに呼び出されます。*/
	virtual void onValueChanged(float oldValue, float newValue);

	/** 指定可能な最小値が変更されたときに呼び出されます。*/
	virtual void onMinimumChanged(float oldMinimum, float newMinimum);

	/** 指定可能な最大値が変更されたときに呼び出されます。*/
	virtual void onMaximumChanged(float oldMaximum, float newMaximum);

LN_CONSTRUCT_ACCESS:
	UIRangeBase();
	virtual ~UIRangeBase();
	void initialize();

private:
	float	m_value;
	float	m_minimum;
	float	m_maximum;
};

LN_NAMESPACE_END
