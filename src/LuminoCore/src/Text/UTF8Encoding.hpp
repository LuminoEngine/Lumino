
#pragma once
#include <Lumino/Text/Encoding.hpp>

namespace ln {

class UTF8Encoding : public TextEncoding
{
public:
    using UTF8 = uint8_t;

    UTF8Encoding(bool byteOrderMark);
    virtual ~UTF8Encoding(){};

    // override TextEncoding
    virtual const Char* getName() const override { return _TT("UTF-8"); }
    virtual int getMinByteCount() const override { return 1; }
    virtual int getMaxByteCount() const override { return 6; }
    virtual TextDecoder* createDecoder() const override { return LN_NEW UTF8Decoder(m_byteOrderMark); }
    virtual TextEncoder* createEncoder() const override { return LN_NEW UTF8Encoder(m_byteOrderMark); }
    virtual byte_t* getPreamble() const override;
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override;

private:
    bool m_byteOrderMark;

public:
    // TextDecoder
    class UTF8Decoder : public TextDecoder
    {
    public:
        UTF8Decoder(bool byteOrderMark)
        {
            m_byteOrderMark = byteOrderMark;
            reset();
        }
        virtual int getMinByteCount() override { return 1; }
        virtual int getMaxByteCount() override { return 6; }
        virtual bool canRemain() override { return true; }
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, DecodeResult* outResult) override;
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
        UTF8Encoder(bool byteOrderMark)
        {
            m_byteOrderMark = byteOrderMark;
            reset();
        }
        virtual int getMinByteCount() override { return 1; }
        virtual int getMaxByteCount() override { return 6; }
        virtual bool canRemain() override { return true; }
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, EncodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return mCompleted; }
        virtual void reset() override
        {
            mUsedDefaultCharCount = 0;
            mCompleted = false;
            m_lastBufferCount = 0;
        }

    private:
        int mUsedDefaultCharCount; // 一連の convertFromUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数
        bool mCompleted;           // 最後の convertFromUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true
        bool m_byteOrderMark;
        UTF16 m_lastBuffer[2];
        int m_lastBufferCount;
    };
};

} // namespace ln
