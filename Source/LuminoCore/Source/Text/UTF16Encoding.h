
#include "../../Include/Lumino/Text/Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		UTF16 エンコーディング
*/
class UTF16Encoding : public Encoding
{
public:
	UTF16Encoding(bool bigEndian, bool byteOrderMark);
	virtual ~UTF16Encoding() {};

public:
	// override Encoding
	virtual const Char* getName() const { return (m_bigEndian) ? _TT("UTF-16BE") : _TT("UTF-16"); }	// .NET に合わせてみる
	virtual int getMinByteCount() const { return 2; }
	virtual int getMaxByteCount() const { return 4; }
	virtual Decoder* createDecoder() const { return LN_NEW UTF16Decoder(); }
	virtual Encoder* createEncoder() const { return LN_NEW UTF16Encoder(); }
	virtual byte_t* getPreamble() const;
	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const;

private:
	bool m_bigEndian;
	bool m_byteOrderMark;

private:
	// Decoder
	class UTF16Decoder : public Decoder
	{
	public:
		UTF16Decoder() { reset(); }
		virtual int getMinByteCount() { return 2; }
		virtual int getMaxByteCount() { return 4; }
		virtual bool canRemain() { return true; }
		virtual void convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool completed() { return m_lastLeadWord == 0x0000 && m_lastLeadWord == 0x0000; }
		virtual void reset() { mUsedDefaultCharCount = 0; m_lastLeadByte = 0x00; m_lastLeadWord = 0x0000; }

	private:
		int			mUsedDefaultCharCount;
		byte_t		m_lastLeadByte;
		uint16_t	m_lastLeadWord;
	};

	// Encoder
	class UTF16Encoder : public Encoder
	{
	public:
		UTF16Encoder() { reset(); }
		virtual int getMinByteCount() { return 2; }
		virtual int getMaxByteCount() { return 4; }
		virtual bool canRemain() { return true; }
		virtual void convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return 0; }
		virtual bool completed() { return true; }
		virtual void reset() { }
	};
};

LN_NAMESPACE_END
