#pragma once
#include "UIElement.hpp"

namespace ln {
	
// 編集機能は持たないが、選択可能なテキスト領域
class UITextArea
	: public UIElement
{
public:
	void setText(const StringRef& value);

LN_CONSTRUCT_ACCESS:
	UITextArea();
	void init();

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRender(UIRenderingContext* context) override;

private:
};

// 編集可能なテキスト領域。undo管理もしたりする。
class UIEditableTextArea
	: public UITextArea
{
public:

LN_CONSTRUCT_ACCESS:
	UIEditableTextArea();
	void init();

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;
	virtual void onRender(UIRenderingContext* context) override;

private:
};


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
    virtual void onRender(UIRenderingContext* context) override;

private:
};



} // namespace ln

