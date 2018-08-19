#pragma once

namespace ln {
class AudioContext;
namespace detail {
class CoreAudioNode;
class CoreAudioSourceNode;
class CoreAudioDestinationNode;
} // namespace detail

class AudioNode
	: public Object
{
public:
	// in=1, out=1 用のユーティリティ
	static void connect(AudioNode* outputSide, AudioNode* inputSide);

	AudioContext* context() const { return m_context; }

protected:
	AudioNode();
	virtual ~AudioNode() = default;
	void initialize();
	virtual detail::CoreAudioNode* coreNode() = 0;

private:
	AudioContext* m_context;
};

class AudioSourceNode
	: public AudioNode
{
public:
	void setPlaybackRate(float rate);

	void start();
	void stop();
	void pause();
	void resume();

LN_CONSTRUCT_ACCESS:
	AudioSourceNode();
	virtual ~AudioSourceNode() = default;
	void initialize(const StringRef& filePath);
	virtual detail::CoreAudioNode* coreNode() override;

private:
	Ref<detail::CoreAudioSourceNode> m_coreObject;
};

class AudioDestinationNode
	: public AudioNode
{
LN_CONSTRUCT_ACCESS:
	AudioDestinationNode();
	virtual ~AudioDestinationNode() = default;
	void initialize(detail::CoreAudioDestinationNode* core);
	virtual detail::CoreAudioNode* coreNode() override;

private:
	Ref<detail::CoreAudioDestinationNode> m_coreObject;
};

} // namespace ln

