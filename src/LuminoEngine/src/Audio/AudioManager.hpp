#pragma once

namespace ln {
namespace detail {

class AudioManager
	: public RefObject
{
public:
	struct Settings
	{
	};

	AudioManager();
	virtual ~AudioManager() = default;
	void initialize(const Settings& settings);
	void dispose();

private:
};

} // namespace detail
} // namespace ln

