
#include "Internal.h"
#include <Lumino/UI/UIScrollViewer.h>
#include "UIManager.h"
#include "EventArgsPool.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIDragDeltaEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIDragDeltaEventArgs, UIEventArgs)

//==============================================================================
// UIThumb
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIThumb, UIElement)
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragStartedEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragDeltaEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragCompletedEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragCanceledEvent);

//------------------------------------------------------------------------------
RefPtr<UIThumb> UIThumb::Create()
{
	auto ptr = RefPtr<UIThumb>::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIThumb::UIThumb()
	: m_lastScreenPosition()
	, m_isDragging(false)
{
}

//------------------------------------------------------------------------------
UIThumb::~UIThumb()
{
}

//------------------------------------------------------------------------------
void UIThumb::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
}

//------------------------------------------------------------------------------
void UIThumb::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	if (ev == UIElement::MouseDownEvent)
	{
		if (!m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetParent());

			m_lastScreenPosition = pos;
			m_isDragging = true;
			CaptureMouse();

			// ドラッグ開始イベント
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragStarted(args);

			e->handled = true;
			return;
		}
	}
	else if (ev == UIElement::MouseUpEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetParent());

			m_isDragging = false;
			ReleaseMouseCapture();

			// ドラッグ終了イベント
			// TODO: template 化
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragCompleted(args);

			e->handled = true;
			return;
		}
	}
	else if (ev == UIElement::MouseMoveEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetParent());

			// ドラッグ中イベント
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragDelta(args);

			e->handled = true;
			return;
		}
	}

	UIElement::OnRoutedEvent(ev, e);
}

//==============================================================================
// UITrack
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITrack, UIElement)

//------------------------------------------------------------------------------
RefPtr<UITrack> UITrack::Create()
{
	auto ptr = RefPtr<UITrack>::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UITrack::UITrack()
	: m_orientation(Orientation::Horizontal)
	, m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_density(1.0f)
	, m_viewportSize(0.0f)
	, m_pageUpButton(nullptr)
	, m_thumb(nullptr)
	, m_pageDownButton(nullptr)
{
}

//------------------------------------------------------------------------------
UITrack::~UITrack()
{
}

//------------------------------------------------------------------------------
void UITrack::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);

	m_pageUpButton = NewObject<UIButton>(manager);
	m_thumb = NewObject<UIThumb>(manager);
	m_pageDownButton = NewObject<UIButton>(manager);

	AddVisualChild(m_pageUpButton);
	AddVisualChild(m_thumb);
	AddVisualChild(m_pageDownButton);
}

//------------------------------------------------------------------------------
float UITrack::ValueFromDistance(float horizontal, float vertical)
{
	float scale = 1;//IsDirectionReversed ? -1 : 1;

	if (m_orientation == Orientation::Horizontal)
	{
		return scale * horizontal * m_density;
	}
	else
	{
		return scale * vertical * m_density;
	}
}

//------------------------------------------------------------------------------
Size UITrack::MeasureOverride(const Size& constraint)
{
	return UIElement::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UITrack::ArrangeOverride(const Size& finalSize)
{
	float decreaseButtonLength;
	float thumbLength;
	float increaseButtonLength;
	float trackLength = (m_orientation == Orientation::Vertical) ? finalSize.height : finalSize.width;

	if (Math::IsNaN(m_viewportSize))
	{
		// ビューサイズが関係ない場合の計算。つまり、Slider コントロール用
		//CalcSliderComponentsSize
		LN_NOTIMPLEMENTED();
	}
	else
	{
		CalcScrollBarComponentsSize(trackLength, m_viewportSize, &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}

	if (m_orientation == Orientation::Horizontal)
	{
		RectF rect(0.0f, 0.0f, 0.0f, finalSize.height);

		// PageUp Button
		if (m_pageUpButton != nullptr)
		{
			rect.x = 0.0f;
			rect.width = decreaseButtonLength;
			m_pageUpButton->ArrangeLayout(rect);
		}
		// Thumb
		if (m_thumb != nullptr)
		{
			rect.x = decreaseButtonLength;
			rect.width = thumbLength;
			m_thumb->ArrangeLayout(rect);
		}
		// PageDown Button
		if (m_pageDownButton != nullptr)
		{
			rect.x = decreaseButtonLength + thumbLength;
			rect.width = increaseButtonLength;
			m_pageDownButton->ArrangeLayout(rect);
		}
	}
	else
	{
		RectF rect(0.0f, 0.0f, finalSize.width, 0.0f);

		// PageUp Button
		if (m_pageUpButton != nullptr)
		{
			rect.y = 0.0f;
			rect.height = decreaseButtonLength;
			m_pageUpButton->ArrangeLayout(rect);
		}
		// Thumb
		if (m_thumb != nullptr)
		{
			rect.y = decreaseButtonLength;
			rect.height = thumbLength;
			m_thumb->ArrangeLayout(rect);
		}
		// PageDown Button
		if (m_pageDownButton != nullptr)
		{
			rect.y = decreaseButtonLength + thumbLength;
			rect.height = increaseButtonLength;
			m_pageDownButton->ArrangeLayout(rect);
		}
	}

	return finalSize;
}

//------------------------------------------------------------------------------
void UITrack::CoerceLength(float& componentLength, float trackLength)
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

//------------------------------------------------------------------------------
void UITrack::CalcScrollBarComponentsSize(
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

	m_density = range / remainingTrackLength;
}

//==============================================================================
// UIScrollEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIScrollEventArgs, UIEventArgs)

//==============================================================================
// UIScrollBar
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIScrollBar, UIControl)

//------------------------------------------------------------------------------
RefPtr<UIScrollBar> UIScrollBar::Create()
{
	auto ptr = RefPtr<UIScrollBar>::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIScrollBar::UIScrollBar()
	: m_track(nullptr)
	, m_dragStartValue(0)
{
}

//------------------------------------------------------------------------------
UIScrollBar::~UIScrollBar()
{
}

//------------------------------------------------------------------------------
void UIScrollBar::Initialize(detail::UIManager* manager)
{
	UIControl::Initialize(manager);

	m_track = NewObject<UITrack>(manager);
	AddVisualChild(m_track);
}

//------------------------------------------------------------------------------
void UIScrollBar::SetValue(float value)
{
	m_track->SetValue(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::GetValue() const
{
	return m_track->GetValue();
}

//------------------------------------------------------------------------------
void UIScrollBar::SetMinimum(float value)
{
	m_track->SetMinimum(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::GetMinimum() const
{
	return m_track->GetMinimum();
}

//------------------------------------------------------------------------------
void UIScrollBar::SetMaximum(float value)
{
	m_track->SetMaximum(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::GetMaximum() const
{
	return m_track->GetMaximum();
}

//------------------------------------------------------------------------------
void UIScrollBar::SetViewportSize(float value)
{
	m_track->SetViewportSize(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::GetViewportSize() const
{
	return m_track->GetViewportSize();
}

//------------------------------------------------------------------------------
void UIScrollBar::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	if (ev == UIThumb::DragStartedEventId)
	{
		m_dragStartValue = m_track->GetValue();
	}
	else if (ev == UIThumb::DragDeltaEventId)
	{
		auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);
		UpdateValue(e2->horizontalChange, e2->verticalChange);
	}
	UIControl::OnRoutedEvent(ev, e);
}

//------------------------------------------------------------------------------
void UIScrollBar::GetStyleClassName(String* outSubStateName)
{
	if (m_track->GetOrientation() == Orientation::Horizontal)
		*outSubStateName = _T("Horizontal");
	else
		*outSubStateName = _T("Vertical");
}

//------------------------------------------------------------------------------
void UIScrollBar::UpdateValue(float horizontalDragDelta, float verticalDragDelta)
{
	float valueDelta = m_track->ValueFromDistance(horizontalDragDelta, verticalDragDelta);

	float newValue = m_dragStartValue + valueDelta;
	m_track->SetValue(newValue);

	// TODO:
	//RefPtr<ScrollEventArgs> args(m_manager->GetEventArgsPool()->Create<ScrollEventArgs>(newValue, ScrollEventType::ThumbTrack), false);
	//RaiseEvent(ScrollEvent, this, args);
}

//==============================================================================
// UIScrollViewer
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIScrollViewer, UIControl)

//------------------------------------------------------------------------------
RefPtr<UIScrollViewer> UIScrollViewer::Create()
{
	auto ptr = RefPtr<UIScrollViewer>::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIScrollViewer::UIScrollViewer()
	: m_verticalScrollBar(nullptr)
	, m_horizontalScrollBar(nullptr)
{
}

//------------------------------------------------------------------------------
UIScrollViewer::~UIScrollViewer()
{
}

//------------------------------------------------------------------------------
void UIScrollViewer::Initialize(detail::UIManager* manager)
{
	UIControl::Initialize(manager);

	m_verticalScrollBar = NewObject<UIScrollBar>(manager);
	m_horizontalScrollBar = NewObject<UIScrollBar>(manager);
	AddVisualChild(m_verticalScrollBar);
	AddVisualChild(m_horizontalScrollBar);
}

//------------------------------------------------------------------------------
Size UIScrollViewer::MeasureOverride(const Size& constraint)
{
	return UIControl::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UIScrollViewer::ArrangeOverride(const Size& finalSize)
{
	return UIControl::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UIScrollViewer::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	UIControl::OnRoutedEvent(ev, e);
}

LN_NAMESPACE_END
