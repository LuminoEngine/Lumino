#pragma once
#include "AudioNode.hpp"

namespace ln {

class AudioPannerNode
	: public AudioNode
{
public:

LN_CONSTRUCT_ACCESS:
	AudioPannerNode();
	virtual ~AudioPannerNode() = default;
	void init();
	virtual detail::AudioNodeCore* coreNode() override;

private:
	Ref<detail::CoreAudioPannerNode> m_coreObject;
};

} // namespace ln

