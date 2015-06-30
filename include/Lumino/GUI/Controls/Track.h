
#pragma once
#include "../UIElement.h"
#include "ButtonBase.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class Track
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Track);
public:
	static PropertyID	ValueProperty;			///< スクロール位置に対する値
	static PropertyID	MaximumProperty;		///< 指定可能な最大値
	static PropertyID	MinimumProperty;		///< 指定可能な最小値
	static PropertyID	OrientationProperty;	///< Track の方向
	static PropertyID	DecreaseButtonProperty;	///< 値を減少させる Button コントロール
	static PropertyID	IncreaseButtonProperty;	///< 値を増加させる Button コントロール

public:
	Track(GUIManager* manager);
	virtual ~Track();

	/// スクロール位置に対する値を設定します。
	void SetValue(double value) { m_value = value; }

	/// スクロール位置に対する値を取得します。規定値は 0 です。
	double GetValue() const { return m_value; }

	/// 指定可能な最大値を設定します。
	void SetMaximum(double value) { m_maximum = value; }

	/// 指定可能な最大値を取得します。規定値は 1 です。
	double GetMaximum() const { return m_maximum; }

	/// 指定可能な最小値を設定します。
	void SetMinimum(double value) { m_minimum = value; }

	/// 指定可能な最小値を取得します。規定値は 0 です。
	double GetMinimum() const { return m_minimum; }

	/// Track の方向を指定します。
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }

	/// Track の方向を取得します。規定値は Orientation::Horizontal です。
	Orientation GetOrientation() const { return m_orientation; }

	/// 値を減少させる Button コントロールを設定します。
	void SetDecreaseButton(ButtonBase* button);

	/// 値を減少させる Button コントロールを取得します。
	ButtonBase* GetDecreaseButton() const{ return m_decreaseButton; }

	/// 値を増加させる Button コントロールを設定します。
	void SetIncreaseButton(ButtonBase* button);

	/// 値を増加させる Button コントロールを取得します。
	ButtonBase* GetIncreaseButton() const { return m_increaseButton; }

protected:
	virtual Size ArrangeOverride(Size finalSize);
	virtual Size MeasureOverride(Size availableSize);

private:
	double		m_value;
	double		m_maximum;
	double		m_minimum;
	Orientation	m_orientation;
	RefPtr<ButtonBase>	m_decreaseButton;
	RefPtr<ButtonBase>	m_increaseButton;
};


} // namespace GUI
} // namespace Lumino
