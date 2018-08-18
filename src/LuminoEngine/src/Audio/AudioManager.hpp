#pragma once
#include "../Engine/RenderingCommandList.hpp"

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

	RenderingType renderingType() const { return RenderingType::Immediate; }
	const Ref<RenderingCommandList>& primaryRenderingCommandList() const { return m_primaryRenderingCommandList; }

private:
	Ref<AudioContext> m_audioContext;
	Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	Ref<RenderingCommandList> m_primaryRenderingCommandList;
};

} // namespace detail
} // namespace ln

