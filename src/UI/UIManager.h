
#pragma once
#include <Lumino/UI/Common.h>

LN_NAMESPACE_BEGIN
namespace Documents { class DocumentsManager; }
class AssetsManager;

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
		AssetsManager*					assetsManager = nullptr;
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
	UIStyleTable* GetDefaultStyleTable() { return m_defaultStyleTable; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	AssetsManager* GetAssetsManager() { return m_assetsManager; }
	UIMainWindow* GetMainWindow() { return m_mainWindow; }

private:
	void MakeDefaultStyle(UIStyleTable* table);

	EventArgsPool*		m_eventArgsPool;
	GraphicsManager*	m_graphicsManager;
	AssetsManager*		m_assetsManager;
	UIStyleTable*		m_defaultStyleTable;
	UIMainWindow*		m_mainWindow;
};

} // namespace detail
LN_NAMESPACE_END
