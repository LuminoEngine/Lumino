#pragma once
#include "UIContainerElement.hpp"

namespace ln {
class Material;

// 複数の UIElement をグループ化する。UIControl と似ているが、フォーカスなどは持たず、ユーザー入力に対するフィードバックはない。
class UICard
    : public UIContainerElement
{
public:
    static Ref<UICard> create();

LN_CONSTRUCT_ACCESS:
    UICard();
	void init();

protected:

private:
};

} // namespace ln

