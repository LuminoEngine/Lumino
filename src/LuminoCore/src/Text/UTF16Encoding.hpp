
#pragma once
#include <Lumino/Text/Encoding.hpp>

namespace ln {

class UTF16Encoding : public TextEncoding
{
public:
    UTF16Encoding(bool bigEndian, bool byteOrderMark);
    virtual ~UTF16Encoding(){};

public:
    // override TextEncoding
    virtual const Char* getName() const override { return (m_bigEndian) ? _TT("UTF-16BE") : _TT("UTF-16"); } // .NET に合わせてみる
    virtual int getMinByteCount() const override { return 2; }
    virtual int getMaxByteCount() const override { return 4; }
    virtual TextDecoder* createDecoder() const override { return LN_NEW UTF16Decoder(); }
    virtual TextEncoder* createEncoder() const override { return LN_NEW UTF16Encoder(); }
    virtual byte_t* getPreamble() const override;
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override;

private:
    bool m_bigEndian;
    bool m_byteOrderMark;

private:
    // TextDecoder
    class UTF16Decoder : public TextDecoder
    {
    public:
        UTF16Decoder() { reset(); }
        virtual int getMinByteCount() override { return 2; }
        virtual int getMaxByteCount() override { return 4; }
        virtual bool canRemain() override { return true; }
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, DecodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return m_lastLeadWord == 0x0000 && m_lastLeadWord == 0x0000; }
        virtual void reset() override
        {
            mUsedDefaultCharCount = 0;
            m_lastLeadByte = 0x00;
            m_lastLeadWord = 0x0000;
        }

    private:
        int mUsedDefaultCharCount;
        byte_t m_lastLeadByte;
        uint16_t m_lastLeadWord;
    };

    // TextEncoder
    class UTF16Encoder : public TextEncoder
    {
    public:
        UTF16Encoder() { reset(); }
        virtual int getMinByteCount() override { return 2; }
        virtual int getMaxByteCount() override { return 4; }
        virtual bool canRemain() override { return true; }
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, EncodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return 0; }
        virtual bool completed() override { return true; }
        virtual void reset() override { m_hiSurrogate = 0; }

    private:
        UTF16 m_hiSurrogate;
    };
};

} // namespace ln
