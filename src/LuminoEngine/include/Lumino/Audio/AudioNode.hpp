#pragma once
#include <shared_mutex>

#if LN_AUDIO_THREAD_ENABLED
#define LN_AUDIO_WRITE_LOCK_COMMIT	std::shared_lock<std::shared_mutex> lock(commitMutex());
#define LN_AUDIO_READ_LOCK_COMMIT	std::shared_lock<std::shared_mutex> lock(commitMutex());
#else
#define LN_AUDIO_WRITE_LOCK_COMMIT
#define LN_AUDIO_READ_LOCK_COMMIT
#endif

namespace ln {
class AudioContext;
namespace detail {
class CoreAudioNode;
class CoreAudioSourceNode;
class CoreAudioPannerNode;
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
	virtual void dispose();
	virtual detail::CoreAudioNode* coreNode() = 0;
	virtual void commit();	// ロック済みの状態で呼ばれる

#if LN_AUDIO_THREAD_ENABLED
	std::shared_mutex& commitMutex();
#endif

private:
	void addConnectionInput(AudioNode* inputSide);
	void addConnectionOutput(AudioNode* outputSide);

	AudioContext* m_context;
	List<Ref<AudioNode>> m_inputConnections;	// input side in this node
	List<Ref<AudioNode>> m_outputConnections;	// output side in this node
	bool m_inputConnectionsDirty;
	bool m_outputConnectionsDirty;

	friend class AudioContext;
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

class AudioPannerNode
	: public AudioNode
{
public:

LN_CONSTRUCT_ACCESS:
	AudioPannerNode();
	virtual ~AudioPannerNode() = default;
	void initialize();
	virtual detail::CoreAudioNode* coreNode() override;

private:
	Ref<detail::CoreAudioPannerNode> m_coreObject;
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

