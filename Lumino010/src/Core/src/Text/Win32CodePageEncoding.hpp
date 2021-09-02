
#define NOMINMAX
#include <Windows.h>
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/Base/String.hpp>

namespace ln {

class Win32CodePageEncoding : public TextEncoding
{
public:
    Win32CodePageEncoding(UINT codePage);
    virtual ~Win32CodePageEncoding(){};

public:
    // override TextEncoding
    virtual const String& name() const override { return m_name; }
    virtual int minByteCount() const override { return 1; }
    virtual int maxByteCount() const override { return m_cpInfo.MaxCharSize; }
    virtual TextDecoder* createDecoder() override { return LN_NEW Win32CodePageDecoder(this, m_cpInfo); }
    virtual TextEncoder* createEncoder() override { return LN_NEW Win32CodePageEncoder(this, m_cpInfo); }
    virtual byte_t* preamble() const override { return nullptr; }
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override;
	virtual bool convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;

private:
    CPINFOEX m_cpInfo;
	String m_name;

public:
    // TextDecoder
    class Win32CodePageDecoder : public TextDecoder
    {
    public:
        Win32CodePageDecoder(TextEncoding* encoding, const CPINFOEX& cpInfo);
        virtual bool canRemain() override { return m_canRemain; }
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return m_usedDefaultCharCount; }
        virtual bool completed() override { return m_lastLeadByte == 0; }
        virtual void reset() override
        {
            m_usedDefaultCharCount = 0;
            m_lastLeadByte = 0x0000;
        }

    private:
        int m_codePage;        // コードページ
        byte_t m_lastLeadByte; // 前回の convert で末尾に見つかった先行バイトを保存する
        int m_usedDefaultCharCount;
        bool m_canRemain;
    };

    // TextEncoder
    class Win32CodePageEncoder : public TextEncoder
    {
    public:
        Win32CodePageEncoder(TextEncoding* encoding, const CPINFOEX& cpInfo);
        virtual bool canRemain() override { return m_canRemain; }
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return m_usedDefaultCharCount; }
        virtual bool completed() override { return true; }
        virtual void reset() override { m_usedDefaultCharCount = 0; }

    private:
        int m_codePage;     // コードページ
        int m_usedDefaultCharCount;
        bool m_canRemain;
    };
};

} // namespace ln
