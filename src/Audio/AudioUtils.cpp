
#include "AudioStream.h"
#include "AudioUtils.h"

namespace Lumino
{
namespace Audio
{

//=============================================================================
// AudioUtils
//=============================================================================

#ifdef _WIN32

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioUtils::ConvertLNWaveFormatToWAVEFORMATEX(const WaveFormat& lnFmt, WAVEFORMATEX* wavFmt)
{
    wavFmt->wFormatTag       = lnFmt.FormatTag;
    wavFmt->nChannels        = lnFmt.Channels;
    wavFmt->nSamplesPerSec   = lnFmt.SamplesPerSec;
    wavFmt->nAvgBytesPerSec  = lnFmt.AvgBytesPerSec;
    wavFmt->nBlockAlign      = lnFmt.BlockAlign;
    wavFmt->wBitsPerSample   = lnFmt.BitsPerSample;
    wavFmt->cbSize           = lnFmt.EXSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioUtils::ConvertWAVEFORMATEXToLNWaveFormat(const WAVEFORMATEX& wavFmt, WaveFormat* lnFmt)
{
    lnFmt->FormatTag      = wavFmt.wFormatTag;
    lnFmt->Channels       = wavFmt.nChannels;
    lnFmt->SamplesPerSec  = wavFmt.nSamplesPerSec;
    lnFmt->AvgBytesPerSec = wavFmt.nAvgBytesPerSec;
    lnFmt->BlockAlign     = wavFmt.nBlockAlign;
    lnFmt->BitsPerSample  = wavFmt.wBitsPerSample;
    lnFmt->EXSize         = wavFmt.cbSize;
}   

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioUtils::PrintWAVEFORMATEX(const WAVEFORMATEX& wavFmt, const char* str)
{
    if ( str )
	{
		printf( "%s\n", str );
	}
	else
	{
        printf( "AudioUtils::dumpWAVEFORMATEX\n" );
	}
	printf( "    wFormatTag                %hu\n", wavFmt.wFormatTag );
	printf( "    nChannels                 %hu\n", wavFmt.nChannels );
	printf( "    nSamplesPerSec            %lu\n", wavFmt.nSamplesPerSec );
	printf( "    nAvgBytesPerSec(Byte/sec) %lu\n", wavFmt.nAvgBytesPerSec );
	printf( "    nBlockAlign               %hu\n", wavFmt.nBlockAlign );
	printf( "    wBitsPerSample            %hu\n", wavFmt.wBitsPerSample );
	printf( "    cbSize                    %hu\n", wavFmt.cbSize );
}

#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
StreamFormat AudioUtils::CheckFormat(Stream* stream)
{
	if (!stream || stream->GetLength() < 4) {
		return StreamFormat_Unknown;
	}

	StreamFormat format = StreamFormat_Unknown;

	byte_t head[4];
	stream->Read(head, 4);
	if (memcmp(head, "RIFF", 4) == 0)
	{
		stream->Seek(4, SeekOrigin_Current);
		byte_t wave[4];
		stream->Read(wave, 4);
		if (memcmp(wave, "WAVE", 4) == 0)
		{
			format = StreamFormat_Wave;
		}
	}
	else if (memcmp(head, "OggS", 4) == 0)
	{
		format = StreamFormat_Ogg;
	}
	else if (memcmp(head, "MThd", 4) == 0)
	{
		format = StreamFormat_Midi;
	}
	else if (memcmp(head, "ID3", 3) == 0)
	{
		format = StreamFormat_Mp3;
	}

	if (format == StreamFormat_Unknown)
	{
		// mp3 の場合はヘッダが ID3 だったり後ろの方に TAG があったり
		// 同期ヘッダだけあったりするので全部チェックする
		if (head[0] == 0xff && head[1] >> 5 == 0x07)
		{
			format = StreamFormat_Mp3;
		}
		else
		{
			stream->Seek(-128, SeekOrigin_End);
			char data[3];
			stream->Read(data, 3);
			if (data[0] == 'T' && data[1] == 'A' && data[2] == 'G')
			{
				format = StreamFormat_Mp3;
			}
		}
	}

	stream->Seek(0, SeekOrigin_Begin);
	return format;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SoundPlayType AudioUtils::CheckAudioPlayType(SoundPlayType type, AudioStream* audioStream, uint32_t limitSize)
{
	// 作成するオーディオプレイヤーの種類を決めていく
	if (audioStream->GetDecoder()->GetSourceFormat() == StreamFormat_Midi)
	{
		type = SoundPlayType_Midi;
	}
	else
	{
		if (type == SoundPlayType_Unknown)
		{
			if (audioStream->GetDecoder()->GetOnmemoryPCMBufferSize() > limitSize)
			{
				type = SoundPlayType_Streaming;
			}
			else
			{
				type = SoundPlayType_OnMemory;
			}
		}
	}

    return type;
}

} // namespace Audio
} // namespace Lumino
