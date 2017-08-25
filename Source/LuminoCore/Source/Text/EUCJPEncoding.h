
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
	virtual const Char* getName() const { return _T("EUC-JP"); }
	virtual int getMinByteCount() const { return 1; }
	virtual int getMaxByteCount() const { return 3; }
	virtual Decoder* createDecoder() const { return LN_NEW EUCJPDecoder(); }
	virtual Encoder* createEncoder() const { return LN_NEW EUCJPEncoder(); }
	virtual byte_t* getPreamble() const { return NULL; }
	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const;

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

	static inline EUCGroup checkEUCGroup(const byte_t* pos, int len);

private:
	// Decoder
	class EUCJPDecoder : public Decoder
	{
	public:
		EUCJPDecoder() { reset(); }
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return 3; }
		virtual bool canRemain() { return true; }
		virtual void convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool completed() { return mCompleted; }
		virtual void reset() { mUsedDefaultCharCount = 0; mCompleted = false; m_lastLeadBytesCount = 0; m_currentGroup = EUCGroup_Unknown; }

	private:
		int		mUsedDefaultCharCount;	///< 一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
		bool	mCompleted;				///< 最後の convertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true

		byte_t		m_lastLeadBytes[3];
		int			m_lastLeadBytesCount;
		EUCGroup	m_currentGroup;
	};

	// Encoder
	class EUCJPEncoder : public Encoder
	{
	public:
		EUCJPEncoder() { reset(); }
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return 3; }
		virtual bool canRemain() { return true; }
		virtual void convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return mUsedDefaultCharCount; }
		virtual bool completed() { return mCompleted; }
		virtual void reset() { mUsedDefaultCharCount = 0; mCompleted = false; m_lastBufferCount = 0; }

	private:
		int		mUsedDefaultCharCount;	///< 一連の convertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
		bool	mCompleted;				///< 最後の convertFromUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
		UTF16	m_lastBuffer[2];
		int		m_lastBufferCount;
	};
};

LN_NAMESPACE_END
