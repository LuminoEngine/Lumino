
#include "../Internal.h"
#include <Lumino/Text/Encoding.h>
#include <Lumino/Base/StringU.h>
#include <Lumino/Base/StringHelper.h>
#include <Lumino/Base/RefObject.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// String
//==============================================================================

UString::UString()
{
	init();
}

UString::~UString()
{
	if (!isSSO())
	{
		LN_SAFE_RELEASE(m_data.core);
	}
}

UString::UString(const UString& str)
	: UString()
{
	copy(str);
}

UString::UString(UString&& str) LN_NOEXCEPT
	: UString()
{
	move(std::forward<UString>(str));
}

UString& UString::operator=(const UString& str)
{
	copy(str);
	return *this;
}

UString& UString::operator=(UString&& str) LN_NOEXCEPT
{
	move(std::forward<UString>(str));
	return *this;
}

UString::UString(const UString& str, int begin)
	: UString()
{
	assign(str.c_str(), begin, str.getLength());
}

UString::UString(const UString& str, int begin, int length)
	: UString()
{
	assign(str.c_str(), begin, length);
}

UString::UString(const UChar* str)
	: UString()
{
	assign(str);
}

UString::UString(const UChar* str, int begin)
	: UString()
{
	assign(str, begin);
}

UString::UString(const UChar* str, int begin, int length)
	: UString()
{
	assign(str, begin, length);
}

UString::UString(int count, UChar ch)
	: UString()
{
	assign(count, ch);
}

UString& UString::operator=(UChar ch)
{
	assign(&ch, 0, 1);
	return *this;
}

UString& UString::operator=(const UChar* str)
{
	assign(str);
	return *this;
}

bool UString::isEmpty() const
{
	if (isSSO())
	{
		return getSSOLength() == 0;
	}
	else
	{
		return !m_data.core || m_data.core->getLength() <= 0;
	}
}

void UString::clear()
{
	if (isSSO())
	{
		setSSOLength(0);
	}
	else
	{
		if (m_data.core)
		{
			m_data.core->clear();
		}
	}
}

void UString::init() LN_NOEXCEPT
{
	m_data.core = nullptr;
	m_data.sso.length = 0;
}

void UString::copy(const UString& str)
{
	memcpy(&m_data, &str.m_data, sizeof(m_data));
	if (isNonSSO())
	{
		LN_SAFE_ADDREF(m_data.core);
	}
}

void UString::move(UString&& str) LN_NOEXCEPT
{
	if (isNonSSO())
	{
		LN_SAFE_RELEASE(m_data.core);
	}

	memcpy(&m_data, &str.m_data, sizeof(m_data));
	str.init();
}

void UString::assign(const UChar* str)
{
	assign(str, 0, UStringHelper::strlen(str));
}

void UString::assign(const UChar* str, int begin)
{
	assign(str, begin, UStringHelper::strlen(str));
}

void UString::assign(const UChar* str, int begin, int length)
{
	if (str && *str)
	{
		if (length < SSOCapacity)
		{
			setSSOLength(length);
		}
		else
		{
			checkDetachShared();
			m_data.core->alloc(length);
		}

		UChar* buf = getBuffer();
		memcpy(buf, str + begin, sizeof(UChar) * length);
		buf[length] = '\0';
	}
}

void UString::assign(int count, UChar ch)
{
	if (count > 0)
	{
		if (count < SSOCapacity)
		{
			setSSOLength(count);
		}
		else
		{
			checkDetachShared();
			m_data.core->alloc(count);
		}

		UChar* buf = getBuffer();
		std::fill<UChar*, UChar>(buf, buf + count, ch);
	}
}

void UString::checkDetachShared()
{
	if (m_data.core && m_data.core->isShared())
	{
		m_data.core->release();
		m_data.core = nullptr;
	}
	if (!m_data.core)
	{
		m_data.core = LN_NEW detail::UStringCore();
		setNonSSO();
	}
}

UChar* UString::getBuffer()
{
	return (isSSO()) ? m_data.sso.buffer : m_data.core->get();
}

void UString::setSSOLength(int len)
{
	m_data.sso.length = 0x80 | (static_cast<size_t>(len) & 0x07);
}

int UString::getSSOLength() const
{
	return m_data.sso.length & 0x07;
}

void UString::setNonSSO()
{
	m_data.sso.length = 0;
}

//==============================================================================
// StringHelper
//==============================================================================

size_t UStringHelper::strlen(const UChar* str)
{
	size_t count = 0;
	for (; *str; ++str) ++count;
	return count;
}

int UStringHelper::compare(const UChar* str1, const UChar* str2)
{
	assert(str1);
	assert(str2);
	for (; *str1; ++str1, ++str2)
	{
		if (*str1 != *str2)
		{
			return (*str1 < *str2) ? -1 : 1;
		}
	}
	return (0);
}

LN_NAMESPACE_END
