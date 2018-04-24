#pragma once

namespace ln {
namespace detail {
class PlatformManager;

class UIManager
	: public RefObject
{
public:
	struct Settings
	{
		//PlatformManager* platformManager;
	};

	UIManager();
	virtual ~UIManager() = default;

	void initialize(const Settings& settings);
	void dispose();

private:
	//PlatformManager* platformManager;
};

} // namespace detail
} // namespace ln

