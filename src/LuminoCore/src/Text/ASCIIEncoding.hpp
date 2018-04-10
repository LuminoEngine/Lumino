
#pragma once
#include <Lumino/Text/Encoding.hpp>

namespace ln {

class ASCIIEncoding : public TextEncoding
{
public:
    ASCIIEncoding();
    virtual ~ASCIIEncoding(){};

    // override TextEncoding
    virtual const Char* getName() const override { return _TT("US-ASCII"); }
    virtual int getMinByteCount() const override { return 1; }
    virtual int getMaxByteCount() const override { return 1; }
    virtual TextDecoder* createDecoder() const override { return LN_NEW ASCIIDecoder(); }
    virtual TextEncoder* createEncoder() const override { return LN_NEW ASCIIEncoder(); }
    virtual byte_t* getPreamble() const override { return NULL; }
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override { return (int)bufferSize; }
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override { return 0; }

    // TextDecoder
    class ASCIIDecoder : public TextDecoder
    {
    public:
        ASCIIDecoder() { reset(); }
        virtual int getMinByteCount() override { return 1; }
        virtual int getMaxByteCount() override { return 1; }
        virtual bool canRemain() override { return true; }
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, DecodeResult* outResult) override;
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
        ASCIIEncoder() { reset(); }
        virtual int getMinByteCount() override { return 1; }
        virtual int getMaxByteCount() override { return 1; }
        virtual bool canRemain() override { return true; }
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, EncodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return mUsedDefaultCharCount; }
        virtual bool completed() override { return true; }
        virtual void reset() override { mUsedDefaultCharCount = 0; }

    private:
        int mUsedDefaultCharCount;
    };
};

} // namespace ln
