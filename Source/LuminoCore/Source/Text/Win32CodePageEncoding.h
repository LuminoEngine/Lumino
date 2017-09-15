
#include "../../include/Lumino/Base/String.h"
#include "../../include/Lumino/Text/Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		Win32 コードページ エンコーディング
*/
class Win32CodePageEncoding : public Encoding
{
public:
	Win32CodePageEncoding(UINT codePage);
	virtual ~Win32CodePageEncoding() {};

public:
	// override Encoding
	virtual const TTCHAR* getName() const { return m_name.c_str(); }
	virtual int getMinByteCount() const { return 1; }
	virtual int getMaxByteCount() const { return m_cpInfo.MaxCharSize; }
	virtual Decoder* createDecoder() const { return LN_NEW Win32CodePageDecoder(m_cpInfo); }
	virtual Encoder* createEncoder() const { return LN_NEW Win32CodePageEncoder(m_cpInfo); }
	virtual byte_t* getPreamble() const { return NULL; }
	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const;

private:
	CPINFOEX	m_cpInfo;
	String		m_name;

LN_INTERNAL_ACCESS:
	// Decoder
	class Win32CodePageDecoder : public Decoder
	{
	public:
		Win32CodePageDecoder(const CPINFOEX& cpInfo);
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return m_maxByteCount; }
		virtual bool canRemain() { return m_canRemain; }
		virtual void convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return m_usedDefaultCharCount; }
		virtual bool completed() { return m_lastLeadByte == 0; }
		virtual void reset() { m_usedDefaultCharCount = 0; m_lastLeadByte = 0x0000; }

	private:
		int		m_codePage;			///< コードページ
		int		m_maxByteCount;		///< 1文字の最大バイト数
		byte_t	m_lastLeadByte;		///< 前回の convert で末尾に見つかった先行バイトを保存する
		int		m_usedDefaultCharCount;
		bool	m_canRemain;
	};

	// Encoder
	class Win32CodePageEncoder : public Encoder
	{
	public:
		Win32CodePageEncoder(const CPINFOEX& cpInfo);
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return m_maxByteCount; }
		virtual bool canRemain() { return m_canRemain; }
		virtual void convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return m_usedDefaultCharCount; }
		virtual bool completed() { return true; }
		virtual void reset() { m_usedDefaultCharCount = 0; }

	private:
		int		m_codePage;			///< コードページ
		int		m_maxByteCount;		///< 1文字の最大バイト数
		int		m_usedDefaultCharCount;
		bool	m_canRemain;
	};
};

LN_NAMESPACE_END
