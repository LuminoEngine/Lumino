
#pragma once
#include "../UIElement.h"
#include "ButtonBase.h"
#include "Thumb.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
	@note	Value はビューサイズ (ピクセル単位)
*/
class Track
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Track);
public:
	LN_PROPERTY(float,			ValueProperty);				///< Value プロパティの識別子
	LN_PROPERTY(float,			MinimumProperty);			///< Minimum プロパティの識別子
	LN_PROPERTY(float,			MaximumProperty);			///< Maximum プロパティの識別子
	LN_PROPERTY(Orientation,	OrientationProperty);		///< Orientation プロパティの識別子
	LN_PROPERTY(ButtonBase*,	DecreaseButtonProperty);	///< DecreaseButton プロパティの識別子
	LN_PROPERTY(Thumb*,			ThumbProperty);				///< Thumb プロパティの識別子
	LN_PROPERTY(ButtonBase*,	IncreaseButtonProperty);	///< IncreaseButton プロパティの識別子

public:
	static TrackPtr Create();

	Track(GUIManager* manager);
	virtual ~Track();

	/// スクロール位置に対する値を設定します。
	void SetValue(float value) { m_value = value; }

	/// スクロール位置に対する値を取得します。規定値は 0 です。
	float GetValue() const { return m_value; }

	/// 指定可能な最小値を設定します。
	void SetMinimum(float value) { m_minimum = value; }

	/// 指定可能な最小値を取得します。規定値は 0 です。
	float GetMinimum() const { return m_minimum; }

	/// 指定可能な最大値を設定します。
	void SetMaximum(float value) { m_maximum = value; }

	/// 指定可能な最大値を取得します。規定値は 1 です。
	float GetMaximum() const { return m_maximum; }

	/// Track の方向を指定します。
	void SetOrientation(Orientation orientation) { m_orientation = orientation; }

	/// Track の方向を取得します。規定値は Orientation::Horizontal です。
	Orientation GetOrientation() const { return m_orientation; }

	/// 値を減少させる Button コントロールを設定します。
	void SetDecreaseButton(ButtonBase* button);

	/// 値を減少させる Button コントロールを取得します。
	ButtonBase* GetDecreaseButton() const{ return m_decreaseButton; }

	/// 値を操作する Thumb コントロールを取得します。
	Thumb* GetThumb() const { return m_thumb; }

	/// 値を操作する Thumb コントロールを設定します。
	void SetThumb(Thumb* thumb);

	/// 値を増加させる Button コントロールを設定します。
	void SetIncreaseButton(ButtonBase* button);

	/// 値を増加させる Button コントロールを取得します。
	ButtonBase* GetIncreaseButton() const { return m_increaseButton; }

	/// スクロール可能なコンテンツの中で表示される部分のサイズを設定します。
	void SetViewportSize(float size) { m_viewportSize = size; }

	/// スクロール可能なコンテンツの中で表示される部分のサイズを取得します。
	float GetViewportSize() const { return m_viewportSize; }

public:
	float ValueFromDistance(float horizontal, float vertical);

protected:
	//virtual void ApplyTemplateHierarchy(/*CombinedLocalResource* parent*/);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);
	virtual SizeF MeasureOverride(const SizeF& availableSize);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//virtual void Render();

private:
	void UpdateComponent(Control* oldValue, Control* newValue);
	void CoerceLength(float& componentLength, float trackLength);
	void CalcSliderComponentsSize(float trackLength, float* outDecreaseButtonLength, float* outThumbLength, float* outIncreaseButtonLength);
	void CalcScrollBarComponentsSize(float trackLength, float viewportSize, float* outDecreaseButtonLength, float* outThumbLength, float* outIncreaseButtonLength);

	void OnDecreaseButtonPropertyChanged(PropertyChangedEventArgs* e);
	void OnThumbPropertyChanged(PropertyChangedEventArgs* e);
	void OnIncreaseButtonPropertyChanged(PropertyChangedEventArgs* e);

#if 0
	void Handler_Thumb_DragStarted(DragEventArgs* e);
	void Handler_Thumb_DragDelta(DragEventArgs* e);
#endif

private:
	float				m_value;
	float				m_minimum;
	float				m_maximum;
	Orientation			m_orientation;
	RefPtr<ButtonBase>	m_decreaseButton;
	RefPtr<Thumb>		m_thumb;
	RefPtr<ButtonBase>	m_increaseButton;
	float				m_viewportSize;
	float				m_density;
#if 0
	float				m_dragStartValue;	///< ドラッグ開始時の m_value
#endif

	//Delegate01
};


} // namespace GUI
} // namespace Lumino
