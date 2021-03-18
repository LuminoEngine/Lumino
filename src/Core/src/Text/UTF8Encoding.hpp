
#pragma once
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/Base/String.hpp>

namespace ln {

class UTF8Encoding : public TextEncoding
{
public:
    using UTF8 = uint8_t;

	static const byte_t BOM[3];
	static const String Name;

    UTF8Encoding(bool byteOrderMark);
    virtual ~UTF8Encoding(){};

    // override TextEncoding
    virtual const String& name() const override { return Name; }
    virtual int minByteCount() const override { return 1; }
    virtual int maxByteCount() const override { return 6; }
    virtual TextDecoder* createDecoder() override { return LN_NEW UTF8Decoder(this, m_byteOrderMark); }
    virtual TextEncoder* createEncoder() override { return LN_NEW UTF8Encoder(this, m_byteOrderMark); }
    virtual byte_t* preamble() const override;
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override;
	virtual bool convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;

private:
    bool m_byteOrderMark;

public:
    // TextDecoder
    class UTF8Decoder : public TextDecoder
    {
    public:
        UTF8Decoder(TextEncoding* encoding, bool byteOrderMark)
			: TextDecoder(encoding)
        {
            m_byteOrderMark = byteOrderMark;
            reset();
        }
        virtual bool canRemain() override { return true; }
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return mCompleted; }
        virtual void reset() override
        {
            mUsedDefaultCharCount = 0;
            mCompleted = false;
            m_bomPhase = 0;
            m_lastLeadBytesCount = 0;
            m_requestLeadBytesCount = 0;
        }

    private:
        int mUsedDefaultCharCount; // 一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
        bool mCompleted;           // 最後の convertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
        bool m_byteOrderMark;

        int m_bomPhase;
        byte_t m_lastLeadBytes[6];
        int m_lastLeadBytesCount;
        int m_requestLeadBytesCount;
    };

    // TextEncoder
    class UTF8Encoder : public TextEncoder
    {
    public:
        UTF8Encoder(TextEncoding* encoding, bool byteOrderMark)
			: TextEncoder(encoding)
        {
            m_byteOrderMark = byteOrderMark;
            reset();
        }
        virtual bool canRemain() override { return true; }
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return mCompleted; }
        virtual void reset() override
        {
            mUsedDefaultCharCount = 0;
            mCompleted = false;
            m_bomWrited = false;
            m_lastBufferCount = 0;
        }

    private:
        int mUsedDefaultCharCount; // 一連の convertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
        bool mCompleted;           // 最後の convertFromUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
        bool m_byteOrderMark;
        bool m_bomWrited;
        UTF16 m_lastBuffer[2];
        int m_lastBufferCount;
    };
};

} // namespace ln
