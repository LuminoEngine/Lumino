
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
	static UIManager* GetInstance(UIManager* priority = nullptr);

	UIManager();
	virtual ~UIManager();

	void Initialize(const Settings& settings);
	void Finalize();
	EventArgsPool* GetEventArgsPool() { return m_eventArgsPool; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	UIMainWindow* GetMainWindow() { return m_mainWindow; }

private:
	EventArgsPool*		m_eventArgsPool;
	GraphicsManager*	m_graphicsManager;
	UIMainWindow*		m_mainWindow;
};

} // namespace detail
LN_NAMESPACE_END
