#pragma once
#include <shared_mutex>
#include "InternalSharedMutex.inc"

namespace ln {
class AudioContext;
namespace detail {
class AudioDecoder;
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
	static void disconnect(AudioNode* outputSide, AudioNode* inputSide);

	AudioContext* context() const { return m_context; }

	/** このノードをの全ての接続を解除します。 */
	void disconnect();

    virtual void onDispose(bool explicitDisposing) override;
protected:
	AudioNode();
	virtual ~AudioNode() = default;
	void init();
    // init の中で context に addAudioNode() するが、その時点で初期化処理は完全に終わっていなければならない。
    // そうしないと、addAudioNode() に入った瞬間 AudioThread で commit が走ったときに、未初期化の変数にアクセスしてしまう。
    // onInitialize() はそういった事情を考慮して、セーフティなタイミングで初期化を行うためのコールバック。
    // 逆に言うと、AudioNode のサブクラスは他のモジュールのように init() は一切実装してはならない。
    //virtual void onInitialize() = 0;
	virtual detail::CoreAudioNode* coreNode() = 0;
	virtual void commit();	// ロック済みの状態で呼ばれる

	detail::AudioRWMutex& commitMutex();
    detail::AudioRWMutex& propertyMutex() { return m_propertyMutex; }

private:
	//void addConnectionInput(AudioNode* inputSide);
	//void addConnectionOutput(AudioNode* outputSide);
	//void removeConnectionInput(AudioNode* inputSide);
	//void removeConnectionOutput(AudioNode* outputSide);

	AudioContext* m_context;
	//List<Ref<AudioNode>> m_inputConnections;	// input side in this node
	//List<Ref<AudioNode>> m_outputConnections;	// output side in this node
	//bool m_inputConnectionsDirty;
	//bool m_outputConnectionsDirty;
    detail::AudioRWMutex m_propertyMutex;

	friend class AudioContext;
};

class AudioSourceNode
	: public AudioNode
{
public:
    enum class PlayingState
    {
        NoChanged,
        Stop,
        Play,
        Pause,
    };

	void setPlaybackRate(float rate);

    void setLoop(bool value);
    bool loop() const;

	void start();
	void stop();
	void pause();
	void resume();
    PlayingState playingState() const;

LN_CONSTRUCT_ACCESS:
	AudioSourceNode();
	virtual ~AudioSourceNode() = default;
	void init(detail::AudioDecoder* decoder);
	virtual detail::CoreAudioNode* coreNode() override;
	virtual void commit() override;

private:

	struct CommitState
	{
		float playbackRate;
		PlayingState requestedState;
        bool loop;
		bool resetRequire;
	};

	Ref<detail::CoreAudioSourceNode> m_coreObject;
	CommitState m_commitState;
};

class AudioPannerNode
	: public AudioNode
{
public:

LN_CONSTRUCT_ACCESS:
	AudioPannerNode();
	virtual ~AudioPannerNode() = default;
	void init();
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
	void init(detail::CoreAudioDestinationNode* core);
	virtual detail::CoreAudioNode* coreNode() override;

private:
	Ref<detail::CoreAudioDestinationNode> m_coreObject;
};

} // namespace ln

