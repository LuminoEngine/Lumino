
#pragma once
#include "../../include/Lumino/Text/Encoding.h"

LN_NAMESPACE_BEGIN

/**
	@brief	最大 2byte(DoubleByte) のマルチバイト文字コードのエンコーディング
*/
class DBCSEncoding : public Encoding
{
public:
	static const int LeadBytePairsSize = 12;
	static const int TableSize = 65535;

	struct TableInfo
	{
		const TCHAR*			name;
		const unsigned char*	leadBytePairs;		// size = LeadBytePairsSize (NULL の場合、この文字コードは DB ではない)
		const unsigned short*	dbcsToUTF16Table;	// size = TableSize
		const unsigned short*	utf16ToDBCSTable;	// size = TableSize
	};

	static const TableInfo Tables[(const int)EncodingType::TERMINATOR];
	
public:
	DBCSEncoding(EncodingType type);
	virtual ~DBCSEncoding() {};

public:
	// override Encoding
	virtual const TCHAR* getName() const { return Tables[(const int)m_encodingType].name; }
	virtual int getMinByteCount() const { return 1; }
	virtual int getMaxByteCount() const { return 2; }
	virtual Decoder* createDecoder() const { return LN_NEW DBCSDecoder(&Tables[(const int)m_encodingType]); }
	virtual Encoder* createEncoder() const { return LN_NEW DBCSEncoder(&Tables[(const int)m_encodingType]); }
	virtual byte_t* getPreamble() const { return NULL; }
	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const;

private:
	EncodingType	m_encodingType;

private:
	// Decoder
	class DBCSDecoder : public Decoder
	{
	public:
		DBCSDecoder(const TableInfo* tableInfo) { m_tableInfo = tableInfo;  reset(); }
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return 2; }
		virtual bool canRemain() { return true; }
		virtual void convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return m_usedDefaultCharCount; }
		virtual bool completed() { return m_lastLeadByte == 0; }
		virtual void reset() { m_usedDefaultCharCount = 0; m_lastLeadByte = 0; }

	private:
		const TableInfo*	m_tableInfo;
		byte_t				m_lastLeadByte;			///< 前回の convert で末尾に見つかった先行バイトを保存する
		int					m_usedDefaultCharCount;	///< 一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
	};

	// Encoder
	class DBCSEncoder : public Encoder
	{
	public:
		DBCSEncoder(const TableInfo* tableInfo) { m_tableInfo = tableInfo; reset(); }
		virtual int getMinByteCount() { return 1; }
		virtual int getMaxByteCount() { return 2; }
		virtual bool canRemain() { return true; }
		virtual void convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int usedDefaultCharCount() { return m_usedDefaultCharCount; }
		virtual bool completed() { return true; }
		virtual void reset() { m_usedDefaultCharCount = 0; }

	private:
		const TableInfo*	m_tableInfo;
		int					m_usedDefaultCharCount;	///< 一連の convertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
	};
};

LN_NAMESPACE_END
