
#pragma once
#include <Lumino/Text/Encoding.hpp>

namespace ln {

class UTF32Encoding : public TextEncoding
{
public:
    UTF32Encoding(bool bigEndian, bool byteOrderMark);
    virtual ~UTF32Encoding(){};

public:
    // override TextEncoding
    virtual const Char* getName() const override { return (m_bigEndian) ? _TT("UTF-32BE") : _TT("UTF-32LE"); }
    virtual int getMinByteCount() const override { return 4; }
    virtual int getMaxByteCount() const override { return 4; }
    virtual TextDecoder* createDecoder() const override { return LN_NEW UTF32Decoder(); }
    virtual TextEncoder* createEncoder() const override { return LN_NEW UTF32Encoder(); }
    virtual byte_t* getPreamble() const override;
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override { return 0; }

private:
    bool m_bigEndian;
    bool m_byteOrderMark;

public:
    // TextDecoder
    class UTF32Decoder : public TextDecoder
    {
    public:
        UTF32Decoder() { reset(); }
        virtual int getMinByteCount() override { return 4; }
        virtual int getMaxByteCount() override { return 4; }
        virtual bool canRemain() override { return true; }
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, DecodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return m_lastLeadBytesCount == 0; }
        virtual void reset() override
        {
            mUsedDefaultCharCount = 0;
            m_lastLeadBytesCount = false;
        }

    private:
        int mUsedDefaultCharCount;
        byte_t m_lastLeadBytes[4];
        int m_lastLeadBytesCount;
    };

    // TextEncoder
    class UTF32Encoder : public TextEncoder
    {
    public:
        UTF32Encoder() { reset(); }
        virtual int getMinByteCount() override { return 4; }
        virtual int getMaxByteCount() override { return 4; }
        virtual bool canRemain() override { return true; }
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, EncodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return m_hiSurrogate == 0x0000; }
        virtual void reset() override
        {
            mUsedDefaultCharCount = 0;
            m_hiSurrogate = 0x0000;
        }

    private:
        int mUsedDefaultCharCount;
        UTF16 m_hiSurrogate;
    };
};

} // namespace ln
