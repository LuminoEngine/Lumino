
#include <LuminoCore/Base/StlHelper.hpp>
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/Base/UnicodeUtils.hpp>

namespace ln {
namespace detail {

std::string StdStringHelper::makeStdString(const String& str)
{
	return str.toStdString();
}

std::string StdStringHelper::makeStdString(const char* str)
{
	return std::string(str);
}

std::string StdStringHelper::makeStdString(const char* str, size_t len)
{
	return std::string(str, len);
}

std::string StdStringHelper::makeStdString(const wchar_t* str)
{
	return makeStdString(str, wcslen(str));
}

std::string StdStringHelper::makeStdString(const wchar_t* str, size_t len)
{
#ifdef LN_WCHAR_16
	std::vector<UTF8> result(len * 6);
	UTFConversionOptions options;
	options.ReplacementChar = '?';
	UnicodeUtils::convertUTF16toUTF8(str, len, result.data(), result.size(), &options);
	return std::string(reinterpret_cast<const char*>(result.data()));
#else
	std::vector<UTF8> result(len * 6);
	UTFConversionOptions options;
	options.ReplacementChar = '?';
	UnicodeUtils::convertUTF32toUTF8(reinterpret_cast<const UTF32*>(str), len, result.data(), result.size(), &options);
	return std::string(reinterpret_cast<const char*>(result.data()));
#endif
}

} // namespace detail
} // namespace ln
