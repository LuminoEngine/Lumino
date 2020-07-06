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
        String defaultThemeName;
	};

	struct CommonInputCommands
	{
		Ref<UICommand> left;
		Ref<UICommand> right;
		Ref<UICommand> up;
		Ref<UICommand> down;
		Ref<UICommand> submit;
		Ref<UICommand> cancel;
		Ref<UICommand> menu;
		Ref<UICommand> shift;
		Ref<UICommand> pageUp;
		Ref<UICommand> pageDown;
	};

    static const float MouseButtonClickTimeout;

	UIManager();
	virtual ~UIManager() = default;

	void init(const Settings& settings);
	void dispose();

    void onElementDisposing(UIElement* element);

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }
    Application* application() const { return m_application; }
    const String& defaultThemeName() const { return m_defaultThemeName; }

    void setPrimaryElement(UIControl* element);
	UIControl* primaryElement() const;

    const Ref<EventArgsPool>& eventArgsPool() const { return m_eventArgsPool; }

    void setMainContext(UIContext* context);
    const Ref<UIContext>& mainContext() const { return m_mainContext; }

    void updateMouseHover(UIRenderView* mouseEventSource, const Point& frameClientPosition);
    const Ref<UIElement>& mouseHoverElement() const { return m_mouseHoverElement; }

	void retainCapture(UIElement* element);
	void releaseCapture(UIElement* element);
	const Ref<UIElement>& capturedElement() const { return m_capturedElement; }

    void tryGetInputFocus(UIElement* element);
	const Ref<UIElement>& forcusedElement() const { return m_forcusedElement; }
	void activateTree(UIElement* element);

    void postEvent(UIElement* target, UIEventArgs* e);
    void dispatchPostedEvents();

    void handleGlobalRoutedEvent(UIEventArgs* e);

	//const Ref<UIFrameLayout>& defaultLayout() const { return m_defaultLayout; }

	void registerActiveTimer(UIActiveTimer* timer);
	void unregisterActiveTimer(UIActiveTimer* timer);
	void tickGlobal(float elapsedSeconds);

    void clearMouseHover();
    void clearFocus();
    void handleDetachFromUITree(UIElement* element);

	const CommonInputCommands& commonInputCommands() const { return m_commonInputCommands; }
	bool handleCommonInputCommands(UIEventArgs* e);

	// ルートまで到達した Bubble イベントを処理する
	void handleRootBubbleEvent(UIEventArgs* e);

private:
    struct EventQueueItem
    {
        Ref<UIElement> target;  // 送信待ち中の削除に備え、強参照で持つ
        Ref<UIEventArgs> args;
    };

	GraphicsManager* m_graphicsManager;
    Application* m_application;
    String m_defaultThemeName;
	//PlatformManager* platformManager;
    Ref<UIControl> m_primaryElement;
    Ref<EventArgsPool> m_eventArgsPool;
    Ref<UIContext> m_mainContext;
    Ref<UIElement> m_mouseHoverElement;
	Ref<UIElement> m_capturedElement;
    Ref<UIElement> m_forcusedElement;
	//Ref<UIFrameLayout> m_defaultLayout;
    std::deque<EventQueueItem> m_eventQueue;
	List<Ref<UIActiveTimer>> m_activeTimers;
	CommonInputCommands m_commonInputCommands;
	List<Ref<UICommand>> m_inputCommands;

    // 
	List<Ref<UIElement>> m_activationCache;
};

} // namespace detail
} // namespace ln

