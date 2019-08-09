#pragma once
#include "UIElement.hpp"

namespace ln {
class UIEditableTextArea;

// UIEditableTextArea を内包する。
// 前後にアイコンやボタンを置いたり、枠を描画して、１つの入力欄とする。
class UITextField
    : public UIElement
{
public:
	void setText(const StringRef& value);

LN_CONSTRUCT_ACCESS:
	UITextField();
	void init();

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
    virtual void onRender(UIRenderingContext* context) override;

private:
	Ref<UIEditableTextArea> m_textArea;
};



} // namespace ln

