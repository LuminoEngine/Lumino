
#include "../Internal.h"
#include <Lumino/Base/Uuid.h>

#if defined(LN_OS_WIN32)
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

namespace ln {

//==============================================================================
// Uuid
//==============================================================================

//------------------------------------------------------------------------------
uint8_t DigitCharToHex(int ch)
{
	if ('0' <= ch && ch <= '9') return ch - 48;
	if ('a' <= ch && ch <= 'f') return ch - 87;
	if ('A' <= ch && ch <= 'F') return ch - 55;
	return 0;
}

//------------------------------------------------------------------------------
uint8_t CharPairToHex(int a, int b)
{
	return DigitCharToHex(a) * 16 + DigitCharToHex(b);
}

//------------------------------------------------------------------------------
TCHAR HexToDigitChar(uint8_t hex)
{
	if (hex >= 10) return 'A' + (hex - 10);
	return '0' + hex;
}

//------------------------------------------------------------------------------
void HexToCharPair(uint8_t hex, TCHAR* ch1, TCHAR* ch2)
{
	*ch1 = HexToDigitChar((hex >> 4) & 0x0F);
	*ch2 = HexToDigitChar((hex) & 0x0F);
}

//------------------------------------------------------------------------------
Uuid Uuid::Generate()
{
#if defined(LN_OS_WIN32)
	GUID guid;
	CoCreateGuid(&guid);
	const uint8_t bytes[16] = 
	{
		(guid.Data1 >> 24) & 0xFF,
		(guid.Data1 >> 16) & 0xFF,
		(guid.Data1 >> 8) & 0xFF,
		(guid.Data1) & 0xff,

		(guid.Data2 >> 8) & 0xFF,
		(guid.Data2) & 0xff,

		(guid.Data3 >> 8) & 0xFF,
		(guid.Data3) & 0xFF,

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
#else
	uuid_t id;
	uuid_generate(id);
	return Uuid(id);
#endif
}

//------------------------------------------------------------------------------
Uuid::Uuid()
{
	m_data = std::vector<byte_t>(16, 0);
}

//------------------------------------------------------------------------------
Uuid::Uuid(const StringRef& uuidText)
{
	char ch1;
	bool lookingCh1 = true;

	// parse UUID text
	for (int i = 0; i < uuidText.GetLength(); i++)
	{
		TCHAR ch = uuidText[i];
		if (ch == _T('-') || ch == _T('{') || ch == _T('}')) continue;

		if (lookingCh1)
		{
			ch1 = ch;
			lookingCh1 = false;
		}
		else
		{
			m_data.push_back(CharPairToHex(ch1, ch));
			lookingCh1 = true;
		}
	}
}

//------------------------------------------------------------------------------
Uuid::Uuid(const uint8_t* bytes)
{
	LN_FAIL_CHECK_ARG(bytes != nullptr) return;
	m_data.assign(bytes, bytes + 16);
}

//------------------------------------------------------------------------------
Uuid::Uuid(const Uuid &other)
{
	m_data = other.m_data;
}

//------------------------------------------------------------------------------
Uuid& Uuid::operator=(const Uuid& other)
{
	m_data = other.m_data;
	return *this;
}

//------------------------------------------------------------------------------
bool Uuid::operator==(const Uuid& other) const
{
	return m_data == other.m_data;
}

//------------------------------------------------------------------------------
bool Uuid::operator!=(const Uuid& other) const
{
	return !((*this) == other);
}

//------------------------------------------------------------------------------
String Uuid::ToString() const
{
	TCHAR str[] = _T("{00000000-0000-0000-0000-000000000000}");

	HexToCharPair(m_data[0], &str[1], &str[2]);
	HexToCharPair(m_data[1], &str[3], &str[4]);
	HexToCharPair(m_data[2], &str[5], &str[6]);
	HexToCharPair(m_data[3], &str[7], &str[8]);

	HexToCharPair(m_data[4], &str[10], &str[11]);
	HexToCharPair(m_data[5], &str[12], &str[13]);

	HexToCharPair(m_data[6], &str[15], &str[16]);
	HexToCharPair(m_data[7], &str[17], &str[18]);

	HexToCharPair(m_data[8], &str[20], &str[21]);
	HexToCharPair(m_data[9], &str[22], &str[23]);

	HexToCharPair(m_data[10], &str[25], &str[26]);
	HexToCharPair(m_data[11], &str[27], &str[28]);
	HexToCharPair(m_data[12], &str[29], &str[30]);
	HexToCharPair(m_data[13], &str[31], &str[32]);
	HexToCharPair(m_data[14], &str[33], &str[34]);
	HexToCharPair(m_data[15], &str[35], &str[36]);

	return String(str);
}

} // namespace ln

