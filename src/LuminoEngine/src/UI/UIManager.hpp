#pragma once

namespace ln {
class UIContext;
class UIContainerElement;
class UIFrameLayout2;
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
	};

    static const float MouseButtonClickTimeout;

	UIManager();
	virtual ~UIManager() = default;

	void init(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

    void setPrimaryElement(UIContainerElement* element);
    UIContainerElement* primaryElement() const;

    const Ref<EventArgsPool>& eventArgsPool() const { return m_eventArgsPool; }

    void setMainContext(UIContext* context);
    const Ref<UIContext>& mainContext() const { return m_mainContext; }

    bool updateMouseHover(UIFrameWindow* mouseEventSource, const Point& frameClientPosition);
    UIElement* mouseHoverElement() const { return m_mouseHoverElement; }

	void retainCapture(UIElement* element);
	void releaseCapture(UIElement* element);
	UIElement* capturedElement() const { return m_capturedElement; }


	//const Ref<UIFrameLayout2>& defaultLayout() const { return m_defaultLayout; }

private:
	GraphicsManager* m_graphicsManager;
	//PlatformManager* platformManager;
    Ref<UIContainerElement> m_primaryElement;
    Ref<EventArgsPool> m_eventArgsPool;
    Ref<UIContext> m_mainContext;
    UIElement* m_mouseHoverElement;
	UIElement* m_capturedElement;
	//Ref<UIFrameLayout2> m_defaultLayout;
};

} // namespace detail
} // namespace ln

