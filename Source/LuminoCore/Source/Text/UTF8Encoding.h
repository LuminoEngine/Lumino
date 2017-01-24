
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
	virtual const TCHAR* GetName() const { return _T("UTF-8"); }
	virtual int GetMinByteCount() const { return 1; }
	virtual int GetMaxByteCount() const { return 6; }
	virtual Decoder* CreateDecoder() const { return LN_NEW UTF8Decoder(m_byteOrderMark); }
	virtual Encoder* CreateEncoder() const { return LN_NEW UTF8Encoder(m_byteOrderMark); }
	virtual byte_t* GetPreamble() const;
	virtual int GetCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int GetLeadExtraLength(const void* buffer, size_t bufferSize) const;

private:
	bool	m_byteOrderMark;

LN_INTERNAL_ACCESS:
	// Decoder
	class UTF8Decoder : public Decoder
	{
	public:
		UTF8Decoder(bool byteOrderMark) { m_byteOrderMark = byteOrderMark; Reset(); }
		virtual int GetMinByteCount() { return 1; }
		virtual int GetMaxByteCount() { return 6; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return mCompleted; }
		virtual void Reset() { mUsedDefaultCharCount = 0; mCompleted = false; m_bomPhase = 0; m_lastLeadBytesCount = 0; m_requestLeadBytesCount = 0; }

	private:
		int PollingChar(UTF8 ch, UTF16* outBuffer, size_t outBufferCharCount, size_t* outBytesUsed, size_t* outCharsUsed);

		int		mUsedDefaultCharCount;	///< 一連の ConvertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
		bool	mCompleted;				///< 最後の ConvertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
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
		UTF8Encoder(bool byteOrderMark) { m_byteOrderMark = byteOrderMark; Reset(); }
		virtual int GetMinByteCount() { return 1; }
		virtual int GetMaxByteCount() { return 6; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return mCompleted; }
		virtual void Reset() { mUsedDefaultCharCount = 0; mCompleted = false; m_lastBufferCount = 0; }

	private:
		int		mUsedDefaultCharCount;	///< 一連の ConvertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
		bool	mCompleted;				///< 最後の ConvertFromUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
		bool	m_byteOrderMark;
		UTF16	m_lastBuffer[2];
		int		m_lastBufferCount;
	};
};

LN_NAMESPACE_END
