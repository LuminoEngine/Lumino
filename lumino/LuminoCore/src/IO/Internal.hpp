#pragma once
#include "../Internal.hpp"

namespace ln {
namespace detail
{
static const int LocalPathBaseLength = 255;

// 文字列が短ければスタックに、長ければヒープに確保する
template<typename TChar>
class GenericStaticallyLocalPath
{
public:
	GenericStaticallyLocalPath();
	GenericStaticallyLocalPath(const char* path, int len);
	GenericStaticallyLocalPath(const wchar_t* path, int len);
	GenericStaticallyLocalPath(const char16_t* path, int len);
	
	const TChar* c_str() const { return m_path.empty() ? m_static : m_path.c_str(); }
	const std::basic_string<TChar>& getPath() const { return m_path; }
	int getLength() const { return m_length; }
	bool isEmpty() const { return m_length == 0; }

	bool isStatic() const { return m_path.empty(); }

private:
	TChar						m_static[LocalPathBaseLength + 1];
	int							m_length;
	std::basic_string<TChar>	m_path;
};

typedef GenericStaticallyLocalPath<char>	StaticallyLocalPathA;
typedef GenericStaticallyLocalPath<wchar_t>	StaticallyLocalPathW;

} // namespace detail
} // namespace ln
