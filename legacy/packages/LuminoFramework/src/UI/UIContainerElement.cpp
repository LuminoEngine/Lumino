#include "Internal.hpp"
#include <LuminoFramework/UI/UIContainerElement.hpp>

namespace ln {

//==============================================================================
// UIContainerElement
//	デフォルトで Focus と TabStop を持つものとしてみる。
//	また、任意個数の論理子要素を追加できるようにしてみる。
//	WPF だとこの役目は ContentControl と ItemsControl に分かれているが、
//	UIContainerElement は複数要素追加可能 & ItemContainerGenerator の機能は持たない。
//	WPF だと Slider や TextBox は Control の一部であるが、論理子要素は持つことができない。
//	でも Lumino では持つことができる。この辺は Qt と似てるかも。
//	Visual要素は原理的には正しいけど扱うのは面倒なので、
//	ユーザーがちょっとカスタムコントロール作ったり装飾したりする用途で、複数要素追加可能としておく。
//	（あと、あまりクラスを増やし過ぎたくない）

UIContainerElement::UIContainerElement()
{
}

bool UIContainerElement::init()
{
    return UIControl::init();
}

//==============================================================================
// UIFrame

UIFrame::UIFrame()
{
}

void UIFrame::init()
{
    UIContainerElement::init();
}

} // namespace ln

