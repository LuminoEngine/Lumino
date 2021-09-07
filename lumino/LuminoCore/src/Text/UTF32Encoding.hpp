
#pragma once
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/Base/String.hpp>

namespace ln {

class UTF32Encoding : public TextEncoding
{
public:
	static const String NameLE;
	static const String NameBE;

    UTF32Encoding(bool bigEndian, bool byteOrderMark);
    virtual ~UTF32Encoding(){};

public:
    // override TextEncoding
    virtual const String& name() const override { return (m_bigEndian) ? NameBE : NameLE; }
    virtual int minByteCount() const override { return 4; }
    virtual int maxByteCount() const override { return 4; }
    virtual TextDecoder* createDecoder() override { return LN_NEW UTF32Decoder(this); }
    virtual TextEncoder* createEncoder() override { return LN_NEW UTF32Encoder(this); }
    virtual byte_t* preamble() const override;
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override { return 0; }
#if LN_USTRING32
    virtual bool convertToUTF32Stateless(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#else
	virtual bool convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#endif

private:
    bool m_bigEndian;
    bool m_byteOrderMark;

public:
    // TextDecoder
    class UTF32Decoder : public TextDecoder
    {
    public:
        UTF32Decoder(TextEncoding* encoding) : TextDecoder(encoding) { reset(); }
        virtual bool canRemain() override { return true; }
#if LN_USTRING32
        virtual bool convertToUTF32(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#else
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#endif
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
        UTF32Encoder(TextEncoding* encoding) : TextEncoder(encoding) { reset(); }
        virtual bool canRemain() override { return true; }
#if LN_USTRING32
        virtual bool convertFromUTF32(const UTF32* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) override;
#else
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) override;
#endif
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
