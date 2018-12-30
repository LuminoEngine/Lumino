#pragma once

namespace ln {
class UIContext;
class UIContainerElement;
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

	void initialize(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

    void setPrimaryElement(UIContainerElement* element);
    UIContainerElement* primaryElement() const;

    const Ref<EventArgsPool>& eventArgsPool() const { return m_eventArgsPool; }

    void setMainContext(UIContext* context);
    const Ref<UIContext>& mainContext() const { return m_mainContext; }

private:
	GraphicsManager* m_graphicsManager;
	//PlatformManager* platformManager;
    Ref<UIContainerElement> m_primaryElement;
    Ref<EventArgsPool> m_eventArgsPool;
    Ref<UIContext> m_mainContext;
};

} // namespace detail
} // namespace ln

