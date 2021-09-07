
#pragma once
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/Base/String.hpp>

namespace ln {

class UTF16Encoding : public TextEncoding
{
public:
	static const String NameLE;
	static const String NameBE;

    UTF16Encoding(bool bigEndian, bool byteOrderMark);
    virtual ~UTF16Encoding(){};

public:
    // override TextEncoding
    virtual const String& name() const override { return (m_bigEndian) ? NameBE : NameLE; }
    virtual int minByteCount() const override { return 2; }
    virtual int maxByteCount() const override { return 4; }
    virtual TextDecoder* createDecoder() override { return LN_NEW UTF16Decoder(this); }
    virtual TextEncoder* createEncoder() override { return LN_NEW UTF16Encoder(this); }
    virtual byte_t* preamble() const override;
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override;
#if LN_USTRING32
    virtual bool convertToUTF32Stateless(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#else
    virtual bool convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#endif

private:
    bool m_bigEndian;
    bool m_byteOrderMark;

private:
    // TextDecoder
    class UTF16Decoder : public TextDecoder
    {
    public:
        UTF16Decoder(TextEncoding* encoding) : TextDecoder(encoding) { reset(); }
        virtual bool canRemain() override { return true; }
#if LN_USTRING32
        virtual bool convertToUTF32(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#else
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#endif
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
        UTF16Encoder(TextEncoding* encoding) : TextEncoder(encoding) { reset(); }
        virtual bool canRemain() override { return true; }
#if LN_USTRING32
        virtual bool convertFromUTF32(const UTF32* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) override;
#else
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) override;
#endif
        virtual int usedDefaultCharCount() override { return 0; }
        virtual bool completed() override { return true; }
        virtual void reset() override { m_hiSurrogate = 0; }

    private:
        UTF16 m_hiSurrogate;
    };
};

} // namespace ln
