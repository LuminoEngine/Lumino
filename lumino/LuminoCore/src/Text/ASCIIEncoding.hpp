
#pragma once
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/Base/String.hpp>

namespace ln {

class ASCIIEncoding : public TextEncoding
{
public:
    static const String Name;

    ASCIIEncoding();
    virtual ~ASCIIEncoding(){};

    // override TextEncoding
    virtual const String& name() const override { return Name; }
    virtual int minByteCount() const override { return 1; }
    virtual int maxByteCount() const override { return 1; }
    virtual TextDecoder* createDecoder() override { return LN_NEW ASCIIDecoder(this); }
    virtual TextEncoder* createEncoder() override { return LN_NEW ASCIIEncoder(this); }
    virtual byte_t* preamble() const override { return nullptr; }
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override { return (int)bufferSize; }
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override { return 0; }

    // TextDecoder
    class ASCIIDecoder : public TextDecoder
    {
    public:
        ASCIIDecoder(TextEncoding* encoding)
            : TextDecoder(encoding)
        {
            reset();
        }
        virtual bool canRemain() override { return true; }
#if LN_USTRING32
        virtual bool convertToUTF32(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#else
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;
#endif
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return true; }
        virtual void reset() override { mUsedDefaultCharCount = 0; }

    private:
        int mUsedDefaultCharCount;
    };

    // TextEncoder
    class ASCIIEncoder : public TextEncoder
    {
    public:
        ASCIIEncoder(TextEncoding* encoding)
            : TextEncoder(encoding)
        {
            reset();
        }
        virtual bool canRemain() override { return true; }
#if LN_USTRING32
        virtual bool convertFromUTF32(const UTF32* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) override;
#else
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) override;
#endif
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return true; }
        virtual void reset() override { mUsedDefaultCharCount = 0; }

    private:
        int mUsedDefaultCharCount;
    };
};

} // namespace ln
