
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/RootFrame.h>
#include <Lumino/GUI/GUIContext.h>
#include "UIManagerImpl.h"
#include "GUIHelper.h"
#include "GUIPainter.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN
	
//=============================================================================
// GUIContext
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(GUIContext, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext* GUIContext::Create()
{
	GUIContext* obj = LN_NEW GUIContext(GUIManagerImpl::Instance);
	AutoReleasePool::GetCurrent()->AddObject(obj);

	GUIManagerImpl::Instance->AddContextOnMainWindow(obj);
	obj->m_nativeWindow = GUIManagerImpl::Instance->GetMainWindow();
	obj->m_onMainWindow = true;

	//RefPtr<GUIContext> obj(LN_NEW GUIContext(GUIManagerImpl::Instance), false);
	//GUIManagerImpl::Instance->AddContextOnMainWindow(obj);
	//obj->m_nativeWindow = GUIManagerImpl::Instance->GetMainWindow();
	//obj->m_onMainWindow = true;
	//obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext::GUIContext(GUIManagerImpl* manager)
	: m_manager(NULL)
	, m_nativeWindow()
	, m_viewPixelSize()
	, m_rootFrame(NULL)
	, m_rootElement(NULL)
	, m_focusElement(NULL)
	, m_mouseHoverElement(NULL)
	, m_capturedElement(NULL)
	, m_mousePosition()
	, m_mouseButtonClickTimeout(GUIManagerImpl::DefaultouseButtonClickTimeout)
	, m_time(0)
	, m_activeAnimationClockList()
	, m_currentCursorImage(NULL)
	, m_cursorAnimationTime(0)
	, m_onMainWindow(false)
{
	memset(m_mouseClickTrackers, 0, sizeof(m_mouseClickTrackers));
	LN_REFOBJ_SET(m_manager, manager);

	m_rootFrame = RootFrame::internalCreateInstance(m_manager);
	m_rootFrame->ChangeContext(this);
	m_rootFrame->ApplyTemplate();	// テーマを直ちに更新

	m_currentCursorImage = m_manager->GetCursorImage(CommonCursorImage::Arrow);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIContext::~GUIContext()
{
	LN_SAFE_RELEASE(m_rootElement);
	if (m_rootFrame) {
		m_rootFrame->ChangeContext(NULL);
		LN_SAFE_RELEASE(m_rootFrame);
	}
	if (m_onMainWindow) {
		m_manager->RemoveContextOnMainWindow(this);
	}
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::SetRootElement(UIElement* element)
{
	//if (m_rootElement != NULL && element == NULL) {
	//	m_rootElement->ChangeContext(NULL);
	//}
	LN_REFOBJ_SET(m_rootElement, element);
	m_rootFrame->SetContent(m_rootElement);
	//if (m_rootElement != NULL) {
	//	m_rootElement->ChangeContext(this);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* GUIContext::GetRootElement() const
{
	return m_rootElement;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::InjectViewportSizeChanged(int width, int height)
{
	m_viewPixelSize.Set(width, height);
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::InjectMouseMove(float clientX, float clientY)
{
	EventArgsPool* pool = m_manager->GetEventArgsPool();

	m_mousePosition.Set(clientX, clientY);

	// キャプチャ中のコントロールがあればそちらに送る
	if (m_capturedElement != NULL)
	{
		RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(MouseButton::None, 0, clientX, clientY, 0), false);
		return m_capturedElement->OnEvent(RoutedEventType::MouseMove, args);
	}
	//if (m_defaultRootFrame == NULL) { return false; }
	UpdateMouseHover(PointF(clientX, clientY));
	if (m_mouseHoverElement == NULL) { return false; }
	RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(MouseButton::None, 0, clientX, clientY, 0), false);
	//if (m_mouseHoverElement != NULL)
	return m_mouseHoverElement->OnEvent(RoutedEventType::MouseMove, args);
	//bool r = m_defaultRootFrame->OnEvent(EventType_MouseMove, args);
	//return UpdateMouseHover(PointF(clientX, clientY));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::InjectMouseButtonDown(MouseButton button, float clientX, float clientY)
{
	EventArgsPool* pool = m_manager->GetEventArgsPool();

	m_mousePosition.Set(clientX, clientY);

	// マウスクリック回数の処理
	MouseClickTracker& tracker = m_mouseClickTrackers[button];
	tracker.ClickCount++;

	double curTime = 0.001 * Environment::GetTickCount();
	float elapsed = (float)(curTime - tracker.LastTime);
	if (elapsed > m_mouseButtonClickTimeout ||
		m_mouseHoverElement != tracker.HoverElement ||
		tracker.ClickCount > 3)
	{
		tracker.ClickCount = 1;
		tracker.HoverElement = m_mouseHoverElement;
	}
	tracker.LastTime = curTime;



	// キャプチャ中のコントロールがあればそちらに送る
	if (m_capturedElement != NULL)
	{
		RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(button, 0, clientX, clientY, tracker.ClickCount), false);
		return m_capturedElement->OnEvent(RoutedEventType::MouseButtonDown, args);
	}
	if (m_mouseHoverElement == NULL) { return false; }
	RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(button, 0, clientX, clientY, tracker.ClickCount), false);
	//if (m_mouseHoverElement != NULL) {
	return m_mouseHoverElement->OnEvent(RoutedEventType::MouseButtonDown, args);
	//}
	//return m_defaultRootFrame->OnEvent(EventType_MouseButtonDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::InjectMouseButtonUp(MouseButton button, float clientX, float clientY)
{
	EventArgsPool* pool = m_manager->GetEventArgsPool();

	m_mousePosition.Set(clientX, clientY);

	// キャプチャ中のコントロールがあればそちらに送る
	if (m_capturedElement != NULL)
	{
		RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(button, 0, clientX, clientY, 0), false);
		return m_capturedElement->OnEvent(RoutedEventType::MouseButtonUp, args);
	}
	//if (m_defaultRootFrame == NULL) { return false; }
	if (m_mouseHoverElement == NULL) { return false; }
	RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(button, 0, clientX, clientY, 0), false);
	//return m_defaultRootFrame->OnEvent(EventType_MouseButtonUp, args);
	return m_mouseHoverElement->OnEvent(RoutedEventType::MouseButtonUp, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::InjectMouseWheel(int delta, float clientX, float clientY)
{
	EventArgsPool* pool = m_manager->GetEventArgsPool();

	m_mousePosition.Set(clientX, clientY);

	// キャプチャ中のコントロールがあればそちらに送る
	if (m_capturedElement != NULL)
	{
		RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(MouseButton::None, 0, clientX, clientY, 0), false);
		return m_capturedElement->OnEvent(RoutedEventType::MouseMove, args);
	}
	//if (m_defaultRootFrame == NULL) { return false; }
	if (m_mouseHoverElement == NULL) { return false; }
	RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(MouseButton::None, delta, clientX, clientY, 0), false);
	//return m_defaultRootFrame->OnEvent(EventType_MouseWheel, args);
	return m_mouseHoverElement->OnEvent(RoutedEventType::MouseWheel, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_focusElement == NULL) { return false; }
	EventArgsPool* pool = m_manager->GetEventArgsPool();
	RefPtr<KeyEventArgs> args(pool->CreateKeyEventArgs(keyCode, isAlt, isShift, isControl), false);
	return m_focusElement->OnEvent(RoutedEventType::KeyDown, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_focusElement == NULL) { return false; }
	EventArgsPool* pool = m_manager->GetEventArgsPool();
	RefPtr<KeyEventArgs> args(pool->CreateKeyEventArgs(keyCode, isAlt, isShift, isControl), false);
	return m_focusElement->OnEvent(RoutedEventType::KeyUp, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::InjectTextInput(TCHAR ch)
{
	if (m_focusElement == NULL) { return false; }
	EventArgsPool* pool = m_manager->GetEventArgsPool();
	RefPtr<KeyEventArgs> args(pool->CreateKeyEventArgs(Key::Unknown, false, false, false), false);
	args->Char = ch;
	return m_focusElement->OnEvent(RoutedEventType::TextInput, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::InjectElapsedTime(float elapsedTime)
{
	m_time += elapsedTime;

	for (AnimationClock* clock : m_activeAnimationClockList)
	{
		clock->AdvanceTime(elapsedTime);
	}

	m_activeAnimationClockList.RemoveAll([](AnimationClock* clock) { return clock->IsFinished(); });

	// マウスカーソルのアニメーション
	if (m_currentCursorImage != NULL)
	{
		m_cursorAnimationTime += elapsedTime;
		if (m_cursorAnimationTime >= m_currentCursorImage->GetPatternDuration()) {
			m_cursorAnimationTime = 0;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& GUIContext::GetViewPixelSize() const
{
	return m_viewPixelSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::UpdateLayout()
{
	//m_viewPixelSize = viewSize;	// この後の UIElement::UpdateLayout() で参照される

	if (m_rootElement != NULL)
	{
		m_rootElement->ApplyTemplate();
		m_rootElement->UpdateLayout();
		m_rootElement->UpdateTransformHierarchy();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::Render()
{
	/*	以前はここでカレントのレンダーターゲットを取得し、そこからサイズを取得していた。
	しかしそれだと他アプリに組み込む仕組みを作るのに少し面倒なことになる。
	*/
	Internal::GUIPainter* painter = m_manager->GetPainter();
	painter->SetProjection(m_viewPixelSize);

	painter->ResetState();
	if (m_rootElement != NULL) {
		m_rootElement->Render();
	}

	// マウスカーソル
	if (m_currentCursorImage != NULL)
	{
		//painter->ResetState();
		//m_currentCursorImage->Draw(painter, m_mousePosition, m_cursorAnimationTime);
	}

	painter->Flush();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::SetFocusElement(UIElement* element)
{
	m_focusElement = element;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::CaptureMouse(UIElement* element)
{
	m_capturedElement = element;
	m_nativeWindow->CaptureMouse();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::ReleaseMouseCapture(UIElement* element)
{
	if (m_capturedElement == element)
	{
		m_capturedElement = NULL;
		m_nativeWindow->ReleaseMouseCapture();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GUIContext::UpdateMouseHover(const PointF& mousePos)
{
	EventArgsPool* pool = m_manager->GetEventArgsPool();
	UIElement* old = m_mouseHoverElement;

	// TODO:IME側のイベントを処理する
	//if ( m_pIme != NULL )
	//{
	//	if ( m_pIme->OnMouseHoverCheck( m_MousePosition, &mMouseHoverControl ) )
	//	{
	//		goto EXIT;
	//	}
	//}

	// 通常のウィンドウのイベントを処理する
	if (m_rootElement != NULL)
	{
		m_mouseHoverElement = m_rootElement->CheckMouseHoverElement(mousePos);
		if (m_mouseHoverElement != NULL) {
			goto EXIT;
		}
	}

	m_mouseHoverElement = NULL;

EXIT:
	// 新旧それぞれの Element に MouseLeave、MouseEnter イベントを送る
	if (m_mouseHoverElement != old)
	{
		RefPtr<MouseEventArgs> args(pool->CreateMouseEventArgs(MouseButton::None, 0, mousePos.X, mousePos.Y, 0), false);
		if (old != NULL) {
			old->OnEvent(RoutedEventType::MouseLeave, args);
		}

		// EventType_MouseLeave とで使いまわしているのでリセットを忘れずに
		args->Handled = false;

		if (m_mouseHoverElement != NULL) {
			return m_mouseHoverElement->OnEvent(RoutedEventType::MouseEnter, args);
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::AddAnimationClock(AnimationClock* clock)
{
	m_activeAnimationClockList.Add(clock);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GUIContext::RemoveAnimationClock(AnimationClock* clock)
{
	m_activeAnimationClockList.Remove(clock);
}


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
