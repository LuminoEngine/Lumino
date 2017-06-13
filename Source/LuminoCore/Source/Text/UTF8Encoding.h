
#include "../../include/Lumino/Text/Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		UTF8 エンコーディング
*/
class UTF8Encoding : public Encoding
{
public:
	UTF8Encoding(bool byteOrderMark);
	virtual ~UTF8Encoding() {};

public:
	// override Encoding
	virtual const TCHAR* getName() const { return _T("UTF-8"); }
	virtual int getMinByteCount() const { return 1; }
	virtual int getMaxByteCount() const { return 6; }
	virtual Decoder* createDecoder() const { return LN_NEW UTF8Decoder(m_byteOrderMark); }
	virtual Encoder* createEncoder() const { return LN_NEW UTF8Encoder(m_byteOrderMark); }
	virtual byte_t* getPreamble() const;
	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const;

private:
	bool	m_byteOrderMark;

LN_INTERNAL_ACCESS:
	// Decoder
	class UTF8Decoder : public Decoder
	{
	public:
		UTF8Decoder(bool byteOrderMark) { m_byteOrderMark = byteOrderMark; reset(); }
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return 6; }
		virtual bool canRemain() { return true; }
		virtual void convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool completed() { return mCompleted; }
		virtual void reset() { mUsedDefaultCharCount = 0; mCompleted = false; m_bomPhase = 0; m_lastLeadBytesCount = 0; m_requestLeadBytesCount = 0; }

	private:
		int pollingChar(UTF8 ch, UTF16* outBuffer, size_t outBufferCharCount, size_t* outBytesUsed, size_t* outCharsUsed);

		int		mUsedDefaultCharCount;	///< 一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
		bool	mCompleted;				///< 最後の convertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
		bool	m_byteOrderMark;

		int		m_bomPhase;
		byte_t	m_lastLeadBytes[6];
		int		m_lastLeadBytesCount;
		int		m_requestLeadBytesCount;
	};

	// Encoder
	class UTF8Encoder : public Encoder
	{
	public:
		UTF8Encoder(bool byteOrderMark) { m_byteOrderMark = byteOrderMark; reset(); }
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return 6; }
		virtual bool canRemain() { return true; }
		virtual void convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool completed() { return mCompleted; }
		virtual void reset() { mUsedDefaultCharCount = 0; mCompleted = false; m_lastBufferCount = 0; }

	private:
		int		mUsedDefaultCharCount;	///< 一連の convertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
		bool	mCompleted;				///< 最後の convertFromUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
		bool	m_byteOrderMark;
		UTF16	m_lastBuffer[2];
		int		m_lastBufferCount;
	};
};

LN_NAMESPACE_END
