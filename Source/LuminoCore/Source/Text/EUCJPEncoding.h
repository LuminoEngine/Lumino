
#include "../../include/Lumino/Text/Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		EUCJP エンコーディング
*/
class EUCJPEncoding : public Encoding
{
public:
	EUCJPEncoding();
	virtual ~EUCJPEncoding();

public:
	// override Encoding
	virtual const TCHAR* GetName() const { return _T("EUC-JP"); }
	virtual int GetMinByteCount() const { return 1; }
	virtual int GetMaxByteCount() const { return 3; }
	virtual Decoder* CreateDecoder() const { return LN_NEW EUCJPDecoder(); }
	virtual Encoder* CreateEncoder() const { return LN_NEW EUCJPEncoder(); }
	virtual byte_t* GetPreamble() const { return NULL; }
	virtual int GetCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int GetLeadExtraLength(const void* buffer, size_t bufferSize) const;

private:

	enum EUCGroup
	{
		EUCGroup_ASCII,
		EUCGroup_Control,
		EUCGroup_MultiByte,
		EUCGroup_Katakana2Byte,
		EUCGroup_Kanji3Byte,
		EUCGroup_Unknown,
	};

	static inline EUCGroup CheckEUCGroup(const byte_t* pos, int len);

private:
	// Decoder
	class EUCJPDecoder : public Decoder
	{
	public:
		EUCJPDecoder() { Reset(); }
		virtual int GetMinByteCount() { return 1; }
		virtual int GetMaxByteCount() { return 3; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return mCompleted; }
		virtual void Reset() { mUsedDefaultCharCount = 0; mCompleted = false; m_lastLeadBytesCount = 0; m_currentGroup = EUCGroup_Unknown; }

	private:
		int		mUsedDefaultCharCount;	///< 一連の ConvertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
		bool	mCompleted;				///< 最後の ConvertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true

		byte_t		m_lastLeadBytes[3];
		int			m_lastLeadBytesCount;
		EUCGroup	m_currentGroup;
	};

	// Encoder
	class EUCJPEncoder : public Encoder
	{
	public:
		EUCJPEncoder() { Reset(); }
		virtual int GetMinByteCount() { return 1; }
		virtual int GetMaxByteCount() { return 3; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool Completed() { return mCompleted; }
		virtual void Reset() { mUsedDefaultCharCount = 0; mCompleted = false; m_lastBufferCount = 0; }

	private:
		int		mUsedDefaultCharCount;	///< 一連の ConvertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
		bool	mCompleted;				///< 最後の ConvertFromUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
		UTF16	m_lastBuffer[2];
		int		m_lastBufferCount;
	};
};

LN_NAMESPACE_END
