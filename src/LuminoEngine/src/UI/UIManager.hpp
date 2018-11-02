#pragma once

namespace ln {
class UIContainerElement;
namespace detail {
class PlatformManager;
class GraphicsManager;

class UIManager
	: public RefObject
{
public:
	struct Settings
	{
		GraphicsManager* graphicsManager;
	};

	UIManager();
	virtual ~UIManager() = default;

	void initialize(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

    void setPrimaryElement(UIContainerElement* element);
    UIContainerElement* primaryElement() const;

private:
	GraphicsManager* m_graphicsManager;
	//PlatformManager* platformManager;
    Ref<UIContainerElement> m_primaryElement;
};

} // namespace detail
} // namespace ln

