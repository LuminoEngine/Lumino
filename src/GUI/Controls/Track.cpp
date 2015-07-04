
/*
	DecreaseButton と IncreaseButton は WPF では
	<Track>
		<Track.DecreaseButton>
			<RepertButton />
		</Track.DecreaseButton>
		<Track.IncreaseButton>
			<RepertButton />
		</Track.IncreaseButton>
	</Track>
	のようにプロパティに直接入れている。

	実装が難しければ
	PART_DecreaseButton のような名前をつけて検索しても良いかもしれない。
*/

#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Track.h>

namespace Lumino
{

namespace GUI
{

//=============================================================================
// Track
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Track);
LN_UI_ELEMENT_SUBCLASS_IMPL(Track);

PropertyID	Track::ValueProperty(_T("Value"));
PropertyID	Track::MinimumProperty(_T("Minimum"));
PropertyID	Track::MaximumProperty(_T("Maximum"));
PropertyID	Track::OrientationProperty(_T("Orientation"));
PropertyID	Track::DecreaseButtonProperty(_T("DecreaseButton"));
PropertyID	Track::ThumbProperty(_T("Thumb"));
PropertyID	Track::IncreaseButtonProperty(_T("IncreaseButton"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Track::Track(GUIManager* manager)
	: Control(manager)
	, m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_orientation(Orientation::Horizontal)
	, m_decreaseButton()
	, m_thumb()
	, m_increaseButton()
	, m_viewportSize(0.0f)
{	
	// Register property
	LN_DEFINE_PROPERTY		(Track, float, ValueProperty, &Track::SetValue, &Track::GetValue, 0.0f);
	LN_DEFINE_PROPERTY		(Track, float, MinimumProperty, &Track::SetMinimum, &Track::GetMinimum, 0.0f);
	LN_DEFINE_PROPERTY		(Track, float, MaximumProperty, &Track::SetMaximum, &Track::GetMaximum, 1.0f);
	LN_DEFINE_PROPERTY_ENUM	(Track, Orientation, OrientationProperty, &Track::SetOrientation, &Track::GetOrientation, Orientation::Horizontal);
	LN_DEFINE_PROPERTY		(Track, ButtonBase*, DecreaseButtonProperty, &Track::SetDecreaseButton, &Track::GetDecreaseButton, NULL);
	LN_DEFINE_PROPERTY		(Track, Thumb*, ThumbProperty, &Track::SetThumb, &Track::GetThumb, NULL);
	LN_DEFINE_PROPERTY		(Track, ButtonBase*, IncreaseButtonProperty, &Track::SetIncreaseButton, &Track::GetIncreaseButton, NULL);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Track::~Track()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::SetDecreaseButton(ButtonBase* button)
{
	m_decreaseButton = button;
	AddVisualChild(button);		// VisualChild としておくことで OnRender() が呼ばれるようになる
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::SetThumb(Thumb* thumb)
{
	m_thumb = thumb;
	AddVisualChild(thumb);		// VisualChild としておくことで OnRender() が呼ばれるようになる
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::SetIncreaseButton(ButtonBase* button)
{
	m_increaseButton = button;
	AddVisualChild(button);		// VisualChild としておくことで OnRender() が呼ばれるようになる
}

//-----------------------------------------------------------------------------
// Note: ここでは子要素を Measure しない
//-----------------------------------------------------------------------------
void Track::MeasureLayout(const SizeF& availableSize)
{
	m_desiredSize = MeasureOverride(availableSize);
}

//-----------------------------------------------------------------------------
// Note: ここでは子要素を Arrange しない
//-----------------------------------------------------------------------------
void Track::ArrangeLayout(const RectF& finalRect)
{
	SizeF renderSize = ArrangeOverride(finalRect.GetSize());
	m_finalRect.Width = renderSize.Width;
	m_finalRect.Height = renderSize.Height;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Track::MeasureOverride(const SizeF& availableSize)
{
	SizeF desiredSize(0.0, 0.0);

	if (!m_thumb.IsNull())
	{
		m_thumb->MeasureLayout(availableSize);
		desiredSize = m_thumb->GetDesiredSize();	// Thumb はこのサイズを要求している。(ユーザーが直接サイズをしたいした場合等)
	}

	if (!Math::IsNaN(m_viewportSize))
	{
		if (m_orientation == Orientation::Vertical)
			desiredSize.Height = 0.0;	// 縦方向は後の計算に任せる
		else
			desiredSize.Width = 0.0;	// 横方向は後の計算に任せる
	}

	return desiredSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Track::ArrangeOverride(const SizeF& finalSize)
{
	float decreaseButtonLength;
	float thumbLength;
	float increaseButtonLength;
	float trackLength = (m_orientation == Orientation::Vertical) ? finalSize.Height : finalSize.Width;

	if (Math::IsNaN(m_viewportSize))
	{
		// ビューサイズが関係ない場合の計算。つまり、Slider コントロール用
		//CalcSliderComponentsSize
	}
	else
	{
		// ビューサイズが関係ない場合の計算。つまり、Slider コントロール用
		CalcScrollBarComponentsSize(trackLength, m_viewportSize, &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}

	if (m_orientation == Orientation::Horizontal)
	{
		RectF rect(0.0f, 0.0f, 0.0f, finalSize.Height);

		// デクリメントボタン
		if (m_decreaseButton != NULL)
		{
			rect.X = 0.0f;
			rect.Width = decreaseButtonLength;
			m_decreaseButton->ArrangeLayout(rect);
		}
		// つまみ
		if (m_thumb != NULL)
		{
			rect.X = decreaseButtonLength;
			rect.Width = thumbLength;
			m_thumb->ArrangeLayout(rect);
		}
		// インクリメントボタン
		if (m_increaseButton != NULL)
		{
			rect.X = decreaseButtonLength + thumbLength;
			rect.Width = increaseButtonLength;
			m_increaseButton->ArrangeLayout(rect);
		}
	}

	return finalSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::CoerceLength(float& componentLength, float trackLength)
{
	if (componentLength < 0)
	{
		componentLength = 0.0f;
	}
	else if (componentLength > trackLength || Math::IsNaN(componentLength))
	{
		componentLength = trackLength;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Track::CalcScrollBarComponentsSize(
	float trackLength,
	float viewportSize, 
	float* outDecreaseButtonLength, 
	float* outThumbLength,
	float* outIncreaseButtonLength)
{
	/*
	 ViewportSize に使用される単位は、コンテンツの長さを記述するために使用される単位と同じです。
	 https://msdn.microsoft.com/ja-jp/library/system.windows.controls.primitives.scrollbar.viewportsize%28v=vs.110%29.aspx
	
	https://msdn.microsoft.com/ja-jp/library/system.windows.controls.primitives.track.maximum%28v=vs.110%29.aspx
	*/

	float min = m_minimum;
	float range = std::max(0.0f, m_maximum - min);
	float offset = std::min(range, m_value - min);			// m_value の位置
	float extent = std::max(0.0f, range) + viewportSize;	// コンテンツ全体のサイズ

	//float trackLength = finalLength;
	float thumbMinLength = 16.0f;

	// Thumb サイズを計算する
	float thumbLength = trackLength * viewportSize / extent;	// コンテンツ全体の内、どの部分を表示しているのか、その割合で Thumb の長さを作る
	CoerceLength(thumbLength, trackLength);
	thumbLength = std::max(thumbMinLength, thumbLength);

	// 残りの部分のサイズ
	float remainingTrackLength = trackLength - thumbLength;

	// DecreaseButton
	float decreaseButtonLength = remainingTrackLength * offset / range;
	CoerceLength(decreaseButtonLength, remainingTrackLength);

	// IncreaseButton
	float increaseButtonLength = remainingTrackLength - decreaseButtonLength;
	CoerceLength(increaseButtonLength, remainingTrackLength);

	*outDecreaseButtonLength = decreaseButtonLength;
	*outThumbLength = thumbLength;
	*outIncreaseButtonLength = increaseButtonLength;
}

} // namespace GUI
} // namespace Lumino

