#pragma once
#include "CoreAudioNode.hpp"

namespace ln {
namespace detail {

class AudioSourceNodeCore
	: public AudioNodeCore
{
public:

protected:
	virtual void process() override;

public:
    // scheduling
    enum class PlayingState
    {
        None,
        Stopped,
        Playing,
        Pausing,
    };

	AudioSourceNodeCore(AudioDevice* context);
	virtual ~AudioSourceNodeCore() = default;
	void init(const Ref<AudioDecoder>& decoder);

	void setPlaybackRate(float rate);

    void setLoop(bool value) { m_loop = value; }
	bool loop() const { return m_loop; }

	
	void start();
	void stop();
	void reset();	// TODO: internal
	void finish();	// TODO: internal
    PlayingState playingState() const { return m_playingState; }

private:
	unsigned numberOfChannels() const;
	void resetSourceBuffers();
	double calculatePitchRate();
	bool renderSilenceAndFinishIfNotLooping(AudioBus * bus, unsigned index, size_t framesToProcess);
	void updatePlayingState();

	Ref<AudioDecoder> m_decoder;
	std::vector<float> m_readBuffer;
	Ref<AudioBus> m_sourceBus;	// resampled
	std::unique_ptr<blink::SincResampler> m_resampler;
	Ref<AudioBus> m_resamplingBus;
	//std::vector<float> m_resamplingBuffer;

	// Current playback position.
	// Since it's floating-point, it interpolate sub-samples.
	double m_virtualReadIndex;

	float m_playbackRate;
	size_t m_readFrames;
	size_t m_seekFrame;

	PlayingState m_playingState;
	//PlayingState m_requestedPlayingState;
	bool m_resetRequested;
    bool m_loop;
};

} // namespace detail
} // namespace ln

