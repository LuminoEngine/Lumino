
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
	void Finalize();
	void CreateGameModeMainFrame(World2D* defaultWorld2D, World3D* defaultWorld3D);
	//void CreateWrapModeMainFrame(void* window, World2D* defaultWorld2D, World3D* defaultWorld3D);
	void ReleaseGameModeMainFrame();
	EventArgsPool* GetEventArgsPool() const { return m_eventArgsPool; }
	const RefPtr<Texture2D>& GetDefaultSkinTexture() const { return m_defaultSkinTexture; }
	UIStyleTable* GetDefaultStyleTable() const { return m_defaultStyleTable; }
	AnimationManager* getAnimationManager() const { return m_animationManager; }
	PlatformManager* getPlatformManager() const { return m_platformManager; }
	GraphicsManager* getGraphicsManager() const { return m_graphicsManager; }
	AssetsManager* GetAssetsManager() const { return m_assetsManager; }
	UIMainWindow* getMainWindow() const { return m_mainWindow; }

	void CaptureMouse(UIElement* element);
	void ReleaseMouseCapture(UIElement* element);
	UIElement* GetMouseCapturedElement() const { return m_mouseCapturedElement; }

	const List<UIFrameWindow*>& GetWindows() const { return m_windows; }
	void AddFrameWindow(UIFrameWindow* window) { m_windows.add(window); }
	void RemoveFrameWindow(UIFrameWindow* window) { m_windows.remove(window); }

private:
	void createGlobalObjects();
	void MakeDefaultStyle(UIStyleTable* table);

	EventArgsPool*		m_eventArgsPool;
	FileManager*		m_fileManager;
	AnimationManager*	m_animationManager;
	PlatformManager*	m_platformManager;
	GraphicsManager*	m_graphicsManager;
	AssetsManager*		m_assetsManager;
	RefPtr<Texture2D>	m_defaultSkinTexture;
	UIStyleTable*		m_defaultStyleTable;
	UIMainWindow*		m_mainWindow;
	List<RefPtr<UIRoutedCommand>>		m_allGlobalCommands;
	List<UIFrameWindow*>	m_windows;

	UIElement*				m_mouseCapturedElement;
};

} // namespace detail
LN_NAMESPACE_END
