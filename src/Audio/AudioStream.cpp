
#include "Internal.h"
#include "AudioStream.h"
#include "WaveDecoder.h"
#include "OggDecoder.h"
#include "Mp3Decoder.h"
#include "MidiDecoder.h"
#include "AudioUtils.h"

namespace Lumino
{
namespace Audio
{
	
//=============================================================================
// AudioStream
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioStream::AudioStream(Stream* stream)
	: m_stream(NULL)
	, m_decoder(NULL)
{
	LN_REFOBJ_SET(m_stream, stream);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioStream::~AudioStream()
{
	LN_SAFE_RELEASE(m_stream);
	LN_SAFE_RELEASE(m_decoder);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool AudioStream::CheckCreated()
{
	if (GetASyncIOState() == ASyncIOState_Completed) {
		return true;
	}
	else if (GetASyncIOState() == ASyncIOState_Failed && GetASyncIOException() != NULL) {
		throw *GetASyncIOException();
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioStream::OnASyncIOProc()
{
	// フォーマットを調べてデコーダを作る
	StreamFormat format = AudioUtils::CheckFormat(m_stream);
	switch (format)
	{
		// Wave
	case StreamFormat_Wave:
		m_decoder = LN_NEW WaveDecoder();
		break;
#ifdef LN_WIN32
		// MP3
	case StreamFormat_Mp3:
		m_decoder = LN_NEW Mp3Decoder();
		break;
#endif
		// OGG
	case StreamFormat_Ogg:
		m_decoder = LN_NEW OggDecoder();
		break;
		// MIDI
	case StreamFormat_Midi:
		m_decoder = LN_NEW MidiDecoder();
		break;
	default:
		LN_THROW(0, InvalidFormatException);
		break;
	}

	// 初期化
	m_decoder->Create(m_stream);
}

//=============================================================================
// AudioDecoder
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioDecoder::AudioDecoder()
	//: m_exception(NULL)
	//, m_finishedCreate(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioDecoder::~AudioDecoder()
{
	//LN_SAFE_DELETE(m_exception);
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool AudioDecoder::CheckCreated()
//{
//	if (m_exception != NULL) {
//		throw m_exception;
//	}
//	return m_finishedCreate;
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void AudioDecoder::OnCreateFinished(Exception* e)
//{
//	m_exception = e;
//	m_finishedCreate = true;
//}
//

} // namespace Audio
} // namespace Lumino
