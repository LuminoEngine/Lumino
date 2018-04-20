
#include "Internal.hpp"
#include <Lumino/Base/String.hpp>
#include <Lumino/Text/Encoding.hpp>

namespace ln {

std::string StringRef::toStdString() const
{
	std::vector<byte_t> bytes = TextEncoding::systemMultiByteEncoding()->encode(getBegin(), length());
	return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

std::wstring StringRef::toStdWString() const
{
	std::vector<byte_t> bytes = TextEncoding::wideCharEncoding()->encode(getBegin(), length());
	return std::wstring(reinterpret_cast<const wchar_t*>(bytes.data()), bytes.size() / sizeof(wchar_t));
}

} // namespace ln
