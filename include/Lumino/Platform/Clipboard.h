/**
	@file	Clipboard.h
*/
#pragma once
#include <Lumino/Base/String.h>

namespace Lumino
{
namespace Platform
{

/**
	@brief	システムクリップボードとのデータ転送を行います。
*/
class Clipboard
{
public:

	/**
		@brief	クリップボードに文字列データを格納します。
	*/
	static void SetText(Window* window, const String& text);

	/**
		@brief	クリップボードに格納されている文字列データを取得します。
		@return	格納されているデータが文字列ではない場合、空の文字列を返します。
	*/
	static String GetText(Window* window);
};

} // namespace Platform
} // namespace Lumino
