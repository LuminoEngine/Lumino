
#include <codecvt>
#include <Lumino/Base/StlHelper.hpp>
#include <Lumino/Base/String.hpp>

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
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	return cv.to_bytes(str);
}

static std::string makeStdString(const wchar_t* str, size_t len)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	return cv.to_bytes(str, str + len);
}

} // namespace detail
} // namespace ln
