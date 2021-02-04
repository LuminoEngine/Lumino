
#include <LuminoCore/Base/Common.hpp>
#include <LuminoCore/Base/Uuid.hpp>

#if defined(LN_OS_WIN32)
#include <objbase.h>
#elif defined(__ANDROID__)
#else
#include <uuid/uuid.h>
#endif

namespace ln {

//==============================================================================
// Uuid

uint8_t DigitCharToHex(int ch)
{
    if ('0' <= ch && ch <= '9') return ch - 48;
    if ('a' <= ch && ch <= 'f') return ch - 87;
    if ('A' <= ch && ch <= 'F') return ch - 55;
    return 0;
}

uint8_t CharPairToHex(int a, int b)
{
    return DigitCharToHex(a) * 16 + DigitCharToHex(b);
}

Char HexToDigitChar(uint8_t hex)
{
    if (hex >= 10) return 'A' + (hex - 10);
    return '0' + hex;
}

void HexToCharPair(uint8_t hex, Char* ch1, Char* ch2)
{
    *ch1 = HexToDigitChar((hex >> 4) & 0x0F);
    *ch2 = HexToDigitChar((hex)&0x0F);
}

Uuid Uuid::generate()
{
#if defined(LN_OS_WIN32)
    GUID guid;
    CoCreateGuid(&guid);
    const uint8_t bytes[16] =
        {
            (uint8_t)((guid.Data1 >> 24) & 0xFF),
            (uint8_t)((guid.Data1 >> 16) & 0xFF),
            (uint8_t)((guid.Data1 >> 8) & 0xFF),
            (uint8_t)((guid.Data1) & 0xFF),

            (uint8_t)((guid.Data2 >> 8) & 0xFF),
            (uint8_t)((guid.Data2) & 0xFF),

            (uint8_t)((guid.Data3 >> 8) & 0xFF),
            (uint8_t)((guid.Data3) & 0xFF),

            guid.Data4[0],
            guid.Data4[1],
            guid.Data4[2],
            guid.Data4[3],
            guid.Data4[4],
            guid.Data4[5],
            guid.Data4[6],
            guid.Data4[7],
        };
    return Uuid(bytes);

#elif defined(LN_OS_ANDROID)
    // Android NDK は uuid サポートしていない。
    // 作るとしたら https://github.com/graeme-hill/crossguid/blob/master/src/guid.cpp
    // のように、JVMEnv の構造体のポインタをもらわないとならない。
    LN_NOTIMPLEMENTED();
#else
    uuid_t id;
    uuid_generate(id);
    return Uuid(id);
#endif
}

Uuid::Uuid()
{
    m_data = std::vector<byte_t>(16, 0);
}

Uuid::Uuid(const StringRef& uuidText)
{
    Char ch1;
    bool lookingCh1 = true;

    // parse UUID text
    for (int i = 0; i < uuidText.length(); i++) {
        Char ch = uuidText[i];
        if (ch == _LT('-') || ch == _LT('{') || ch == _LT('}') || ch == _LT('(') || ch == _LT(')')) continue;

        if (lookingCh1) {
            ch1 = ch;
            lookingCh1 = false;
        } else {
            m_data.push_back(CharPairToHex(ch1, ch));
            lookingCh1 = true;
        }
    }
}

Uuid::Uuid(const uint8_t* bytes)
{
    if (LN_REQUIRE(bytes != nullptr)) return;
    m_data.assign(bytes, bytes + 16);
}

Uuid::Uuid(const Uuid& other)
{
    m_data = other.m_data;
}

Uuid& Uuid::operator=(const Uuid& other)
{
    m_data = other.m_data;
    return *this;
}

bool Uuid::operator==(const Uuid& other) const
{
    return m_data == other.m_data;
}

bool Uuid::operator!=(const Uuid& other) const
{
    return !((*this) == other);
}

bool Uuid::isEmpty() const
{
    for (auto i : m_data) {
        if (i != 0) return false;
    }
    return true;
}

String Uuid::toString() const
{
    //Char str[] = _LT("{00000000-0000-0000-0000-000000000000}");
    // https://msdn.microsoft.com/ja-jp/library/97af8hh4(v=vs.80).aspx
    Char fmtD[] = _LT("00000000-0000-0000-0000-000000000000");

    Char* str = fmtD;
    HexToCharPair(m_data[0], &str[0], &str[1]);
    HexToCharPair(m_data[1], &str[2], &str[3]);
    HexToCharPair(m_data[2], &str[4], &str[5]);
    HexToCharPair(m_data[3], &str[6], &str[7]);

    HexToCharPair(m_data[4], &str[9], &str[10]);
    HexToCharPair(m_data[5], &str[11], &str[12]);

    HexToCharPair(m_data[6], &str[14], &str[15]);
    HexToCharPair(m_data[7], &str[16], &str[17]);

    HexToCharPair(m_data[8], &str[19], &str[20]);
    HexToCharPair(m_data[9], &str[21], &str[22]);

    HexToCharPair(m_data[10], &str[24], &str[25]);
    HexToCharPair(m_data[11], &str[26], &str[27]);
    HexToCharPair(m_data[12], &str[28], &str[29]);
    HexToCharPair(m_data[13], &str[30], &str[31]);
    HexToCharPair(m_data[14], &str[32], &str[33]);
    HexToCharPair(m_data[15], &str[34], &str[35]);

    return String(str);
}

} // namespace ln

//==============================================================================

namespace std {

// for unordered_map key
std::size_t hash<ln::Uuid>::operator()(const ln::Uuid& key) const
{
    return ln::CRCHash::compute((const char*)key.data().data(), static_cast<int>(key.data().size()));
}

} // namespace std
