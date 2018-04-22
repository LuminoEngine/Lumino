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

private:
	Ref<PlatformWindowManager> m_windowManager;
};

} // namespace detail
} // namespace ln

