﻿
#include "../Internal.h"
#include <memory>
#include <Lumino/Text/Encoding.h>
#include <Lumino/Base/StringU.h>
#include <Lumino/Base/StringHelper.h>
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Formatter.h>

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
	release();
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
	assign(str.c_str() + begin, str.getLength());
}

UString::UString(const UString& str, int begin, int length)
	: UString()
{
	assign(str.c_str() + begin, length);
}

UString::UString(const UChar* str)
	: UString()
{
	assign(str);
}

UString::UString(const UChar* str, int length)
	: UString()
{
	assign(str, length);
}

UString::UString(int count, UChar ch)
	: UString()
{
	assign(count, ch);
}

UString::UString(const char* str)
	: UString()
{
	assignFromCStr(str);
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
			checkDetachShared();
			m_data.core->clear();
		}
	}
}

int UString::indexOf(const UStringRef& str, int startIndex, CaseSensitivity cs) const
{
	//return StringTraits::indexOf(c_str(), getLength(), str.c_str(), str.getLength(), startIndex, cs);
	return StringTraits::indexOf(c_str(), getLength(), str.data(), str.getLength(), startIndex, cs);
}

int UString::indexOf(UChar ch, int startIndex, CaseSensitivity cs) const
{
	return StringTraits::indexOf(c_str(), getLength(), &ch, 1, startIndex, cs);
}

int UString::compare(const UStringRef& str1, int index1, const UStringRef& str2, int index2, int length, CaseSensitivity cs)
{
	const UChar* s1 = str1.data() + index1;
	const UChar* s2 = str2.data() + index2;
	return StringTraits::compare(s1, str1.getLength() - index1, s2, str2.getLength() - index2, length, cs);
}

void UString::init() LN_NOEXCEPT
{
	m_data.core = nullptr;
	m_data.sso.length = 0;
}

void UString::release() LN_NOEXCEPT
{
	if (isNonSSO())
	{
		LN_SAFE_RELEASE(m_data.core);
	}
}

void UString::copy(const UString& str)
{
	bool changed = false;
	if (isSSO() != str.isSSO()) changed = true;
	else if (isNonSSO() && (!m_data.core || m_data.core != m_data.core)) changed = true;

	if (changed)
	{
		release();
		memcpy(&m_data, &str.m_data, sizeof(m_data));
		if (isNonSSO())
		{
			LN_SAFE_ADDREF(m_data.core);
		}
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

// 中身は不定値となる
void UString::allocBuffer(int length)
{
	if (length < SSOCapacity)
	{
		release();
		setSSOLength(length);
	}
	else
	{
		checkDetachShared();
		m_data.core->resize(length);
	}
}

// 中身は以前のものが維持され、新しい領域は不定値となる
void UString::resizeBuffer(int length)
{
	if (isSSO())
	{
		if (length < SSOCapacity)
		{
			// SSO -> SSO
			setSSOLength(length);
		}
		else
		{
			// SSO -> NonSSO
			std::unique_ptr<detail::UStringCore> core(LN_NEW detail::UStringCore());
			core->resize(length);
			memcpy(core->get(), m_data.sso.buffer, std::min(getSSOLength(), length) * sizeof(UChar));
			core->get()[length] = '\0';
			m_data.core = core.get();
			core.release();
			setNonSSO();
		}
	}
	else
	{
		if (length < SSOCapacity)
		{
			// NonSSO -> SSO
			if (m_data.core)
			{
				detail::UStringCore* oldCore = m_data.core;
				setSSOLength(length);
				memcpy(m_data.sso.buffer, oldCore->get(), std::min(oldCore->getLength(), length) * sizeof(UChar));
				oldCore->release();
			}
			else
			{
				setSSOLength(length);
			}
		}
		else
		{
			//bool shared = false;

			// NonSSO -> NonSSO
			if (m_data.core)
			{
				if (m_data.core->isShared())
				{
					detail::UStringCore* oldCore = m_data.core;
					m_data.core = LN_NEW detail::UStringCore();
					m_data.core->resize(length);
					memcpy(m_data.core->get(), oldCore->get(), std::min(oldCore->getLength(), length) * sizeof(UChar));
					oldCore->release();
				}
				else
				{
					m_data.core->resize(length);
				}
			}
			else
			{
				m_data.core = LN_NEW detail::UStringCore();
				m_data.core->resize(length);
			}
		}
	}
}

void UString::assign(const UChar* str)
{
	assign(str, UStringHelper::strlen(str));
}

void UString::assign(const UChar* str, int length)
{
	if (str && *str)
	{
		allocBuffer(length);
		UChar* buf = getBuffer();
		memcpy(buf, str, sizeof(UChar) * length);
	}
	else
	{
		allocBuffer(0);
	}
}

void UString::assign(int count, UChar ch)
{
	if (count > 0)
	{
		allocBuffer(count);
		UChar* buf = getBuffer();
		std::fill<UChar*, UChar>(buf, buf + count, ch);
	}
}

void UString::assignFromCStr(const char* str, int length)
{
	// ASCII だけの文字列か調べる。ついでに文字数も調べる。
	length = (length < 0) ? INT_MAX : length;
	int len = 0;
	const char* pos = str;
	bool ascii = true;
	for (; *pos && len < length; ++pos, ++len)
	{
		if (isascii(*pos) == 0)
		{
			ascii = false;
		}
	}

	if (ascii)
	{
		allocBuffer(len);
		UChar* buf = getBuffer();
		for (int i = 0; i < len; ++i)
		{
			buf[i] = str[i];
		}
	}
	else
	{
		LN_NOTIMPLEMENTED();
		//Encoding* encoding = Encoding::getSystemMultiByteEncoding();
		//Ref<Decoder> decoder(encoding->createDecoder(), false);
		//getMaxByteCount
		//	decoder->convertToUTF16(str, length, )
	}
}

// 共有を切り離したあたらしい Core を作成し、NonSSO にする
void UString::checkDetachShared()
{
	if (isSSO())
	{
		m_data.core = nullptr;
	}
	else
	{
		if (m_data.core && m_data.core->isShared())
		{
			m_data.core->release();
			m_data.core = nullptr;
		}
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
	m_data.sso.buffer[len] = '\0';
}

int UString::getSSOLength() const
{
	return m_data.sso.length & 0x07;
}

void UString::setNonSSO()
{
	m_data.sso.length = 0;
}

void UString::append(const UChar* str, int length)
{
	int firstLen = getLength();
	resizeBuffer(firstLen + length);
	UChar* b = getBuffer() + firstLen;
	memcpy(b, str, length * sizeof(UChar));
}

//==============================================================================
// StringHelper
//==============================================================================

size_t UStringHelper::strlen(const UChar* str)
{
	if (!str) return 0;
	size_t count = 0;
	for (; *str; ++str) ++count;
	return count;
}

int UStringHelper::compare(const UChar* str1, const UChar* str2)
{
	str1 = (str1) ? str1 : u"";
	str2 = (str2) ? str2 : u"";
	for (; *str1; ++str1, ++str2)
	{
		if (*str1 != *str2)
		{
			return (*str1 < *str2) ? -1 : 1;
		}
	}
	return (0);
}

template<typename TChar, typename TValue>
static void toStringIntX(TValue v, TChar* outStr, int size)
{
	char buf[64];
	detail::StdCharArrayBuffer<char> b(buf, 64);
	std::ostream os(&b);
	os << v;
	const char* str = b.GetCStr();
	int i = 0;
	for (; *str && i < (size - 1); ++str, ++i)
	{
		outStr[i] = *str;
	}
	outStr[i] = '\0';
}

template<typename TChar>
void UStringHelper::toStringInt8(int8_t v, TChar* outStr, int size)
{
	toStringIntX((int32_t)v, outStr, size);
}
template void UStringHelper::toStringInt8<char>(int8_t v, char* outStr, int size);
template void UStringHelper::toStringInt8<wchar_t>(int8_t v, wchar_t* outStr, int size);
template void UStringHelper::toStringInt8<char16_t>(int8_t v, char16_t* outStr, int size);



//namespace fmt {

////==============================================================================
//// GenericFormatStringBuilder
////==============================================================================
//
//template class GenericFormatStringBuilder<char16_t>;
//
//template<typename TChar>
//GenericFormatStringBuilder<TChar>::GenericFormatStringBuilder()
//	: m_buffer()
//	, m_bufferUsed(0)
//{
//}
//
//} // namespace fmt

LN_NAMESPACE_END
