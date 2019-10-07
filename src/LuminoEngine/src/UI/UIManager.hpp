#pragma once

namespace ln {
class Application;
class UIContext;
class UIContainerElement;
class UIFrameLayout;
class UIActiveTimer;
class UIRenderView;

namespace detail {
class PlatformManager;
class GraphicsManager;
class EventArgsPool;

class UIManager
	: public RefObject
{
public:
	struct Settings
	{
		GraphicsManager* graphicsManager;
        Application* application = nullptr;
	};

    static const float MouseButtonClickTimeout;

	UIManager();
	virtual ~UIManager() = default;

	void init(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }
    Application* application() const { return m_application; }

    void setPrimaryElement(UIContainerElement* element);
    UIContainerElement* primaryElement() const;

    const Ref<EventArgsPool>& eventArgsPool() const { return m_eventArgsPool; }

    void setMainContext(UIContext* context);
    const Ref<UIContext>& mainContext() const { return m_mainContext; }

    bool updateMouseHover(UIRenderView* mouseEventSource, const Point& frameClientPosition);
    UIElement* mouseHoverElement() const { return m_mouseHoverElement; }

	void retainCapture(UIElement* element);
	void releaseCapture(UIElement* element);
	UIElement* capturedElement() const { return m_capturedElement; }

    void focus(UIElement* element);
    UIElement* forcusedElement() const { return m_forcusedElement; }

    void postEvent(UIElement* target, UIEventArgs* e);
    void dispatchPostedEvents();

    void handleGlobalRoutedEvent(UIEventArgs* e);

	//const Ref<UIFrameLayout>& defaultLayout() const { return m_defaultLayout; }

	void registerActiveTimer(UIActiveTimer* timer);
	void unregisterActiveTimer(UIActiveTimer* timer);
	void tickGlobal(float elapsedSeconds);

private:
    struct EventQueueItem
    {
        Ref<UIElement> target;  // 送信待ち中の削除に備え、強参照で持つ
        Ref<UIEventArgs> args;
    };

	GraphicsManager* m_graphicsManager;
    Application* m_application;
	//PlatformManager* platformManager;
    Ref<UIContainerElement> m_primaryElement;
    Ref<EventArgsPool> m_eventArgsPool;
    Ref<UIContext> m_mainContext;
    UIElement* m_mouseHoverElement;
	UIElement* m_capturedElement;
    UIElement* m_forcusedElement;
	//Ref<UIFrameLayout> m_defaultLayout;
    std::deque<EventQueueItem> m_eventQueue;
	List<Ref<UIActiveTimer>> m_activeTimers;

};

} // namespace detail
} // namespace ln

