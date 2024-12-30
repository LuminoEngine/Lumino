#pragma once

namespace ln {

//namespace blink {
//	class SincResampler;
//}

//enum class Channel : int
//{
//	First = 0,
//	Left = 0,
//	Right = 1,
//	Center = 2, // center and mono are the same
//	Mono = 2,
//	LFE = 3,
//	SurroundLeft = 4,
//	SurroundRight = 5,
//};

namespace detail {

// The buffer used for data stream between a audio nodes.
// Data is floating point, and range is -1.0 ~ +1.0
class ARIChannel
	: public RefObject
{
public:
	ARIChannel(size_t length);
	virtual ~ARIChannel() = default;

	float* mutableData() { clearSilentFlag();  return m_data.data(); }	// Direct access to PCM sample data. clears silent flag.
	const float* constData() const { return m_data.data(); }
	size_t length() const { return m_data.size(); }

	void setSilentAndZero();
	void clearSilentFlag() { m_isSilent = false; }
	bool isSilent() const { return m_isSilent; }

	//void clear();
	void copyTo(float* buffer, size_t bufferLength, size_t stride) const;
	void copyFrom(const float* buffer, size_t bufferLength, size_t stride);
	void copyFrom(const ARIChannel* ch);
	void sumFrom(const ARIChannel* ch);
    void fillZero(size_t start, size_t length); // isSilent は変化しない

	// chromium interface
	float* MutableData() { return mutableData(); }
	const float* Data() const { return constData(); }


private:
	std::vector<float> m_data;
	bool m_isSilent;
};

// collection of a audio channels.
class ARIAudioBus
	: public RefObject
{
public:
	enum
	{
		kLayoutCanonical = 0
	};

	enum ChannelInterpretation {
		kSpeakers,
		kDiscrete,
	};


	enum {
		kChannelLeft = 0,
		kChannelRight = 1,
		kChannelCenter = 2,  // center and mono are the same
		kChannelMono = 2,
		kChannelLFE = 3,
		kChannelSurroundLeft = 4,
		kChannelSurroundRight = 5,
	};

	ARIAudioBus();
	virtual ~ARIAudioBus() = default;
	void initialize2(int channelCount, size_t length, int sampleRate = 0);

	size_t length() const { return m_validLength; }	// フレーム数
	//void setValidLength(size_t length) { m_validLength = length; }
	//size_t fullLength() const { return m_channels[0]->length(); }
	int sampleRate() const { return m_sampleRate; }
	void setSampleRate(int value) { m_sampleRate = value; }

	int channelCount() const { return m_channels.size(); }
	int numberOfChannels() const { return m_channels.size(); }

	ARIChannel* channel(int index) const { return m_channels[index]; }
	ARIChannel* channelByType(unsigned  type);
	const ARIChannel* channelByType(unsigned  type) const;

	void setSilentAndZero();	// set silent flag, and zero clear buffers if needed. if set a valid samples in process(), please call clearSilentFlag()
	void clearSilentFlag();
	bool isSilent() const;	// return true if all child true.

    void fillZero(size_t start, size_t length);
	void mergeToChannelBuffers(float* buffer, size_t length);
	void separateFrom(const float* buffer, size_t length, int channelCount);
	void sumFrom(const ARIAudioBus* sourceBus);

    void copyFrom(ARIAudioBus* source);
	void copyWithGainFrom(const ARIAudioBus& source_bus, float gain);
	void copyBySampleRateConverting(const ARIAudioBus* source_bus, int new_sample_rate);
	bool topologyMatches(const ARIAudioBus& bus) const;

	// chromium interface
	int NumberOfChannels() const { return m_channels.size(); }
	ARIChannel* Channel(int index) const { return channel(index); }
	ARIChannel* ChannelByType(unsigned  type) { return channelByType(type); }


private:
    void sumFromByUpMixing(const ARIAudioBus* sourceBus);
    void sumFromByDownMixing(const ARIAudioBus* sourceBus);
    void discreteSumFrom(const ARIAudioBus* sourceBus);

	List<Ref<ARIChannel>> m_channels;
	size_t m_validLength;
	int m_sampleRate;

	int m_layout = kLayoutCanonical;
};

} // namespace detail
} // namespace ln

