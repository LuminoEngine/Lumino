#pragma once

namespace ln {
namespace detail {
class AudioContext;

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
	void update();

private:
	Ref<AudioContext> m_audioContext;
};

} // namespace detail
} // namespace ln

