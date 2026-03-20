
#include "Internal.hpp"
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/Layout/UILayoutPanel.hpp>
#include <LuminoFramework/UI/Controls/UIButton.hpp>
#include <LuminoFramework/UI/Controls/UIScrollView.hpp>

namespace ln {

//==============================================================================
// UIScrollView

UIScrollView::UIScrollView()
{
}

void UIScrollView::init()
{
	UIControl::init();
}


//==============================================================================
// UIThumb

UIThumb::UIThumb()
	: m_lastScreenPosition()
	, m_isDragging(false)
{
}

UIThumb::~UIThumb()
{
}

void UIThumb::init()
{
	UIElement::init();
}

void UIThumb::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent)
	{
		if (!m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition();

			m_lastScreenPosition = pos;
			m_isDragging = true;
			retainCapture();

			auto args = UIDragDeltaEventArgs::create(this, UIEvents::ScrollDragStartedEvent, pos.x - m_lastScreenPosition.x, pos.y - m_lastScreenPosition.y);
			onDragStarted(args);

			e->handled = true;
			return;
		}
	}
	else if (e->type() == UIEvents::MouseUpEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition();

			m_isDragging = false;
			releaseCapture();

			auto args = UIDragDeltaEventArgs::create(this, UIEvents::ScrollDragCompletedEvent, pos.x - m_lastScreenPosition.x, pos.y - m_lastScreenPosition.y);
			onDragCompleted(args);

			e->handled = true;
			return;
		}
	}
	else if (e->type() == UIEvents::MouseMoveEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition();

			auto args = UIDragDeltaEventArgs::create(this, UIEvents::ScrollDragDeltaEvent, pos.x - m_lastScreenPosition.x, pos.y - m_lastScreenPosition.y);
			onDragDelta(args);

			e->handled = true;
			return;
		}
	}

	UIElement::onRoutedEvent(e);
}

void UIThumb::onDragStarted(UIDragDeltaEventArgs* e)
{
	raiseEvent(e);
}

void UIThumb::onDragDelta(UIDragDeltaEventArgs* e)
{
	raiseEvent(e);
}

void UIThumb::onDragCompleted(UIDragDeltaEventArgs* e)
{
	raiseEvent(e);
}

void UIThumb::onDragCanceled(UIDragDeltaEventArgs* e)
{
	raiseEvent(e);
}

//==============================================================================
// UIRangeBase

/*
	onMaximumChanged, onMinimumChanged は用意するが、対応する Changed イベントは
	用意しない。WPF のインターフェイスと合わせたものだが、そもそもそれをハンドルする
	ようなこともないだろうし、キャンセルされたときに困る。
*/

UIRangeBase::UIRangeBase()
	: m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
{
}

UIRangeBase::~UIRangeBase()
{
}

void UIRangeBase::init()
{
	UIElement::init();
}

void UIRangeBase::setValue(float value)
{
	float oldValue = m_value;
	m_value = value;
	if (m_value != oldValue)
	{
		onValueChanged(oldValue, m_value);
	}
}

float UIRangeBase::value() const
{
	return m_value;
}

void UIRangeBase::setMinimum(float value)
{
	float oldValue = m_minimum;
	m_minimum = value;
	if (m_minimum != oldValue)
	{
		onMinimumChanged(oldValue, m_minimum);
	}
}

float UIRangeBase::minimum() const
{
	return m_minimum;
}

void UIRangeBase::setMaximum(float value)
{
	float oldValue = m_maximum;
	m_maximum = value;
	if (m_maximum != oldValue)
	{
		onMaximumChanged(oldValue, m_maximum);
	}
}

float UIRangeBase::maximum() const
{
	return m_maximum;
}

void UIRangeBase::onValueChanged(float oldValue, float newValue)
{
}

void UIRangeBase::onMinimumChanged(float oldMinimum, float newMinimum)
{
}

void UIRangeBase::onMaximumChanged(float oldMaximum, float newMaximum)
{
}


//==============================================================================
// UITrack

const String UITrack::OrientationStates = _LT("OrientationStates");
const String UITrack::HorizontalState = _LT("Horizontal");
const String UITrack::VerticalState = _LT("Vertical");

UITrack::UITrack()
	: m_orientation(UILayoutOrientation::Horizontal)
	, m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_density(1.0f)
	, m_viewportSize(0.0f)
	, m_decreaseButton(nullptr)
	, m_thumb(nullptr)
	, m_increaseButton(nullptr)
{
	m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
}

UITrack::~UITrack()
{
}

void UITrack::init()
{
	UIElement::init();

	// register VisualState
	auto* vsm = getVisualStateManager();
	vsm->registerState(OrientationStates, HorizontalState);
	vsm->registerState(OrientationStates, VerticalState);

	//m_decreaseButton = makeObject_deprecated<UIButton>();
	m_thumb = makeObject_deprecated<UIThumb>();
	//m_increaseButton = makeObject_deprecated<UIButton>();

	//m_decreaseButton->addClass(_TT("UITrack-DecreaseButton");
	m_thumb->addClass(_TT("UITrack-Thumb"));
	//m_increaseButton->addClass(_TT("UITrack-IncreaseButton");

	//addVisualChild(m_decreaseButton);
	addVisualChild(m_thumb);
	//addVisualChild(m_increaseButton);

	setOrientation(UILayoutOrientation::Horizontal);
}

void UITrack::setOrientation(UILayoutOrientation orientation)
{
	m_orientation = orientation;

	switch (orientation)
	{
	case UILayoutOrientation::Horizontal:
		getVisualStateManager()->gotoState(HorizontalState);
		break;
	case UILayoutOrientation::Vertical:
		getVisualStateManager()->gotoState(VerticalState);
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

void UITrack::setValue(float value)
{
	m_value = std::max(m_minimum, std::min(m_maximum, value));
}

float UITrack::valueFromDistance(float horizontal, float vertical)
{
	float scale = 1;//IsDirectionReversed ? -1 : 1;

	if (m_orientation == UILayoutOrientation::Horizontal)
	{
		return scale * horizontal * m_density;
	}
	else
	{
		return scale * vertical * m_density;
	}
}

UIButton* UITrack::getDecreaseButton() const
{
	return m_decreaseButton;
}

UIButton* UITrack::getIncreaseButton() const
{
	return m_increaseButton;
}

UIThumb* UITrack::getThumb() const
{
	return m_thumb;
}

Size UITrack::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	if (m_decreaseButton) {
		m_decreaseButton->measureLayout(layoutContext, constraint);
	}

	if (m_thumb) {
		m_thumb->measureLayout(layoutContext, constraint);
	}

	if (m_increaseButton) {
		m_increaseButton->measureLayout(layoutContext, constraint);
	}

	return UIElement::measureOverride(layoutContext, constraint);
}

Size UITrack::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const auto finalSize = finalArea.getSize();

	float decreaseButtonLength;
	float thumbLength;
	float increaseButtonLength;
	float trackLength = (m_orientation == UILayoutOrientation::Vertical) ? finalSize.height : finalSize.width;

	if (Math::isNaN(m_viewportSize))
	{
		// ビューサイズが関係ない場合の計算。つまり、Slider コントロール用
		calcSliderComponentsSize(finalSize, (m_orientation == UILayoutOrientation::Vertical), &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}
	else
	{
		calcScrollBarComponentsSize(trackLength, m_viewportSize, &decreaseButtonLength, &thumbLength, &increaseButtonLength);
	}

	if (m_orientation == UILayoutOrientation::Horizontal)
	{
		Rect rect(0.0f, 0.0f, 0.0f, finalSize.height);

		// PageUp Button
		if (m_decreaseButton != nullptr)
		{
			rect.x = 0.0f;
			rect.width = decreaseButtonLength;
			m_decreaseButton->arrangeLayout(layoutContext, rect);
		}
		// Thumb
		if (m_thumb != nullptr)
		{
			rect.x = decreaseButtonLength;
			rect.width = std::ceil(thumbLength);	// Pixel snap
			m_thumb->arrangeLayout(layoutContext, rect);
		}
		// PageDown Button
		if (m_increaseButton != nullptr)
		{
			rect.x = decreaseButtonLength + thumbLength;
			rect.width = increaseButtonLength;
			m_increaseButton->arrangeLayout(layoutContext, rect);
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
			m_decreaseButton->arrangeLayout(layoutContext, rect);
		}
		// Thumb
		if (m_thumb != nullptr)
		{
			rect.y = decreaseButtonLength;
			rect.height = std::ceil(thumbLength);	// Pixel snap
			m_thumb->arrangeLayout(layoutContext, rect);
		}
		// PageDown Button
		if (m_increaseButton != nullptr)
		{
			rect.y = decreaseButtonLength + thumbLength;
			rect.height = increaseButtonLength;
			m_increaseButton->arrangeLayout(layoutContext, rect);
		}
	}

	return finalSize;
}

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
		thumbLength = (m_thumb == nullptr) ? 0 : m_thumb->desiredSize().height;
	}
	else
	{
		trackLength = arrangeSize.width;
		thumbLength = (m_thumb == nullptr) ? 0 : m_thumb->desiredSize().width;
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
	//float extent = std::max(0.0f, range);

	float tl = (viewportSize / (range + viewportSize)) * trackLength;

	//float trackLength = finalLength;
	float thumbMinLength = 16.0f;

	// Thumb サイズを計算する
	//float thumbLength = trackLength * viewportSize / extent;    // コンテンツ全体の内、どの部分を表示しているのか、その割合で Thumb の長さを作る
	float thumbLength = (viewportSize / (range + viewportSize)) * trackLength;	// MSDN の計算式
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
// UIScrollBar

const String UIScrollBar::OrientationStates = _LT("OrientationStates");
const String UIScrollBar::HorizontalState = _LT("Horizontal");
const String UIScrollBar::VerticalState = _LT("Vertical");

UIScrollBar::UIScrollBar()
    : m_track(nullptr)
    , m_dragStartValue(0)
    , m_lineUpButton(nullptr)
    , m_lineDownButton(nullptr)
{
}

UIScrollBar::~UIScrollBar()
{
}

void UIScrollBar::init()
{
    UIElement::init();

    // register VisualState
    auto* vsm = getVisualStateManager();
    vsm->registerState(OrientationStates, HorizontalState);
    vsm->registerState(OrientationStates, VerticalState);

    m_track = makeObject_deprecated<UITrack>();
    //m_lineUpButton = makeObject_deprecated<UIButton>();
    //m_lineDownButton = makeObject_deprecated<UIButton>();
    addVisualChild(m_track);
    //addVisualChild(m_lineUpButton);
    //addVisualChild(m_lineDownButton);

    //m_lineUpButton->setStyleSubControlName(tr::TypeInfo::getTypeInfo<UIScrollBar>()->getName(), _LT("LineUpButton"));
    //m_lineDownButton->setStyleSubControlName(tr::TypeInfo::getTypeInfo<UIScrollBar>()->getName(), _LT("LineDownButton"));
	// TODO: styleseet
    m_track->setAlignments(UIAlignment::Stretch);
    //m_lineUpButton->setHAlignment(UIHAlignment::Stretch);
    //m_lineUpButton->setVAlignment(UIVAlignment::Stretch);
    //m_lineDownButton->setHAlignment(UIHAlignment::Stretch);
    //m_lineDownButton->setVAlignment(UIVAlignment::Stretch);

    //// TODO:
    //m_lineUpButton->setSize(Size(16, 16));
    //m_lineDownButton->setSize(Size(16, 16));

    setOrientation(UILayoutOrientation::Horizontal);
}

void UIScrollBar::setOrientation(UILayoutOrientation orientation)
{
    m_track->setOrientation(orientation);

    switch (orientation)
    {
    case UILayoutOrientation::Horizontal:
        getVisualStateManager()->gotoState(HorizontalState);
        break;
    case UILayoutOrientation::Vertical:
        getVisualStateManager()->gotoState(VerticalState);
        break;
    default:
        LN_NOTIMPLEMENTED();
        break;
    }
}

UILayoutOrientation UIScrollBar::getOrientation() const
{
    return m_track->getOrientation();
}

void UIScrollBar::setValue(float value)
{
    m_track->setValue(value);
}

float UIScrollBar::getValue() const
{
    return m_track->getValue();
}

void UIScrollBar::setMinimum(float value)
{
    m_track->setMinimum(value);
}

float UIScrollBar::getMinimum() const
{
    return m_track->getMinimum();
}

void UIScrollBar::setMaximum(float value)
{
    m_track->setMaximum(value);
}

float UIScrollBar::getMaximum() const
{
    return m_track->getMaximum();
}

void UIScrollBar::setViewportSize(float value)
{
    m_track->setViewportSize(value);
}

float UIScrollBar::getViewportSize() const
{
    return m_track->getViewportSize();
}

void UIScrollBar::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::ScrollDragStartedEvent)
    {
        m_dragStartValue = m_track->getValue();
    }
    else if (e->type() == UIEvents::ScrollDragDeltaEvent)
    {
        auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);
        updateValue(e2->offsetX(), e2->offsetY());

        auto args = UIScrollEventArgs::create(this, UIEvents::ScrollEvent, Math::clamp(m_track->getValue(), getMinimum(), getMaximum()), ScrollEventType::ThumbTrack);
        raiseEvent(args);
        m_track->invalidate(detail::UIElementDirtyFlags::Layout, true);
    }
    else if (e->type() == UIEvents::ScrollDragCompletedEvent)
    {
        auto args = UIScrollEventArgs::create(this, UIEvents::ScrollEvent, Math::clamp(m_track->getValue(), getMinimum(), getMaximum()), ScrollEventType::EndScroll);
        raiseEvent(args);
        m_track->invalidate(detail::UIElementDirtyFlags::Layout, true);
    }
    UIElement::onRoutedEvent(e);
}

Size UIScrollBar::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    if (m_track) {
        m_track->measureLayout(layoutContext, constraint);
    }

    if (m_lineUpButton) {
        m_lineUpButton->measureLayout(layoutContext, constraint);
    }

    if (m_lineDownButton) {
        m_lineDownButton->measureLayout(layoutContext, constraint);
    }

    return UIElement::measureOverride(layoutContext, constraint);
}

Size UIScrollBar::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const auto finalSize = finalArea.getSize();
    Size upSize;
    Size downSize;
    UILayoutOrientation orientation = getOrientation();

    switch (orientation)
    {
    case UILayoutOrientation::Horizontal:
        if (m_lineUpButton) {
            upSize = m_lineUpButton->desiredSize();
            downSize = m_lineUpButton->desiredSize();
            m_lineUpButton->arrangeLayout(layoutContext, Rect(0, 0, upSize.width, finalSize.height));
        }
        if (m_lineDownButton) {
            m_lineDownButton->arrangeLayout(layoutContext, Rect(finalSize.width - downSize.width, 0, downSize.width, finalSize.height));
        }
        if (m_track) {
            m_track->arrangeLayout(layoutContext, Rect(upSize.width, 0, finalSize.width - upSize.width - downSize.width, finalSize.height));
        }
        break;
    case UILayoutOrientation::Vertical:
        if (m_track) {
            m_track->arrangeLayout(layoutContext, Rect(0, 0, finalSize));
        }
        if (m_lineUpButton) {
            m_lineUpButton->arrangeLayout(layoutContext, Rect(0, 0, finalSize.width, m_lineUpButton->desiredSize().height));
        }
        if (m_lineDownButton) {
            m_lineDownButton->arrangeLayout(layoutContext, Rect(0, finalSize.height - m_lineDownButton->desiredSize().height, finalSize.width, m_lineDownButton->desiredSize().height));
        }
        break;
    default:
        LN_NOTIMPLEMENTED();
        break;
    }

    return UIElement::arrangeOverride(layoutContext, finalArea);
}

void UIScrollBar::updateValue(float horizontalDragDelta, float verticalDragDelta)
{
    float valueDelta = m_track->valueFromDistance(horizontalDragDelta, verticalDragDelta);

    float newValue = m_dragStartValue + valueDelta;
    m_track->setValue(newValue);


	//std::cout << newValue << std::endl;
	//std::cout << m_track->getValue() << std::endl;


    // TODO:
    //Ref<ScrollEventArgs> args(m_manager->getEventArgsPool()->Create<ScrollEventArgs>(newValue, ScrollEventType::ThumbTrack), false);
    //raiseEvent(ScrollEvent, this, args);
}

//==============================================================================
// UIScrollViewer

UIScrollViewHelper::UIScrollViewHelper(UIControl* owner)
	: m_owner(owner)
	, m_horizontalScrollBar(nullptr)
	, m_verticalScrollBar(nullptr)
	, m_scrollTarget(nullptr)
{
	m_owner->setClipToBounds(true);
}

void UIScrollViewHelper::setHScrollbarVisible(bool value)
{
	if (value) {
		if (!m_horizontalScrollBar) {
			m_horizontalScrollBar = makeObject_deprecated<UIScrollBar>();
			m_horizontalScrollBar->setOrientation(UILayoutOrientation::Horizontal);
			//m_horizontalScrollBar->setWidth(16);	// TODO: style
			m_horizontalScrollBar->setAlignments(UIAlignment::VerticalStretch);
			m_owner->addVisualChild(m_horizontalScrollBar);
		}
	}
	else {
		if (m_horizontalScrollBar) {
			m_owner->removeVisualChild(m_horizontalScrollBar);
			m_horizontalScrollBar = nullptr;
		}
	}
}

void UIScrollViewHelper::setVScrollbarVisible(bool value)
{
	if (value) {
		if (!m_verticalScrollBar) {
			m_verticalScrollBar = makeObject_deprecated<UIScrollBar>();
			//m_verticalScrollBar->setHeight(16);	// TODO: style
			m_verticalScrollBar->setOrientation(UILayoutOrientation::Vertical);
			m_verticalScrollBar->setAlignments(UIAlignment::HorizontalStretch);
			m_owner->addVisualChild(m_verticalScrollBar);
		}
	}
	else {
		if (m_verticalScrollBar) {
			m_owner->removeVisualChild(m_verticalScrollBar);
			m_verticalScrollBar = nullptr;
		}
	}
}

Size UIScrollViewHelper::measure(UILayoutContext* layoutContext, const Size& ownerAreaSize)
{
	Size desiredSize(0, 0);

	if (m_horizontalScrollBar) {
		m_horizontalScrollBar->measureLayout(layoutContext, ownerAreaSize);
		desiredSize.height += m_horizontalScrollBar->desiredSize().height;
	}

	if (m_verticalScrollBar) {
		m_verticalScrollBar->measureLayout(layoutContext, ownerAreaSize);
		desiredSize.width += m_verticalScrollBar->desiredSize().width;
	}

	return desiredSize;
	//Rect clientArea(0, 0, desiredSize.width, desiredSize.height);
	//return clientArea;
}

Rect UIScrollViewHelper::calculateClientArea(const Size& ownerAreaSize)
{
	Rect clientArea(0, 0, ownerAreaSize.width, ownerAreaSize.height);

	if (m_horizontalScrollBar) {
		clientArea.height -= m_horizontalScrollBar->desiredSize().height;
	}

	if (m_verticalScrollBar) {
		clientArea.width -= m_verticalScrollBar->desiredSize().width;
	}

	return clientArea;
}

void UIScrollViewHelper::arrange(UILayoutContext* layoutContext, const Size& ownerAreaSize)
{

	if (m_scrollTarget)
	{
		if (m_horizontalScrollBar) {
			m_horizontalScrollBar->setMinimum(0.0f);
			m_horizontalScrollBar->setMaximum(std::max(0.0f, m_scrollTarget->getExtentWidth() - m_scrollTarget->getViewportWidth()));
			m_horizontalScrollBar->setViewportSize(m_scrollTarget->getViewportWidth());
		}
		if (m_verticalScrollBar) {
			m_verticalScrollBar->setMinimum(0.0f);
			m_verticalScrollBar->setMaximum(std::max(0.0f, m_scrollTarget->getExtentHeight() - m_scrollTarget->getViewportHeight()));
			m_verticalScrollBar->setViewportSize(m_scrollTarget->getViewportHeight());
		}
	}
	else
	{
		if (m_horizontalScrollBar) {
			m_horizontalScrollBar->setMinimum(0.0f);
			m_horizontalScrollBar->setMaximum(0.0f);
			m_horizontalScrollBar->setViewportSize(0.0f);
		}
		if (m_verticalScrollBar) {
			m_verticalScrollBar->setMinimum(0.0f);
			m_verticalScrollBar->setMaximum(0.0f);
			m_verticalScrollBar->setViewportSize(0.0f);
		}
	}

	Rect rc;

	if (m_horizontalScrollBar) {
		auto desiredSize = m_horizontalScrollBar->desiredSize();
		rc.width = ownerAreaSize.width;
		rc.height = desiredSize.height;
		rc.x = 0;
		rc.y = ownerAreaSize.height - desiredSize.height;
		m_horizontalScrollBar->arrangeLayout(layoutContext, rc);
	}

	if (m_verticalScrollBar) {
		auto desiredSize = m_verticalScrollBar->desiredSize();
		rc.width = desiredSize.width;
		rc.height = ownerAreaSize.height;
		rc.x = ownerAreaSize.width - desiredSize.width;
		rc.y = 0;
		m_verticalScrollBar->arrangeLayout(layoutContext, rc);
	}




}

void UIScrollViewHelper::refreshScrollInfo()
{

}

void UIScrollViewHelper::handleRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::ScrollEvent)
	{
		auto* e2 = static_cast<UIScrollEventArgs*>(e);

		if (m_scrollTarget != nullptr)
		{
			if (e->sender() == m_horizontalScrollBar)
			{
				m_scrollTarget->setHorizontalOffset(e2->newValue());
			}
			else if (e->sender() == m_verticalScrollBar)
			{
				m_scrollTarget->setVerticalOffset(e2->newValue());
			}
		}
	}
}



//==============================================================================
// UIScrollViewer

UIScrollViewer::UIScrollViewer()
    : m_horizontalScrollBar(nullptr)
    , m_verticalScrollBar(nullptr)
    , m_scrollTarget(nullptr)
{
}

UIScrollViewer::~UIScrollViewer()
{
}

void UIScrollViewer::setHScrollbarVisible(bool value)
{
	if (value) {
		if (!m_horizontalScrollBar) {
			m_horizontalScrollBar = makeObject_deprecated<UIScrollBar>();
			m_horizontalScrollBar->setOrientation(UILayoutOrientation::Horizontal);
			m_horizontalScrollBar->setWidth(16);	// TODO: style
			m_horizontalScrollBar->setAlignments(UIAlignment::VerticalStretch);
			addVisualChild(m_horizontalScrollBar);
		}
	}
	else {
		if (m_horizontalScrollBar) {
			removeVisualChild(m_horizontalScrollBar);
			m_horizontalScrollBar = nullptr;
		}
	}
}

void UIScrollViewer::setVScrollbarVisible(bool value)
{
	if (value) {
		if (!m_verticalScrollBar) {
			m_verticalScrollBar = makeObject_deprecated<UIScrollBar>();
			m_verticalScrollBar->setHeight(16);	// TODO: style
			m_verticalScrollBar->setOrientation(UILayoutOrientation::Vertical);
			m_verticalScrollBar->setAlignments(UIAlignment::HorizontalStretch);
			addVisualChild(m_verticalScrollBar);
		}
	}
	else {
		if (m_verticalScrollBar) {
			removeVisualChild(m_verticalScrollBar);
			m_verticalScrollBar = nullptr;
		}
	}
}

void UIScrollViewer::init()
{
    UIControl::init();
	setHScrollbarVisible(true);
	setVScrollbarVisible(true);
    //m_horizontalScrollBar = makeObject_deprecated<UIScrollBar>();
    //m_horizontalScrollBar->setOrientation(UILayoutOrientation::Horizontal);
    //m_verticalScrollBar = makeObject_deprecated<UIScrollBar>();
    //m_verticalScrollBar->setOrientation(UILayoutOrientation::Vertical);
    //addVisualChild(m_horizontalScrollBar);
    //addVisualChild(m_verticalScrollBar);

    ////m_verticalScrollBar->setHeight(16);
    //m_verticalScrollBar->setWidth(16);
    //m_verticalScrollBar->setVAlignment(UIVAlignment::Stretch);
}

Size UIScrollViewer::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    Size desiredSize = UIControl::measureOverride(layoutContext, constraint);

    if (m_verticalScrollBar) {
        m_verticalScrollBar->measureLayout(layoutContext, constraint);
        desiredSize.width += m_verticalScrollBar->desiredSize().width;
    }
    if (m_horizontalScrollBar) {
        m_horizontalScrollBar->measureLayout(layoutContext, constraint);
        desiredSize.height += m_horizontalScrollBar->desiredSize().height;
    }

    return desiredSize;
}

Size UIScrollViewer::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const auto finalSize = finalArea.getSize();
	Rect rc;
	float barWidth = 0;
	float barHeight = 0;

	if (m_horizontalScrollBar) {
		rc.width = finalSize.width - barWidth;
		rc.height = barHeight;
		rc.x = 0;
		rc.y = finalSize.height - barHeight;
		m_horizontalScrollBar->arrangeLayout(layoutContext, rc);
		barWidth = m_horizontalScrollBar->actualSize().width;
	}

	if (m_verticalScrollBar) {
		rc.width = barWidth;
		rc.height = finalSize.height - barHeight;
		rc.x = finalSize.width - barWidth;
		rc.y = 0;
		m_verticalScrollBar->arrangeLayout(layoutContext, rc);
		barHeight = m_verticalScrollBar->actualSize().height;
	}




    Rect childArea(0, 0, finalSize.width - barWidth, finalSize.height - barHeight);
    Size actualSize = UIControl::arrangeOverride(layoutContext, childArea);




    if (m_scrollTarget != nullptr)
    {
		if (m_horizontalScrollBar) {
			m_horizontalScrollBar->setMinimum(0.0f);
			m_horizontalScrollBar->setMaximum(std::max(0.0f, m_scrollTarget->getExtentWidth() - m_scrollTarget->getViewportWidth()));
			m_horizontalScrollBar->setViewportSize(m_scrollTarget->getViewportWidth());
		}
		if (m_verticalScrollBar) {
			m_verticalScrollBar->setMinimum(0.0f);
			m_verticalScrollBar->setMaximum(std::max(0.0f, m_scrollTarget->getExtentHeight() - m_scrollTarget->getViewportHeight()));
			m_verticalScrollBar->setViewportSize(m_scrollTarget->getViewportHeight());
		}
    }
    else
    {
		if (m_horizontalScrollBar) {
			m_horizontalScrollBar->setMinimum(0.0f);
			m_horizontalScrollBar->setMaximum(0.0f);
			m_horizontalScrollBar->setViewportSize(0.0f);
		}
		if (m_verticalScrollBar) {
			m_verticalScrollBar->setMinimum(0.0f);
			m_verticalScrollBar->setMaximum(0.0f);
			m_verticalScrollBar->setViewportSize(0.0f);
		}
    }

    return actualSize;
}

void UIScrollViewer::onRoutedEvent(UIEventArgs* e)
{
    UIControl::onRoutedEvent(e);

    if (e->type() == UIEvents::ScrollEvent)
    {
        auto* e2 = static_cast<UIScrollEventArgs*>(e);

        if (m_scrollTarget != nullptr)
        {
            if (e->sender() == m_horizontalScrollBar)
            {
                m_scrollTarget->setHorizontalOffset(e2->newValue());
            }
            else if (e->sender() == m_verticalScrollBar)
            {
                m_scrollTarget->setVerticalOffset(e2->newValue());
            }
        }
    }
}

//void UIScrollViewer::onLayoutPanelChanged(UILayoutPanel2_Deprecated* newPanel)
//{
//    m_scrollTarget = newPanel;
//}


} // namespace ln

