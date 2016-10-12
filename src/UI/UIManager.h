
#pragma once
#include <Lumino/UI/Common.h>

LN_NAMESPACE_BEGIN
class AssetsManager;
class PlatformManager;
class PlatformWindow;

namespace detail
{
class EventArgsPool;
class AnimationManager;

class UIManager
	: public RefObject
{
public:
	struct Settings
	{
		AnimationManager*				animationManager = nullptr;
		PlatformManager*				platformManager = nullptr;
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
	EventArgsPool* GetEventArgsPool() const { return m_eventArgsPool; }
	UIStyleTable* GetDefaultStyleTable() const { return m_defaultStyleTable; }
	AnimationManager* GetAnimationManager() const { return m_animationManager; }
	PlatformManager* GetPlatformManager() const { return m_platformManager; }
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	AssetsManager* GetAssetsManager() const { return m_assetsManager; }
	UIMainWindow* GetMainWindow() const { return m_mainWindow; }

private:
	void MakeDefaultStyle(UIStyleTable* table);

	EventArgsPool*		m_eventArgsPool;
	AnimationManager*	m_animationManager;
	PlatformManager*	m_platformManager;
	GraphicsManager*	m_graphicsManager;
	AssetsManager*		m_assetsManager;
	RefPtr<Texture2D>	m_defaultSkinTexture;
	UIStyleTable*		m_defaultStyleTable;
	UIMainWindow*		m_mainWindow;
};

} // namespace detail
LN_NAMESPACE_END
