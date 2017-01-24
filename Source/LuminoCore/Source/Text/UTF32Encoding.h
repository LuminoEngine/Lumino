
#include "../../include/Lumino/Text/Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		UTF32 エンコーディング
*/
class UTF32Encoding : public Encoding
{
public:
	UTF32Encoding(bool bigEndian, bool byteOrderMark);
	virtual ~UTF32Encoding() {};

public:
	// override Encoding
	virtual const TCHAR* GetName() const { return (m_bigEndian) ? _T("UTF-32BE") : _T("UTF-32LE"); }
	virtual int GetMinByteCount() const { return 4; }
	virtual int GetMaxByteCount() const { return 4; }
	virtual Decoder* CreateDecoder() const { return LN_NEW UTF32Decoder(); }
	virtual Encoder* CreateEncoder() const { return LN_NEW UTF32Encoder(); }
	virtual byte_t* GetPreamble() const;
	virtual int GetCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int GetLeadExtraLength(const void* buffer, size_t bufferSize) const { return 0; }

private:
	bool m_bigEndian;
	bool m_byteOrderMark;

private:
	// Decoder
	class UTF32Decoder : public Decoder
	{
	public:
		UTF32Decoder() { Reset(); }
		virtual int GetMinByteCount() { return 4; }
		virtual int GetMaxByteCount() { return 4; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return m_lastLeadBytesCount == 0; }
		virtual void Reset() { mUsedDefaultCharCount = 0; m_lastLeadBytesCount = false; }

	private:
		int		mUsedDefaultCharCount;
		byte_t	m_lastLeadBytes[4];
		int		m_lastLeadBytesCount;
	};

	// Encoder
	class UTF32Encoder : public Encoder
	{
	public:
		UTF32Encoder() { Reset(); }
		virtual int GetMinByteCount() { return 4; }
		virtual int GetMaxByteCount() { return 4; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return m_lastLeadWord == 0x0000; }
		virtual void Reset() { mUsedDefaultCharCount = 0; m_lastLeadWord = 0x0000; }

	private:
		int			mUsedDefaultCharCount;
		uint16_t	m_lastLeadWord;
	};
};

LN_NAMESPACE_END
