#pragma once
#include "../Engine/RenderingCommandList.hpp"

namespace ln {
class AudioContext;
namespace detail {
class AudioDecoder;

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

	const Ref<AudioContext>& primaryContext() const { return m_primaryContext; }
	RenderingType renderingType() const { return RenderingType::Immediate; }
	const Ref<RenderingCommandList>& primaryRenderingCommandList() const { return m_primaryRenderingCommandList; }

	Ref<AudioDecoder> createAudioDecoder(const StringRef& filePath);

private:
	Ref<AudioContext> m_primaryContext;
	Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	Ref<RenderingCommandList> m_primaryRenderingCommandList;
};

} // namespace detail
} // namespace ln

