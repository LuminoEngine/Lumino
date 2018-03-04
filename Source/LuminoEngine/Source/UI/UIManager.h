
#pragma once
#include <Lumino/UI/Common.h>

LN_NAMESPACE_BEGIN
class FileManager;
class AssetsManager;
class PlatformManager;
class PlatformWindow;
class World2D;
class World3D;
class UIRoutedCommand;

namespace detail
{
class EventArgsPool;
class AnimationManager;
class DocumentsManager;

class UIManager
	: public RefObject
{
public:
	struct Settings
	{
		FileManager*			fileManager = nullptr;
		AnimationManager*		animationManager = nullptr;
		PlatformManager*		platformManager = nullptr;
		GraphicsManager*		graphicsManager = nullptr;
		AssetsManager*			assetsManager = nullptr;
		DocumentsManager*		documentsManager = nullptr;
		PathName				defaultSkinFilePath;
	};

	static const float MouseButtonClickTimeout;

public:
	static UIManager* getInstance(UIManager* priority = nullptr);

	UIManager();
	virtual ~UIManager();

	void initialize(const Settings& settings);
	void dispose();
	//void createGameModeMainFrame(World2D* defaultWorld2D, World3D* defaultWorld3D);
	////void CreateWrapModeMainFrame(void* window, World2D* defaultWorld2D, World3D* defaultWorld3D);
	//void releaseGameModeMainFrame();
	void setMainWindow(UIMainWindow* window) { m_mainWindow = window; }

	EventArgsPool* getEventArgsPool() const { return m_eventArgsPool; }
	const Ref<Texture2D>& getDefaultSkinTexture() const { return m_defaultSkinTexture; }
	UIStyleTable* getDefaultStyleTable() const { return m_defaultStyleTable; }
	AnimationManager* getAnimationManager() const { return m_animationManager; }
	PlatformManager* getPlatformManager() const { return m_platformManager; }
	GraphicsManager* getGraphicsManager() const { return m_graphicsManager; }
	AssetsManager* getAssetsManager() const { return m_assetsManager; }
	UIMainWindow* getMainWindow() const { return m_mainWindow; }

	void captureMouse(UIElement* element);
	void releaseMouseCapture(UIElement* element);
	UIElement* getMouseCapturedElement() const { return m_mouseCapturedElement; }

	const List<UIFrameWindow*>& getWindows() const { return m_windows; }
	void addFrameWindow(UIFrameWindow* window) { m_windows.add(window); }
	void removeFrameWindow(UIFrameWindow* window) { m_windows.remove(window); }

private:
	void createGlobalObjects();
	void makeDefaultStyle(UIStyleTable* table);

	EventArgsPool*		m_eventArgsPool;
	FileManager*		m_fileManager;
	AnimationManager*	m_animationManager;
	PlatformManager*	m_platformManager;
	GraphicsManager*	m_graphicsManager;
	AssetsManager*		m_assetsManager;
	Ref<Texture2D>	m_defaultSkinTexture;
	UIStyleTable*		m_defaultStyleTable;
	UIMainWindow*		m_mainWindow;
	List<Ref<UIRoutedCommand>>		m_allGlobalCommands;
	List<UIFrameWindow*>	m_windows;

	UIElement*				m_mouseCapturedElement;
};

} // namespace detail
LN_NAMESPACE_END
