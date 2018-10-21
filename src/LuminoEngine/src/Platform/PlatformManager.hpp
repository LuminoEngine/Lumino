#pragma once
#include "PlatformWindowManager.hpp"

namespace ln {
namespace detail {
class PlatformWindowManager;

class PlatformManager
	: public RefObject
{
public:
	struct Settings
	{
		WindowCreationSettings	mainWindowSettings;
	};

	PlatformManager();
	virtual ~PlatformManager() = default;

	void initialize(const Settings& settings);
	void dispose();

	const Ref<PlatformWindowManager>& windowManager() const { return m_windowManager; }
	const Ref<PlatformWindow>& mainWindow() const { return m_mainWindow; }

private:
	Ref<PlatformWindowManager> m_windowManager;
	Ref<PlatformWindow> m_mainWindow;	// v0.5.0 で持たないことを検討したが、Graphics, UI との初期化順の関係や、Android, Emscripten など既に出来上がっている View にアタッチしたいときなどに欲しい
};

} // namespace detail
} // namespace ln

