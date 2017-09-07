
#include "Internal.h"
#include <Lumino/UI/UIScrollViewer.h>
#include <Lumino/UI/UILayoutPanel.h>
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
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIThumb, UIElement)
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragStartedEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragDeltaEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragCompletedEvent);
LN_ROUTED_EVENT_IMPLEMENT2(UIThumb, UIDragDeltaEventArgs, DragCanceledEvent);

//------------------------------------------------------------------------------
Ref<UIThumb> UIThumb::create()
{
	auto ptr = Ref<UIThumb>::makeRef();
	ptr->initialize();
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
void UIThumb::initialize()
{
	UIElement::initialize();
}

//------------------------------------------------------------------------------
void UIThumb::onRoutedEvent(UIEventArgs* e)
{
	if (e->getType() == UIEvents::MouseDownEvent)
	{
		if (!m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition(getVisualParent());

			m_lastScreenPosition = pos;
			m_isDragging = true;
			captureMouse();

			// ドラッグ開始イベント
			detail::EventArgsPool* pool = getManager()->getEventArgsPool();
			Ref<UIDragDeltaEventArgs> args(pool->create<UIDragDeltaEventArgs>(DragStartedEventId), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			onDragStarted(args);

			e->handled = true;
			return;
		}
	}
	else if (e->getType() == UIEvents::MouseUpEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition(getVisualParent());

			m_isDragging = false;
			releaseMouseCapture();

			// ドラッグ終了イベント
			// TODO: template 化
			detail::EventArgsPool* pool = getManager()->getEventArgsPool();
			Ref<UIDragDeltaEventArgs> args(pool->create<UIDragDeltaEventArgs>(DragCompletedEventId), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			onDragCompleted(args);

			e->handled = true;
			return;
		}
	}
	else if (e->getType() == UIEvents::MouseMoveEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition(getVisualParent());

			// ドラッグ中イベント
			detail::EventArgsPool* pool = getManager()->getEventArgsPool();
			Ref<UIDragDeltaEventArgs> args(pool->create<UIDragDeltaEventArgs>(DragDeltaEventId), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			onDragDelta(args);

			e->handled = true;
			return;
		}
	}

	UIElement::onRoutedEvent(e);
}

//==============================================================================
// UITrack
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITrack, UIElement)

const String UITrack::OrientationStates = _LT("OrientationStates");
const String UITrack::HorizontalState = _LT("Horizontal");
const String UITrack::VerticalState = _LT("Vertical");

//------------------------------------------------------------------------------
Ref<UITrack> UITrack::create()
{
	auto ptr = Ref<UITrack>::makeRef();
	ptr->initialize();
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
	, m_decreaseButton(nullptr)
	, m_thumb(nullptr)
	, m_increaseButton(nullptr)
{
}

//------------------------------------------------------------------------------
UITrack::~UITrack()
{
}

//------------------------------------------------------------------------------
void UITrack::initialize()
{
	UIElement::initialize();

	// register VisualState
	auto* vsm = getVisualStateManager();
	vsm->registerVisualState(OrientationStates, HorizontalState);
	vsm->registerVisualState(OrientationStates, VerticalState);

	m_decreaseButton = newObject<UIButton>();
	m_thumb = newObject<UIThumb>();
	m_increaseButton = newObject<UIButton>();

	m_decreaseButton->setStyleSubControlName(tr::TypeInfo::getTypeInfo<UITrack>()->getName(), _LT("DecreaseButton"));
	m_increaseButton->setStyleSubControlName(tr::TypeInfo::getTypeInfo<UITrack>()->getName(), _LT("IncreaseButton"));

	addVisualChild(m_decreaseButton);
	addVisualChild(m_thumb);
	addVisualChild(m_increaseButton);

	setOrientation(Orientation::Horizontal);
}

//------------------------------------------------------------------------------
void UITrack::setOrientation(Orientation orientation)
{
	m_orientation = orientation;

	switch (orientation)
	{
	case Orientation::Horizontal:
		goToVisualState(HorizontalState);
		break;
	case Orientation::Vertical:
		goToVisualState(VerticalState);
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

//------------------------------------------------------------------------------
float UITrack::valueFromDistance(float horizontal, float vertical)
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
UIButton* UITrack::getDecreaseButton() const
{
	return m_decreaseButton;
}

//------------------------------------------------------------------------------
UIButton* UITrack::getIncreaseButton() const
{
	return m_increaseButton;
}

//------------------------------------------------------------------------------
UIThumb* UITrack::getThumb() const
{
	return m_thumb;
}

//------------------------------------------------------------------------------
Size UITrack::measureOverride(const Size& constraint)
{
	m_decreaseButton->measureLayout(constraint);
	m_thumb->measureLayout(constraint);
	m_increaseButton->measureLayout(constraint);
	return UIElement::measureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UITrack::arrangeOverride(const Size& finalSize)
{
	float decreaseButtonLength;
	float thumbLength;
	float increaseButtonLength;
	float trackLength = (m_orientation == Orientation::Vertical) ? finalSize.height : finalSize.width;

	if (Math::isNaN(m_viewportSize))
	{
		// ビューサイズが関係ない場合の計算。つまり、Slider コントロール用
		calcSliderComponentsSize(finalSize, (m_orientation == Orientation::Vertical), &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}
	else
	{
		calcScrollBarComponentsSize(trackLength, m_viewportSize, &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}

	if (m_orientation == Orientation::Horizontal)
	{
		Rect rect(0.0f, 0.0f, 0.0f, finalSize.height);

		// PageUp Button
		if (m_decreaseButton != nullptr)
		{
			rect.x = 0.0f;
			rect.width = decreaseButtonLength;
			m_decreaseButton->arrangeLayout(rect);
		}
		// Thumb
		if (m_thumb != nullptr)
		{
			rect.x = decreaseButtonLength;
			rect.width = thumbLength;
			m_thumb->arrangeLayout(rect);
		}
		// PageDown Button
		if (m_increaseButton != nullptr)
		{
			rect.x = decreaseButtonLength + thumbLength;
			rect.width = increaseButtonLength;
			m_increaseButton->arrangeLayout(rect);
		}
	}
	else
	{
		Rect rect(0.0f, 0.0f, finalSize.width, 0.0f);

		// PageUp Button
		if (m_decreaseButton != nullptr)
		{
			rect.y = 0.0f;
			rect.height = decreaseButtonLength;
			m_decreaseButton->arrangeLayout(rect);
		}
		// Thumb
		if (m_thumb != nullptr)
		{
			rect.y = decreaseButtonLength;
			rect.height = thumbLength;
			m_thumb->arrangeLayout(rect);
		}
		// PageDown Button
		if (m_increaseButton != nullptr)
		{
			rect.y = decreaseButtonLength + thumbLength;
			rect.height = increaseButtonLength;
			m_increaseButton->arrangeLayout(rect);
		}
	}

	return finalSize;
}

//------------------------------------------------------------------------------
void UITrack::coerceLength(float& componentLength, float trackLength)
{
	if (componentLength < 0)
	{
		componentLength = 0.0f;
	}
	else if (componentLength > trackLength || Math::isNaN(componentLength))
	{
		componentLength = trackLength;
	}
}

//------------------------------------------------------------------------------
void UITrack::calcSliderComponentsSize(
	const Size& arrangeSize,
	bool isVertical,
	float* outDecreaseButtonLength,
	float* outThumbLength,
	float* outIncreaseButtonLength)
{
	float min = m_minimum;
	float range = std::max(0.0f, m_maximum - min);
	float offset = std::min(range, m_value - min);

	float trackLength;
	float thumbLength;

	if (isVertical)
	{
		trackLength = arrangeSize.height;
		thumbLength = (m_thumb == nullptr) ? 0 : m_thumb->getDesiredSize().height;
	}
	else
	{
		trackLength = arrangeSize.width;
		thumbLength = (m_thumb == nullptr) ? 0 : m_thumb->getDesiredSize().width;
	}

	coerceLength(thumbLength, trackLength);

	float remainingTrackLength = trackLength - thumbLength;

	float decreaseButtonLength = remainingTrackLength * offset / range;
	coerceLength(decreaseButtonLength, remainingTrackLength);

	float increaseButtonLength = remainingTrackLength - decreaseButtonLength;
	coerceLength(increaseButtonLength, remainingTrackLength);


	*outDecreaseButtonLength = decreaseButtonLength;
	*outThumbLength = thumbLength;
	*outIncreaseButtonLength = increaseButtonLength;

	m_density = range / remainingTrackLength;
}

//------------------------------------------------------------------------------
void UITrack::calcScrollBarComponentsSize(
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
	float offset = std::min(range, m_value - min);            // m_value の位置
	float extent = std::max(0.0f, range) + viewportSize;    // コンテンツ全体のサイズ

	//float trackLength = finalLength;
	float thumbMinLength = 16.0f;

	// Thumb サイズを計算する
	float thumbLength = trackLength * viewportSize / extent;    // コンテンツ全体の内、どの部分を表示しているのか、その割合で Thumb の長さを作る
	coerceLength(thumbLength, trackLength);
	thumbLength = std::max(thumbMinLength, thumbLength);

	// 残りの部分のサイズ
	float remainingTrackLength = trackLength - thumbLength;

	// DecreaseButton
	float decreaseButtonLength = remainingTrackLength * offset / range;
	coerceLength(decreaseButtonLength, remainingTrackLength);

	// IncreaseButton
	float increaseButtonLength = remainingTrackLength - decreaseButtonLength;
	coerceLength(increaseButtonLength, remainingTrackLength);

	*outDecreaseButtonLength = decreaseButtonLength;
	*outThumbLength = thumbLength;
	*outIncreaseButtonLength = increaseButtonLength;

	m_density = range / remainingTrackLength;
}

//==============================================================================
// UIScrollEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIScrollEventArgs, UIEventArgs)

//------------------------------------------------------------------------------
UIScrollEventArgsPtr UIScrollEventArgs::create(Object* sender, float newValue, ScrollEventType type, bool caching)
{
	if (caching)
	{
		detail::EventArgsPool* pool = detail::UIManager::getInstance()->getEventArgsPool();
		Ref<UIScrollEventArgs> ptr(pool->create<UIScrollEventArgs>(sender, newValue, type), false);
		return ptr;
	}
	else
	{
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

//------------------------------------------------------------------------------
UIScrollEventArgs::UIScrollEventArgs()
	: newValue(0.0f)
	, type(ScrollEventType::ThumbTrack)
{
}

//------------------------------------------------------------------------------
UIScrollEventArgs::~UIScrollEventArgs()
{
}

//------------------------------------------------------------------------------
void UIScrollEventArgs::initialize(Object* sender, float newValue_, ScrollEventType type_)
{
	sender = sender;
	newValue = newValue_;
	type = type_;
}

//==============================================================================
// UIScrollBar
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIScrollBar, UIControl)
LN_ROUTED_EVENT_IMPLEMENT2(UIScrollBar, UIScrollEventArgs, ScrollEvent);

const String UIScrollBar::OrientationStates = _LT("OrientationStates");
const String UIScrollBar::HorizontalState = _LT("Horizontal");
const String UIScrollBar::VerticalState = _LT("Vertical");

//------------------------------------------------------------------------------
Ref<UIScrollBar> UIScrollBar::create()
{
	auto ptr = Ref<UIScrollBar>::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIScrollBar::UIScrollBar()
	: m_track(nullptr)
	, m_dragStartValue(0)
	, m_lineUpButton(nullptr)
	, m_lineDownButton(nullptr)
{
}

//------------------------------------------------------------------------------
UIScrollBar::~UIScrollBar()
{
}

//------------------------------------------------------------------------------
void UIScrollBar::initialize()
{
	UIControl::initialize();

	// register VisualState
	auto* vsm = getVisualStateManager();
	vsm->registerVisualState(OrientationStates, HorizontalState);
	vsm->registerVisualState(OrientationStates, VerticalState);

	m_track = newObject<UITrack>();
	m_lineUpButton = newObject<UIButton>();
	m_lineDownButton = newObject<UIButton>();
	addVisualChild(m_track);
	addVisualChild(m_lineUpButton);
	addVisualChild(m_lineDownButton);

	m_lineUpButton->setStyleSubControlName(tr::TypeInfo::getTypeInfo<UIScrollBar>()->getName(), _LT("LineUpButton"));
	m_lineDownButton->setStyleSubControlName(tr::TypeInfo::getTypeInfo<UIScrollBar>()->getName(), _LT("LineDownButton"));

	// TODO:
	m_lineUpButton->setSize(Size(16, 16));
	m_lineDownButton->setSize(Size(16, 16));

	setOrientation(Orientation::Horizontal);
}

//------------------------------------------------------------------------------
void UIScrollBar::setOrientation(Orientation orientation)
{
	m_track->setOrientation(orientation);

	switch (orientation)
	{
	case Orientation::Horizontal:
		goToVisualState(HorizontalState);
		break;
	case Orientation::Vertical:
		goToVisualState(VerticalState);
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

//------------------------------------------------------------------------------
Orientation UIScrollBar::getOrientation() const
{
	return m_track->getOrientation();
}

//------------------------------------------------------------------------------
void UIScrollBar::setValue(float value)
{
	m_track->setValue(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::getValue() const
{
	return m_track->getValue();
}

//------------------------------------------------------------------------------
void UIScrollBar::setMinimum(float value)
{
	m_track->setMinimum(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::getMinimum() const
{
	return m_track->getMinimum();
}

//------------------------------------------------------------------------------
void UIScrollBar::setMaximum(float value)
{
	m_track->setMaximum(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::getMaximum() const
{
	return m_track->getMaximum();
}

//------------------------------------------------------------------------------
void UIScrollBar::setViewportSize(float value)
{
	m_track->setViewportSize(value);
}

//------------------------------------------------------------------------------
float UIScrollBar::getViewportSize() const
{
	return m_track->getViewportSize();
}

//------------------------------------------------------------------------------
void UIScrollBar::onRoutedEvent(UIEventArgs* e)
{
	if (e->getType() == UIThumb::DragStartedEventId)
	{
		m_dragStartValue = m_track->getValue();
	}
	else if (e->getType() == UIThumb::DragDeltaEventId)
	{
		auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);
		updateValue(e2->horizontalChange, e2->verticalChange);

		auto args = UIScrollEventArgs::create(this, m_track->getValue(), ScrollEventType::ThumbTrack);
		raiseEvent(ScrollEventId, this, args);

		//switch (m_track->getOrientation())
		//{
		//case Orientation::Horizontal:

		//	break;
		//case Orientation::Vertical:
		//	break;
		//case Orientation::ReverseHorizontal:
		//case Orientation::ReverseVertical:
		//default:
		//	LN_NOTIMPLEMENTED();
		//	break;
		//}
	}
	else if (e->getType() == UIThumb::DragCompletedEventId)
	{
		auto args = UIScrollEventArgs::create(this, m_track->getValue(), ScrollEventType::EndScroll);
		raiseEvent(ScrollEventId, this, args);
	}
	UIControl::onRoutedEvent(e);
}

//------------------------------------------------------------------------------
Size UIScrollBar::measureOverride(const Size& constraint)
{
	m_track->measureLayout(constraint);
	m_lineUpButton->measureLayout(constraint);
	m_lineDownButton->measureLayout(constraint);
	return UIControl::measureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UIScrollBar::arrangeOverride(const Size& finalSize)
{
	Size upSize;
	Size downSize;
	Orientation orientation = getOrientation();

	switch (orientation)
	{
	case Orientation::Horizontal:
		upSize = m_lineUpButton->getDesiredSize();
		downSize = m_lineUpButton->getDesiredSize();
		m_lineUpButton->arrangeLayout(Rect(0, 0, upSize.width, finalSize.height));
		m_lineDownButton->arrangeLayout(Rect(finalSize.width - downSize.width, 0, downSize.width, finalSize.height));
		m_track->arrangeLayout(Rect(upSize.width, 0, finalSize.width - upSize.width - downSize.width, finalSize.height));
		break;
	case Orientation::Vertical:
		m_track->arrangeLayout(Rect(0, 0, finalSize));
		m_lineUpButton->arrangeLayout(Rect(0, 0, finalSize.width, m_lineUpButton->getDesiredSize().height));
		m_lineDownButton->arrangeLayout(Rect(0, finalSize.height - m_lineDownButton->getDesiredSize().height, finalSize.width, m_lineDownButton->getDesiredSize().height));
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}

	return UIControl::arrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
//void UIScrollBar::GetStyleClassName(String* outSubStateName)
//{
//	if (m_track->getOrientation() == Orientation::Horizontal)
//		*outSubStateName = _LT("Horizontal");
//	else
//		*outSubStateName = _LT("Vertical");
//}

//------------------------------------------------------------------------------
void UIScrollBar::updateValue(float horizontalDragDelta, float verticalDragDelta)
{
	float valueDelta = m_track->valueFromDistance(horizontalDragDelta, verticalDragDelta);

	float newValue = m_dragStartValue + valueDelta;
	m_track->setValue(newValue);

	// TODO:
	//Ref<ScrollEventArgs> args(m_manager->getEventArgsPool()->Create<ScrollEventArgs>(newValue, ScrollEventType::ThumbTrack), false);
	//raiseEvent(ScrollEvent, this, args);
}

//==============================================================================
// UIScrollViewer
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIScrollViewer, UIControl)

//------------------------------------------------------------------------------
Ref<UIScrollViewer> UIScrollViewer::create()
{
	auto ptr = Ref<UIScrollViewer>::makeRef();
	ptr->initialize();
	return ptr;
}

//------------------------------------------------------------------------------
UIScrollViewer::UIScrollViewer()
	: m_horizontalScrollBar(nullptr)
	, m_verticalScrollBar(nullptr)
	, m_scrollTarget(nullptr)
{
}

//------------------------------------------------------------------------------
UIScrollViewer::~UIScrollViewer()
{
}

//------------------------------------------------------------------------------
void UIScrollViewer::initialize()
{
	UIControl::initialize();

	m_horizontalScrollBar = newObject<UIScrollBar>();
	m_horizontalScrollBar->setOrientation(Orientation::Horizontal);
	m_verticalScrollBar = newObject<UIScrollBar>();
	m_verticalScrollBar->setOrientation(Orientation::Vertical);
	addVisualChild(m_horizontalScrollBar);
	addVisualChild(m_verticalScrollBar);
}

//------------------------------------------------------------------------------
Size UIScrollViewer::measureOverride(const Size& constraint)
{
	Size desiredSize = UIControl::measureOverride(constraint);

	desiredSize.width += m_verticalScrollBar->getWidth();
	desiredSize.height += m_horizontalScrollBar->getHeight();

	return desiredSize;
}

//------------------------------------------------------------------------------
Size UIScrollViewer::arrangeOverride(const Size& finalSize)
{
	float barWidth = m_verticalScrollBar->getWidth();
	float barHeight = m_horizontalScrollBar->getHeight();


	Size childArea(finalSize.width - barWidth, finalSize.height - barHeight);
	Size actualSize = UIControl::arrangeOverride(childArea);


	Rect rc;

	rc.width = barWidth;
	rc.height = finalSize.height - barHeight;
	rc.x = finalSize.width - barWidth;
	rc.y = 0;
	m_verticalScrollBar->arrangeLayout(rc);

	rc.width = finalSize.width - barWidth;
	rc.height = barHeight;
	rc.x = 0;
	rc.y = finalSize.height - barHeight;
	m_horizontalScrollBar->arrangeLayout(rc);

	if (m_scrollTarget != nullptr)
	{
		m_horizontalScrollBar->setMinimum(0.0f);
		m_horizontalScrollBar->setMaximum(m_scrollTarget->getExtentWidth());
		m_horizontalScrollBar->setViewportSize(m_scrollTarget->getViewportWidth());
		m_verticalScrollBar->setMinimum(0.0f);
		m_verticalScrollBar->setMaximum(m_scrollTarget->getExtentHeight());
		m_verticalScrollBar->setViewportSize(m_scrollTarget->getViewportHeight());
	}
	else
	{
		m_horizontalScrollBar->setMinimum(0.0f);
		m_horizontalScrollBar->setMaximum(0.0f);
		m_horizontalScrollBar->setViewportSize(0.0f);
		m_verticalScrollBar->setMinimum(0.0f);
		m_verticalScrollBar->setMaximum(0.0f);
		m_verticalScrollBar->setViewportSize(0.0f);
	}

	return actualSize;
}

//------------------------------------------------------------------------------
void UIScrollViewer::onRoutedEvent(UIEventArgs* e)
{
	UIControl::onRoutedEvent(e);

	if (e->getType() == UIScrollBar::ScrollEventId)
	{
		auto* e2 = static_cast<UIScrollEventArgs*>(e);

		if (m_scrollTarget != nullptr)
		{
			if (e->sender == m_horizontalScrollBar)
			{
				m_scrollTarget->setHorizontalOffset(e2->newValue);
			}
			else if (e->sender == m_verticalScrollBar)
			{
				m_scrollTarget->setVerticalOffset(e2->newValue);
			}
		}
	}
}

//------------------------------------------------------------------------------
void UIScrollViewer::onLogicalChildrenPresenterChanged(UILayoutPanel* newPanel)
{
	m_scrollTarget = newPanel;
}


LN_NAMESPACE_END
