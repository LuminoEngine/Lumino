#pragma once
#include "AudioDecoder.hpp"

namespace ln {

namespace detail {

class WaveDecoder
	: public AudioDecoder
{
public:
	virtual ~WaveDecoder() = default;
    bool init(Stream* stream, DiagnosticsManager* diag);
	virtual const AudioDataInfo& audioDataInfo() const override;
	//virtual void seekToSample(uint32_t sampleNumber) override;
    //virtual void seekToFrame(size_t frameNumber) override;
	virtual uint32_t read(size_t seekFrameNumber, float* buffer, uint32_t requestFrames) override;
	virtual void reset() override;

    std::vector<float> readAllSamples();

private:
	Ref<Stream> m_stream;
	AudioDataInfo m_info;
	size_t m_pcmDataOffset;
	size_t m_pcmDataLength;
    size_t m_pcmDataPos;
	size_t m_bitDepth;
	size_t m_seekSample;
	std::vector<byte_t> m_workBuffer;
};

} // namespace detail
} // namespace ln

