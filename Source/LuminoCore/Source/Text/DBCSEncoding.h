
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
	virtual const TCHAR* GetName() const { return Tables[(const int)m_encodingType].name; }
	virtual int GetMinByteCount() const { return 1; }
	virtual int GetMaxByteCount() const { return 2; }
	virtual Decoder* CreateDecoder() const { return LN_NEW DBCSDecoder(&Tables[(const int)m_encodingType]); }
	virtual Encoder* CreateEncoder() const { return LN_NEW DBCSEncoder(&Tables[(const int)m_encodingType]); }
	virtual byte_t* GetPreamble() const { return NULL; }
	virtual int GetCharacterCount(const void* buffer, size_t bufferSize) const;
	virtual int GetLeadExtraLength(const void* buffer, size_t bufferSize) const;

private:
	EncodingType	m_encodingType;

private:
	// Decoder
	class DBCSDecoder : public Decoder
	{
	public:
		DBCSDecoder(const TableInfo* tableInfo) { m_tableInfo = tableInfo;  Reset(); }
		virtual int GetMinByteCount() { return 1; }
		virtual int GetMaxByteCount() { return 2; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return m_usedDefaultCharCount; }
		virtual bool Completed() { return m_lastLeadByte == 0; }
		virtual void Reset() { m_usedDefaultCharCount = 0; m_lastLeadByte = 0; }

	private:
		const TableInfo*	m_tableInfo;
		byte_t				m_lastLeadByte;			///< 前回の Convert で末尾に見つかった先行バイトを保存する
		int					m_usedDefaultCharCount;	///< 一連の ConvertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
	};

	// Encoder
	class DBCSEncoder : public Encoder
	{
	public:
		DBCSEncoder(const TableInfo* tableInfo) { m_tableInfo = tableInfo; Reset(); }
		virtual int GetMinByteCount() { return 1; }
		virtual int GetMaxByteCount() { return 2; }
		virtual bool CanRemain() { return true; }
		virtual void ConvertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
		virtual int UsedDefaultCharCount() { return m_usedDefaultCharCount; }
		virtual bool Completed() { return true; }
		virtual void Reset() { m_usedDefaultCharCount = 0; }

	private:
		const TableInfo*	m_tableInfo;
		int					m_usedDefaultCharCount;	///< 一連の ConvertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
	};
};

LN_NAMESPACE_END
