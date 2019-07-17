#pragma once
#include "UIContainerElement.hpp"

namespace ln {

// - フォーカスを持つことができる。
// - フォーカスの移動単位となる。
	// TODO: 廃止予定。WPF 参考に用意したが、Control固有のパラメータはフォーカスを持てるか、タブストップ持つかしか必要なくなった。
	// これらは UIElement の virutal メソッドなりで実装する予定なので、Control 自体不要となる。
class UIControl
	: public UIContainerElement
{
public:
    UIControl();
	void init();

private:
};

} // namespace ln

