#pragma once
#include "FontCore.hpp"

namespace ln {
namespace detail {

class FreeTypeFont
	: public FontCore
{
public:
    FreeTypeFont();
	void initialize();
	void dispose();

private:
};

} // namespace detail
} // namespace ln

