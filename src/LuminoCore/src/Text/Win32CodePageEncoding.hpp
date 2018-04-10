
#include <Windows.h>
#include <Lumino/Text/Encoding.hpp>

namespace ln {

class Win32CodePageEncoding : public TextEncoding
{
public:
    Win32CodePageEncoding(UINT codePage);
    virtual ~Win32CodePageEncoding(){};

public:
    // override TextEncoding
    virtual const TTCHAR* getName() const override { return reinterpret_cast<const char16_t*>(m_name.c_str()); }
    virtual int getMinByteCount() const override { return 1; }
    virtual int getMaxByteCount() const override { return m_cpInfo.MaxCharSize; }
    virtual TextDecoder* createDecoder() const override { return LN_NEW Win32CodePageDecoder(m_cpInfo); }
    virtual TextEncoder* createEncoder() const override { return LN_NEW Win32CodePageEncoder(m_cpInfo); }
    virtual byte_t* getPreamble() const override { return NULL; }
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const override;
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const override;

private:
    CPINFOEX m_cpInfo;
    std::wstring m_name;

public:
    // TextDecoder
    class Win32CodePageDecoder : public TextDecoder
    {
    public:
        Win32CodePageDecoder(const CPINFOEX& cpInfo);
        virtual int getMinByteCount() override { return 1; }
        virtual int getMaxByteCount() override { return m_maxByteCount; }
        virtual bool canRemain() override { return m_canRemain; }
        virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, DecodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return m_usedDefaultCharCount; }
        virtual bool completed() override { return m_lastLeadByte == 0; }
        virtual void reset() override
        {
            m_usedDefaultCharCount = 0;
            m_lastLeadByte = 0x0000;
        }

    private:
        int m_codePage;        // コードページ
        int m_maxByteCount;    // 1文字の最大バイト数
        byte_t m_lastLeadByte; // 前回の convert で末尾に見つかった先行バイトを保存する
        int m_usedDefaultCharCount;
        bool m_canRemain;
    };

    // TextEncoder
    class Win32CodePageEncoder : public TextEncoder
    {
    public:
        Win32CodePageEncoder(const CPINFOEX& cpInfo);
        virtual int getMinByteCount() override { return 1; }
        virtual int getMaxByteCount() override { return m_maxByteCount; }
        virtual bool canRemain() override { return m_canRemain; }
        virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, EncodeResult* outResult) override;
        virtual int usedDefaultCharCount() override { return m_usedDefaultCharCount; }
        virtual bool completed() override { return true; }
        virtual void reset() override { m_usedDefaultCharCount = 0; }

    private:
        int m_codePage;     // コードページ
        int m_maxByteCount; // 1文字の最大バイト数
        int m_usedDefaultCharCount;
        bool m_canRemain;
    };
};

} // namespace ln
