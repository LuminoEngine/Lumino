#pragma once
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include "AudioDecoder.hpp"

namespace ln {
namespace detail {

class OggAudioDecoder
	: public AudioDecoder
{
public:
    OggAudioDecoder();
	virtual ~OggAudioDecoder();
	void init(Stream* stream, DiagnosticsManager* diag);
	virtual const AudioDataInfo& audioDataInfo() const override;
	//virtual void seekToSample(uint32_t sampleNumber) override;
    //virtual void seekToFrame(size_t frameNumber) override;
	virtual uint32_t read(size_t seekFrameNumber, float* buffer, uint32_t requestFrames) override;
	virtual void reset() override;

private:
    static size_t readOggCallback(void* buffer, size_t element_size, size_t count, void* stream);
    static int seekOggCallback(void* stream, ogg_int64_t offset, int whence);
    static int closeOggCallback(void* stream);
    static long tellOggCallback(void* stream);

	Ref<Stream> m_stream;
    OggVorbis_File m_oggVorbisFile;
	AudioDataInfo m_info;
    size_t m_seekFrame;


	size_t m_pcmDataOffset;
	size_t m_pcmDataLength;
    //size_t m_pcmDataPos;
	size_t m_bitDepth;
	std::vector<byte_t> m_workBuffer;
};

} // namespace detail
} // namespace ln

