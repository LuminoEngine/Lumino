#pragma once

namespace ln {
namespace detail {

class AnimationManager
	: public RefObject
{
public:
	struct Settings
	{
	};

    AnimationManager();
	virtual ~AnimationManager();
	void initialize(const Settings& settings);
	void dispose();

    void updateFrame(float elapsedSeconds);

private:
};

} // namespace detail
} // namespace ln

