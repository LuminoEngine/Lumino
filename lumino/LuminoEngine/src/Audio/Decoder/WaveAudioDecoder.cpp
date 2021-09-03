
#include "Internal.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "WaveAudioDecoder.hpp"

namespace ln {
namespace detail {

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#define LN_CPU_X86 1
#endif

#if defined(__arm__) || defined(_M_ARM)
#define LN_CPU_ARM 1
#endif

#if !defined(LN_CPU_ARCH_BIG_ENDIAN) && !defined(LN_CPU_ARCH_LITTLE_ENDIAN)
#if LN_CPU_X86 || LN_CPU_ARM || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define LN_CPU_ARCH_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define LN_CPU_ARCH_BIG_ENDIAN 1
#else
#error LN_CPU_ARCH_LITTLE_ENDIAN or LN_CPU_ARCH_BIG_ENDIAN should be defined.
#endif
#endif

static const float NQR_BYTE_2_FLT = 1.0f / 127.0f;
#define NQR_INT16_MAX 32767.f
#define NQR_INT24_MAX 8388608.f
#define NQR_INT32_MAX 2147483648.f

#define int8_to_float32(s)  ((float) (s) * NQR_BYTE_2_FLT)
#define uint8_to_float32(s)(((float) (s) - 128) * NQR_BYTE_2_FLT)
#define int16_to_float32(s) ((float) (s) / NQR_INT16_MAX)
#define int24_to_float32(s) ((float) (s) / NQR_INT24_MAX)
#define int32_to_float32(s) ((float) (s) / NQR_INT32_MAX)

#define float32_to_int8(s)  (float) (s * 127.f)
#define float32_to_uint8(s) (float) ((s * 127.f) + 128)
#define float32_to_int16(s) (float) (s * NQR_INT16_MAX)
#define float32_to_int24(s) (float) (s * NQR_INT24_MAX)
#define float32_to_int32(s) (float) (s * NQR_INT32_MAX)

static inline uint16_t swapEndian16(uint16_t value)
{
	return (uint16_t)((value >> 8) | (value << 8));
}

static inline uint32_t swapEndian24(uint32_t value)
{
	return (((value & 0x00ff0000) >> 16) |
		((value & 0x0000ff00)) |
		((value & 0x000000ff) << 16)) & 0x00FFFFFF;
}

static inline uint32_t swapEndian32(uint32_t value)
{
	return (((value & 0x000000ff) << 24) |
		((value & 0x0000ff00) << 8) |
		((value & 0x00ff0000) >> 8) |
		((value & 0xff000000) >> 24));
}

static inline uint64_t swapEndian64(uint64_t value)
{
	return (((value & 0x00000000000000ffLL) << 56) |
		((value & 0x000000000000ff00LL) << 40) |
		((value & 0x0000000000ff0000LL) << 24) |
		((value & 0x00000000ff000000LL) << 8) |
		((value & 0x000000ff00000000LL) >> 8) |
		((value & 0x0000ff0000000000LL) >> 24) |
		((value & 0x00ff000000000000LL) >> 40) |
		((value & 0xff00000000000000LL) >> 56));
}

#ifdef LN_CPU_ARCH_LITTLE_ENDIAN
#define Read16L(n) (n)
#define Read24L(n) (n)
#define Read32L(n) (n)
#define Read64L(n) (n)
#else
#define Read16L(n) swapEndian16(n)
#define Read24L(n) swapEndian24(n)
#define Read32L(n) swapEndian32(n)
#define Read64L(n) swapEndian64(n)
#endif

void AudioDecoder::convertToFloat32(float* dst, const void* src, const size_t frameLength, PCMFormat format)
{
	if (format == PCMFormat::U8)
	{
		const uint8_t * srcPtr = reinterpret_cast<const uint8_t *>(src);
		for (size_t i = 0; i < frameLength; ++i)
			dst[i] = uint8_to_float32(srcPtr[i]);
	}
	else if (format == PCMFormat::S8)
	{
		const int8_t * srcPtr = reinterpret_cast<const int8_t *>(src);
		for (size_t i = 0; i < frameLength; ++i)
			dst[i] = int8_to_float32(srcPtr[i]);
	}
	else if (format == PCMFormat::S16L)
	{
		const int16_t * srcPtr = reinterpret_cast<const int16_t *>(src);
		for (size_t i = 0; i < frameLength; ++i)
			dst[i] = int16_to_float32(Read16L(srcPtr[i]));
	}
	else if (format == PCMFormat::S24L)
	{
		LN_NOTIMPLEMENTED();
	}
	else if (format == PCMFormat::S32L)
	{
		const int32_t * srcPtr = reinterpret_cast<const int32_t *>(src);
		for (size_t i = 0; i < frameLength; ++i)
			dst[i] = int32_to_float32(Read32L(srcPtr[i]));
	}
	else if (format == PCMFormat::S64L)
	{
		LN_NOTIMPLEMENTED();
	}
	else if (format == PCMFormat::Float)
	{
		memcpy(dst, src, frameLength * sizeof(float));
	}
	else if (format == PCMFormat::Double)
	{
		LN_NOTIMPLEMENTED();
	}
}

static float dither(float s) { return s; }	// TODO: dummy

void AudioDecoder::convertFromFloat32(void * dst, const float * src, const size_t frameLength, PCMFormat format)
{
	if (format == PCMFormat::U8)
	{
		uint8_t * dstPtr = reinterpret_cast<uint8_t *>(dst);
		for (size_t i = 0; i < frameLength; ++i)
			dstPtr[i] = (uint8_t)dither(lroundf(float32_to_uint8(src[i])));
	}
	else if (format == PCMFormat::S8)
	{
		int8_t * dstPtr = reinterpret_cast<int8_t *>(dst);
		for (size_t i = 0; i < frameLength; ++i)
			dstPtr[i] = (int8_t)dither(lroundf(float32_to_int8(src[i])));
	}
	else if (format == PCMFormat::S16L)
	{
		int16_t * dstPtr = reinterpret_cast<int16_t *>(dst);
		for (size_t i = 0; i < frameLength; ++i)
			dstPtr[i] = (int16_t)dither(lroundf(float32_to_int16(src[i])));
	}
	else if (format == PCMFormat::S24L)
	{
		LN_NOTIMPLEMENTED();
	}
	else if (format == PCMFormat::S32L)
	{
		int32_t * dstPtr = reinterpret_cast<int32_t *>(dst);
		for (size_t i = 0; i < frameLength; ++i)
			dstPtr[i] = (int32_t)dither(lroundf(float32_to_int32(src[i])));
	}
	else
	{
		LN_UNREACHABLE();
	}
}


struct WaveFileHeader
{
	uint32_t	riff;		// Chunk ID: 'RIFF'
	uint32_t	size;		// Entire file in bytes
	uint32_t	waveHeader;	// Chunk ID: 'WAVE'
};

struct FmtChunkHeader
{
	uint16_t format;            // Format code
	uint16_t channel_count;     // Num interleaved channels
	uint32_t sample_rate;       // SR
	uint32_t data_rate;         // Data rate
	uint16_t frame_size;        // 1 frame = channels * bits per sample (also known as block align)
	uint16_t bit_depth;         // Bits per sample  
};

enum WaveFormatCode
{
	WaveFormatCode_Unknown = 0x0,       // Unknown Wave Format
	WaveFormatCode_PCM = 0x1,           // PCM Format
	WaveFormatCode_IEEE = 0x3,          // IEEE float/double
};

//==============================================================================
// WaveDecoder

bool WaveDecoder::init(Stream* stream, DiagnosticsManager* diag)
{
	if (LN_REQUIRE(stream)) return false;

	m_stream = stream;
	BinaryReader reader(stream);

	//--------------------
	// read RIFF Header

	WaveFileHeader rh;
	reader.read(&rh, sizeof(WaveFileHeader));

	if (memcmp(&rh.riff, "RIFF", 4) != 0)
	{
		// detail error, little
		if (memcmp(&rh.riff, "RIFX", 4) == 0 || memcmp(&rh.riff, "FFIR", 4) != 0)
		{
			diag->reportError("big endian file is not supported.");
			return false;
		}
		else
		{
			diag->reportError("bad RIFF header.");
			return false;
		}
	}

	if (memcmp(&rh.waveHeader, "WAVE", 4) != 0)
	{
		diag->reportError("bad WAVE header.");
		return false;
	}

	//--------------------
	// read Chuncs

	char chunk[4];
	while (reader.read(chunk, 4) == 4)
	{
		if (strncmp(chunk, "fmt ", 4) == 0)
		{
			uint32_t chunkSize = reader.readUInt32();
			if (chunkSize == 16 || chunkSize == 18 || chunkSize == 20 || chunkSize == 40) {
			}
			else {
				diag->reportError("Invalid fmt chunk.");
				return false;
			}

			FmtChunkHeader fmtChunk;
			reader.read(&fmtChunk, sizeof(FmtChunkHeader));

			m_info.channelCount = fmtChunk.channel_count;
			m_info.sampleRate = fmtChunk.sample_rate;
			m_info.frameSize = fmtChunk.frame_size;
			m_bitDepth = fmtChunk.bit_depth;

			switch (m_bitDepth)
			{
				case 4: m_info.sourceFormat = PCMFormat::S16L; m_info.byteParSample = 2; break; // IMA ADPCM
				case 8: m_info.sourceFormat = PCMFormat::U8; m_info.byteParSample = 1; break;
				case 16: m_info.sourceFormat = PCMFormat::S16L; m_info.byteParSample = 2; break;
				case 24: m_info.sourceFormat = PCMFormat::S24L; m_info.byteParSample = 3; break;
				case 32: m_info.sourceFormat = (fmtChunk.format == WaveFormatCode_IEEE) ? PCMFormat::Float : PCMFormat::S32L;  m_info.byteParSample = 4; break;
				case 64: m_info.sourceFormat = (fmtChunk.format == WaveFormatCode_IEEE) ? PCMFormat::Double : PCMFormat::S64L;  m_info.byteParSample = 8; break;
			}

			if (fmtChunk.format == WaveFormatCode_PCM) {

			}
			else {
				diag->reportError("Invalid format.");
				return false;
			}
			
			// skip extention area
			if (chunkSize > 16) {
				reader.seek(chunkSize - 16);
			}
		}
		else if (strncmp(chunk, "data", 4) == 0)
		{
			uint32_t chunkSize = reader.readUInt32();
			m_pcmDataOffset = m_stream->position();
			m_pcmDataLength = chunkSize;
			m_info.totalSeconds = static_cast<double>(m_pcmDataLength) / static_cast<double>(m_info.sampleRate) / m_info.frameSize;
			m_info.totalSamples = (m_pcmDataLength / m_info.frameSize) * m_info.channelCount;
			break;
		}
		// Skip all but "fmt" and "data"
		else
		{
			uint32_t chunkSize = reader.readUInt32();
			reader.seek(chunkSize);
		}
	}

	m_workBuffer.resize(m_info.sampleRate * m_info.channelCount);
    m_pcmDataPos = 0;

    return true;
}

const AudioDataInfo& WaveDecoder::audioDataInfo() const
{
	return m_info;
}

//void WaveDecoder::seekToSample(uint32_t sampleNumber)
//{
//	m_stream->seek(m_pcmDataOffset + (sampleNumber * m_info.byteParSample), SeekOrigin::Begin);
//}

//void WaveDecoder::seekToFrame(size_t frameNumber)
//{
//    size_t frameBytes = m_info.byteParSample * m_info.channelCount;
//    size_t newPos = frameBytes * frameNumber;
//    if (LN_REQUIRE(newPos <= m_pcmDataLength)) return;
//    m_pcmDataPos = newPos;
//}

uint32_t WaveDecoder::read(size_t seekFrameNumber, float* buffer, uint32_t requestFrames)
{
	{
		size_t frameBytes = m_info.byteParSample * m_info.channelCount;
		size_t newPos = frameBytes * seekFrameNumber;
		if (LN_REQUIRE(newPos <= m_pcmDataLength)) return 0;
		m_pcmDataPos = newPos;
	}

	uint32_t requestSamples = requestFrames * m_info.channelCount;
	size_t requestByteSize = requestSamples * m_info.byteParSample;

    size_t size = std::min(requestByteSize, m_pcmDataLength - m_pcmDataPos);
    //size_t newPos = m_pcmDataPos + requestSize;
    if (size == 0) {
        return 0;
    }

    m_stream->seek(m_pcmDataOffset + m_pcmDataPos, SeekOrigin::Begin);
	size_t readSize = m_stream->read(m_workBuffer.data(), size);
    LN_CHECK(readSize == size);
    m_pcmDataPos += size;

	uint32_t readSamples = readSize / m_info.byteParSample;
	uint32_t readFrames = readSamples / m_info.channelCount;
#if 0
	memcpy(buffer, m_workBuffer.data(), readSize);
#else
	convertToFloat32(buffer, m_workBuffer.data(), std::min(readSamples, requestSamples), m_info.sourceFormat);
#endif
	return readFrames;
}

std::vector<float> WaveDecoder::readAllSamples()
{
    std::vector<byte_t> data;
    data.resize(m_pcmDataLength);

    m_stream->seek(m_pcmDataOffset, SeekOrigin::Begin);
    size_t readSize = m_stream->read(data.data(), m_pcmDataLength);


    std::vector<float> ret;
    ret.resize(m_info.totalSamples);
    convertToFloat32(ret.data(), data.data(), ret.size(), PCMFormat::S16L);
    return ret;
}

void WaveDecoder::reset()
{
}

} // namespace detail
} // namespace ln

