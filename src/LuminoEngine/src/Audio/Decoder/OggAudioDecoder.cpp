
#include "Internal.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "OggAudioDecoder.hpp"

namespace ln {
namespace detail {

//==============================================================================
// OggAudioDecoder

OggAudioDecoder::OggAudioDecoder()
{
}

OggAudioDecoder::~OggAudioDecoder()
{
	ov_clear(&m_oggVorbisFile);
}

void OggAudioDecoder::init(Stream* stream, DiagnosticsManager* diag)
{
	if (LN_REQUIRE(stream)) return;

	m_stream = stream;
	BinaryReader reader(stream);

    ov_callbacks callbacks = {
        readOggCallback,
        seekOggCallback,
        closeOggCallback,
        tellOggCallback };

    int err = ov_open_callbacks(m_stream, &m_oggVorbisFile, 0, 0, callbacks);
    if (err != 0)
    {
        const char* es = "";
        switch (err)
        {
            case OV_EREAD:		es = "OV_EREAD"; break;
            case OV_ENOTVORBIS:	es = "OV_ENOTVORBIS"; break;
            case OV_EVERSION:	es = "OV_EVERSION"; break;
            case OV_EBADHEADER:	es = "OV_EBADHEADER"; break;
            case OV_EFAULT:		es = "OV_EFAULT"; break;
        }
        LN_ERROR(es);
        return;
    }

    //--------------------
    // read wave info
    vorbis_info* ogg_info = ov_info(&m_oggVorbisFile, -1);
    //int bits_per_sample = WORD_BITS;	// とりあえず 16bit 固定
    //m_waveFormat.formatTag = 1;	// PCM
    m_info.channelCount = ogg_info->channels;
    m_info.byteParSample = 2;
    m_info.sourceFormat = PCMFormat::S16L;
    m_info.sampleRate = ogg_info->rate;
    m_info.frameSize = m_info.channelCount * m_info.byteParSample;

    ogg_int64_t totalFrames = ov_pcm_total(&m_oggVorbisFile, -1);
    m_info.totalSamples = totalFrames * m_info.channelCount;
    m_info.totalSeconds = static_cast<double>(totalFrames) / static_cast<double>(m_info.sampleRate);


    //static_cast<uint32_t>(totalFrames)* WORD_SIZE * m_waveFormat.channels




    //m_waveFormat.samplesPerSec = ogg_info->rate;
    //m_waveFormat.avgBytesPerSec = ogg_info->rate * bits_per_sample / 8 * ogg_info->channels;
    //m_waveFormat.blockAlign = bits_per_sample / 8 * ogg_info->channels;
    //m_waveFormat.bitsPerSample = bits_per_sample;
    //m_waveFormat.exSize = 0;


    //int channelCount;
    //int sampleRate;
    //double totalSeconds;
    //size_t frameSize; // samples の、1フレームの要素数 (channelCount * byteParSample)
    //size_t totalSamples;	// 全サンプル数。ステレオの場合、複数チャンネル込み。
    ////std::vector<float> samples;
    //PCMFormat sourceFormat;
    //size_t byteParSample;



   // m_info.channelCount = fmtChunk.channel_count;
   // m_info.sampleRate = fmtChunk.sample_rate;
   // m_info.frameSize = fmtChunk.frame_size;
   // 
			//m_info.frameSize = fmtChunk.frame_size;



	m_workBuffer.resize(m_info.sampleRate * m_info.channelCount);
    m_seekFrame = 0;
    ov_time_seek(&m_oggVorbisFile, 0.0);
}

const AudioDataInfo& OggAudioDecoder::audioDataInfo() const
{
	return m_info;
}

//void OggAudioDecoder::seekToSample(uint32_t sampleNumber)
//{
//	m_stream->seek(m_pcmDataOffset + (sampleNumber * m_info.byteParSample), SeekOrigin::Begin);
//}

//void OggAudioDecoder::seekToFrame(size_t frameNumber)
//{
//    m_seekFrame = frameNumber;
//    //size_t frameBytes = m_info.byteParSample * m_info.channelCount;
//    //size_t newPos = frameBytes * frameNumber;
//    //if (LN_REQUIRE(newPos <= m_pcmDataLength)) return;
//    //m_pcmDataPos = newPos;
//}

uint32_t OggAudioDecoder::read(size_t seekFrameNumber, float* buffer, uint32_t requestFrames)
{
	m_seekFrame = seekFrameNumber;



	uint32_t requestSamples = requestFrames * m_info.channelCount;
	size_t requestByteSize = requestSamples * m_info.byteParSample;

    //size_t size = std::min(requestByteSize, m_pcmDataLength - m_pcmDataPos);
    ////size_t newPos = m_pcmDataPos + requestSize;
    //if (size == 0) {
    //    return 0;
    //}

    int r = ov_pcm_seek(&m_oggVorbisFile, m_seekFrame); // in pcm par frame (※API Reference は par sample とか言い方が少し違うので注意)
    LN_ENSURE(r == 0);

    size_t readByteSize = 0;
    int bitstream = 0;
    while (1)
    {
        long r = ov_read(&m_oggVorbisFile, (char*)(m_workBuffer.data()) + readByteSize, requestByteSize, 0, 2, 1, &bitstream);
        if (LN_ENSURE(r >= 0)) return 0;
        if (r == 0) {    // EOF
            break;
        }

        // http://sai2s.seesaa.net/article/309261827.html
        if (bitstream != 0) {
            LN_NOTIMPLEMENTED();
            return 0;
        }

        readByteSize += r;
        requestByteSize -= r;

        if (requestByteSize <= 0) {
            // buffer filled.
            break;
        }
    }


    //int bitstream = 0;  // http://sai2s.seesaa.net/article/309261827.html
    //long readSize = ov_read(&m_oggVorbisFile, (char*)(m_workBuffer.data()), size, 0, 2, 1, &bitstream);
    //LN_ENSURE(readSize >= 0);
    ////LN_CHECK(readSize == size);

    //m_pcmDataPos += size;

	uint32_t readSamples = readByteSize / m_info.byteParSample;
	uint32_t readFrames = readSamples / m_info.channelCount;
#if 0
	memcpy(buffer, m_workBuffer.data(), readSize);
#else
	convertToFloat32(buffer, m_workBuffer.data(), std::min(readSamples, requestSamples), m_info.sourceFormat);
#endif

    m_seekFrame += readFrames;
	return readFrames;
}

void OggAudioDecoder::reset()
{
}

size_t OggAudioDecoder::readOggCallback(void* buffer, size_t element_size, size_t count, void* stream)
{
    if (!stream) return -1;
    Stream* file = reinterpret_cast<Stream*>(stream);
    return file->read(buffer, element_size * count);
}

int OggAudioDecoder::seekOggCallback(void* stream, ogg_int64_t offset, int whence)
{
    if (!stream) return -1;
    Stream* file = reinterpret_cast<Stream*>(stream);
    file->seek(static_cast<int>(offset), (SeekOrigin)whence);
    return 0;
}

int OggAudioDecoder::closeOggCallback(void* stream)
{
    return 0;	// InFile のデストラクタで閉じるので、ここでは何もしない。
}

long OggAudioDecoder::tellOggCallback(void* stream)
{
    if (!stream) return -1;
    Stream* file = reinterpret_cast<Stream*>(stream);
    return static_cast<int>(file->position());
}

} // namespace detail
} // namespace ln

