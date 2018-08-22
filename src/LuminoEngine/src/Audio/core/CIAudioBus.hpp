#pragma once

namespace ln {
namespace detail {

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

// The buffer used for data stream between a audio nodes.
// Data is floating point, and range is -1.0 ~ +1.0
class CIAudioChannel
	: public RefObject
{
public:
	CIAudioChannel(size_t length);
	virtual ~CIAudioChannel() = default;

	float* mutableData() { clearSilentFlag();  return m_data.data(); }	// Direct access to PCM sample data. clears silent flag.
	const float* constData() const { return m_data.data(); }
	size_t length() const { return m_data.size(); }

	void setSilentAndZero();
	void clearSilentFlag() { m_isSilent = false; }
	bool isSilent() const { return m_isSilent; }

	//void clear();
	void copyTo(float* buffer, size_t bufferLength, size_t stride) const;
	void copyFrom(const float* buffer, size_t bufferLength, size_t stride);
	void copyFrom(const CIAudioChannel* ch);
	void sumFrom(const CIAudioChannel* ch);


	// chromium interface
	float* MutableData() { return mutableData(); }
	const float* Data() const { return constData(); }


private:
	std::vector<float> m_data;
	bool m_isSilent;
};

// collection of a audio channels.
class CIAudioBus
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

	CIAudioBus();
	virtual ~CIAudioBus() = default;
	void initialize2(int channelCount, size_t length, int sampleRate = 0);

	size_t length() const { return m_validLength; }	// フレーム数
	//void setValidLength(size_t length) { m_validLength = length; }
	//size_t fullLength() const { return m_channels[0]->length(); }
	int sampleRate() const { return m_sampleRate; }
	void setSampleRate(int value) { m_sampleRate = value; }

	int channelCount() const { return m_channels.size(); }
	int numberOfChannels() const { return m_channels.size(); }

	CIAudioChannel* channel(int index) const { return m_channels[index]; }
	CIAudioChannel* channelByType(unsigned  type);
	const CIAudioChannel* channelByType(unsigned  type) const;

	void setSilentAndZero();	// set silent flag, and zero clear buffers if needed. if set a valid samples in process(), please call clearSilentFlag()
	void clearSilentFlag();
	bool isSilent() const;	// return true if all child true.


	void mergeToChannelBuffers(float* buffer, size_t length);
	void separateFrom(const float* buffer, size_t length, int channelCount);
	void sumFrom(const CIAudioBus* bus);

	void copyWithGainFrom(const CIAudioBus& source_bus, float gain);
	void copyBySampleRateConverting(const CIAudioBus* source_bus, int new_sample_rate);
	bool topologyMatches(const CIAudioBus& bus) const;

	// chromium interface
	int NumberOfChannels() const { return m_channels.size(); }
	CIAudioChannel* Channel(int index) const { return channel(index); }
	CIAudioChannel* ChannelByType(unsigned  type) { return channelByType(type); }


private:
	List<Ref<CIAudioChannel>> m_channels;
	size_t m_validLength;
	int m_sampleRate;

	int m_layout = kLayoutCanonical;
};

} // namespace detail
} // namespace ln

