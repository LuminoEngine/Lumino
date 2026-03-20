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
	virtual detail::ARINode* coreNode() override;

private:
	Ref<detail::ARIPannerNode> m_coreObject;
};

} // namespace ln

