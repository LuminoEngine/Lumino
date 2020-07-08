#pragma once
#include "InternalSharedMutex.inc"

namespace ln {
namespace detail {
class AudioManager;
class AudioDevice;
class ARIDestinationNode;
class ARINode;
class SoundCore;
} // namespace detail
class AudioNode;
class AudioDestinationNode;
class Sound;

class AudioContext
	: public RefObject
{
public:
	static AudioContext* primary();	// TODO: move Engine 

	AudioContext();
	virtual ~AudioContext() = default;
	void init();
	virtual void dispose();
	void updateFrame();

	void processOnAudioThread(float elapsedSeconds);

    /** この AudioContext 内でベースとして使われるサンプルレート(1秒あたりのサンプル数)を取得します。 */
    int sampleRate() const;

	AudioDestinationNode* destination() const;

	void addAudioNode(AudioNode* node);
	void removeAudioNode(AudioNode* node);
	void removeAllNodes();

	// TODO: internal
	detail::AudioManager* manager() const { return m_manager; }
	//void markGC(detail::ARINode* node);	// call only audio thread.

	detail::AudioDevice* coreObject();

    //void addSound(Sound* sound);

LN_INTERNAL_ACCESS:
	detail::AudioRWMutex m_commitMutex;
	detail::AudioRWMutex& commitMutex() { return m_commitMutex; }

	//void disposeNodeOnGenericThread(AudioNode* node);	// Audio Thread 以外での AudioNode::dispose
	//void tryRemoveAudioNode(AudioNode* node);

private:
	void addAudioNodeInternal(const Ref<detail::ARINode>& node);
	void removeAudioNodeInternal(const Ref<detail::ARINode>& node);

	// call by aduio thread.
	void commitGraphs(float elapsedSeconds);

	detail::AudioManager* m_manager;
	Ref<detail::AudioDevice> m_audioDevice;
	Ref<detail::ARIDestinationNode> m_coreDestinationNode;
	Ref<AudioDestinationNode> m_destinationNode;

	// 強参照。ユーザープログラムからの明示的な破棄が必要。
	// Non-ThreadSafe.
	List<Ref<AudioNode>> m_audioNodes;

	// 強参照。AudioThread で受信したコマンドによって add/remove
	List<Ref<detail::ARINode>> m_coreAudioNodes;


	//List<AudioNode*> m_allAudioNodes;
	//List<Ref<AudioNode>> m_aliveNodes;
	//List<AudioNode*> m_markedNodes;

	// commitGraphs() 中、m_allAudioNodes の AudioNode のインスタンスが消えないように参照を持っておくための list
	//List<Ref<AudioNode>> m_allAudioNodes_onCommit;


    //List<WeakRefPtr<Sound>> m_soundList;

	friend class detail::AudioManager;
	friend class detail::SoundCore;
};

} // namespace ln

