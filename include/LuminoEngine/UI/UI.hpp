#pragma once
#include "Common.hpp"

namespace ln {
	
/** UI モジュールに関係するユーティリティです。 */
LN_CLASS(Static)
class UI
{
public:
	static UITheme* mainTheme();

	/** add */
	LN_METHOD()
	static void add(UIElement* element);
	
	/** remove */
	LN_METHOD()
	static void remove(UIElement* element);
};

} // namespace ln

