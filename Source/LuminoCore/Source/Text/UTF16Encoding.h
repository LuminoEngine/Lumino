
#include "../../include/Lumino/Text/Encoding.h"

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
	virtual const TCHAR* GetName() const { return (m_bigEndian) ? _T("UTF-16BE") : _T("UTF-16"); }	// .NET に合わせてみる
	virtual int GetMinByteCount() const { return 2; }
	virtual int GetMaxByteCount() const { return 4; }
	virtual Decoder* CreateDecoder() const { return LN_NEW UTF16Decoder(); }
	virtual Encoder* CreateEncoder() const { return LN_NEW UTF16Encoder(); }
	virtual byte_t* GetPreamble() const;
	virtual int GetCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int GetLeadExtraLength(const void* buffer, size_t bufferSize) const;

private:
	bool m_bigEndian;
	bool m_byteOrderMark;

private:
	// Decoder
	class UTF16Decoder : public Decoder
	{
	public:
		UTF16Decoder() { Reset(); }
		virtual int GetMinByteCount() { return 2; }
		virtual int GetMaxByteCount() { return 4; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return m_lastLeadWord == 0x0000 && m_lastLeadWord == 0x0000; }
		virtual void Reset() { mUsedDefaultCharCount = 0; m_lastLeadByte = 0x00; m_lastLeadWord = 0x0000; }

	private:
		int			mUsedDefaultCharCount;
		byte_t		m_lastLeadByte;
		uint16_t	m_lastLeadWord;
	};

	// Encoder
	class UTF16Encoder : public Encoder
	{
	public:
		UTF16Encoder() { Reset(); }
		virtual int GetMinByteCount() { return 2; }
		virtual int GetMaxByteCount() { return 4; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return 0; }
		virtual bool Completed() { return true; }
		virtual void Reset() { }
	};
};

LN_NAMESPACE_END
