
#pragma once
#include <Lumino/UI/Common.h>

LN_NAMESPACE_BEGIN
namespace Documents { class DocumentsManager; }
namespace detail
{
class EventArgsPool;

class UIManager
	: public RefObject
{
public:

	struct Settings
	{
		GraphicsManager*				graphicsManager = nullptr;
		PlatformWindow*					mainWindow = nullptr;
		Documents::DocumentsManager*	documentsManager = nullptr;
	};

	static const float MouseButtonClickTimeout;

public:
	UIManager();
	virtual ~UIManager();

	void Initialize(const Settings& settings);
	void Finalize();
	EventArgsPool* GetEventArgsPool() { return m_eventArgsPool; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	UIWindow* GetMainWindow() { return m_mainWindow; }
	UIContext* GetDefaultUIContext() { return m_defaultUIContext; }

private:
	EventArgsPool*		m_eventArgsPool;
	GraphicsManager*	m_graphicsManager;
	UIWindow*			m_mainWindow;
	UIContext*			m_defaultUIContext;
};

} // namespace detail
LN_NAMESPACE_END
