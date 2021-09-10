#pragma once
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoPlatform/PlatformModule.hpp>
#include "PlatformWindowManager.hpp"

namespace ln {
namespace detail {
class PlatformWindowManager;

class PlatformManager
	: public PlatformModule
{
public:
	struct Settings
	{
        bool useGLFWWindowSystem = true;
		bool glfwWithOpenGLAPI = false;

		WindowCreationSettings	mainWindowSettings;
	};

	//static bool initialize();
	//static void terminate();
	static inline PlatformManager* instance() { return static_cast<PlatformManager*>(EngineContext2::instance()->platformManager); }


	PlatformManager();
	virtual ~PlatformManager() = default;

	Result init(const Settings& settings);
	void dispose();

	const Ref<PlatformWindowManager>& windowManager() const { return m_windowManager; }
	const Ref<PlatformWindow>& mainWindow() const { return m_mainWindow; }
	bool glfwWithOpenGLAPI() const { return m_glfwWithOpenGLAPI; }
	OpenGLContext* openGLContext() const;
	void processSystemEventQueue();

private:
	//static Ref<PlatformManager> s_instance;

	Ref<PlatformWindowManager> m_windowManager;
	Ref<PlatformWindow> m_mainWindow;	// v0.5.0 で持たないことを検討したが、Graphics, UI との初期化順の関係や、Android, Emscripten など既に出来上がっている View にアタッチしたいときなどに欲しい
	bool m_glfwWithOpenGLAPI;
	bool m_messageLoopProcessing;
};

} // namespace detail
} // namespace ln

