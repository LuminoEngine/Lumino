
#pragma once
#include <LuminoCore/Text/Encoding.hpp>

namespace ln {
struct TableInfo;

class DBCSEncoding : public TextEncoding
{
public:
	static const int LeadBytePairsSize = 12;
	static const int TableSize = 65535;

	static const TableInfo* getTableInfo(EncodingType type);
	
public:
	DBCSEncoding(EncodingType type);
	virtual ~DBCSEncoding() {};

	// override TextEncoding
	virtual const String& name() const override;
	virtual int minByteCount() const override { return 1; }
	virtual int maxByteCount() const override { return 2; }
	virtual TextDecoder* createDecoder() override { return LN_NEW DBCSDecoder(this, m_tableInfo); }
	virtual TextEncoder* createEncoder() override { return LN_NEW DBCSEncoder(this, m_tableInfo); }
	virtual byte_t* preamble() const override { return nullptr; }
	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override;

private:
	const TableInfo* m_tableInfo;

	// TextDecoder
	class DBCSDecoder : public TextDecoder
	{
	public:
		DBCSDecoder(TextEncoding* encoding, const TableInfo* info) : TextDecoder(encoding), m_tableInfo(info) { reset(); }
		virtual bool canRemain() override { return true; }
		virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, DecodeResult* outResult) override;
		virtual int usedDefaultCharCount() override { return m_usedDefaultCharCount; }
		virtual bool completed() override { return m_lastLeadByte == 0; }
		virtual void reset() override { m_usedDefaultCharCount = 0; m_lastLeadByte = 0; }

	private:
		const TableInfo* m_tableInfo;
		byte_t m_lastLeadByte;			// 前回の convert で末尾に見つかった先行バイトを保存する
		int m_usedDefaultCharCount;		// 一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
	};

	// TextEncoder
	class DBCSEncoder : public TextEncoder
	{
	public:
		DBCSEncoder(TextEncoding* encoding, const TableInfo* info) : TextEncoder(encoding), m_tableInfo(info) { reset(); }
		virtual bool canRemain() override { return true; }
		virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, EncodeResult* outResult) override;
		virtual int usedDefaultCharCount() override { return m_usedDefaultCharCount; }
		virtual bool completed() override { return true; }
		virtual void reset() override { m_usedDefaultCharCount = 0; }

	private:
		const TableInfo* m_tableInfo;
		int m_usedDefaultCharCount;		// 一連の convertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
	};

//public:
//	// override Encoding
//	virtual const Char* getName() const { return Tables[(const int)m_encodingType].name; }
//	virtual int getMinByteCount() const { return 1; }
//	virtual int getMaxByteCount() const { return 2; }
//	virtual TextDecoder* createDecoder() const { return LN_NEW DBCSDecoder(&Tables[(const int)m_encodingType]); }
//	virtual TextEncoder* createEncoder() const { return LN_NEW DBCSEncoder(&Tables[(const int)m_encodingType]); }
//	virtual byte_t* getPreamble() const { return NULL; }
//	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const;
//	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const;
//
//private:
//	EncodingType	m_encodingType;
//
//private:
//	// Decoder
//	class DBCSDecoder : public TextDecoder
//	{
//	public:
//		DBCSDecoder(const TableInfo* tableInfo) { m_tableInfo = tableInfo;  reset(); }
//		virtual int getMinByteCount() { return 1; }
//		virtual int getMaxByteCount() { return 2; }
//		virtual bool canRemain() { return true; }
//		virtual void convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed);
//		virtual int usedDefaultCharCount() { return m_usedDefaultCharCount; }
//		virtual bool completed() { return m_lastLeadByte == 0; }
//		virtual void reset() { m_usedDefaultCharCount = 0; m_lastLeadByte = 0; }
//
//	private:
//		const TableInfo*	m_tableInfo;
//		byte_t				m_lastLeadByte;			///< 前回の convert で末尾に見つかった先行バイトを保存する
//		int					m_usedDefaultCharCount;	///< 一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
//	};
//
//	// Encoder
//	class DBCSEncoder : public TextEncoder
//	{
//	public:
//		DBCSEncoder(const TableInfo* tableInfo) { m_tableInfo = tableInfo; reset(); }
//		virtual int getMinByteCount() { return 1; }
//		virtual int getMaxByteCount() { return 2; }
//		virtual bool canRemain() { return true; }
//		virtual void convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed);
//		virtual int usedDefaultCharCount() { return m_usedDefaultCharCount; }
//		virtual bool completed() { return true; }
//		virtual void reset() { m_usedDefaultCharCount = 0; }
//
//	private:
//		const TableInfo*	m_tableInfo;
//		int					m_usedDefaultCharCount;	///< 一連の convertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
//	};
};

} // namespace ln
