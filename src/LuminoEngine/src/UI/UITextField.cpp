
#include "Internal.hpp"
#include <LuminoEngine/UI/UITextField.hpp>

namespace ln {


struct UITextLocation
{
	int lineIndex = 0;	// 0~
	int offset = 0;		// 行内の Char インデックス
};

struct UITextSelection
{
	UITextLocation locationA;
	UITextLocation locationB;
	// A, B の前後関係は問わない
};

struct UITextRange
{
	int beginIndex = 0;
	int endIndex = 0;
	// Char 単位
};

// フォントスタイルが同じ一続きの文字列。１行の中に複数ある。
class UILogicalRun : public RefObject
{
public:

public:	// TODO: private

};

// 論理行。入力文字列を改行で切ったもの。
class UILogicalLine : public RefObject
{
public:

public:	// TODO: private
	String m_text;
	List<Ref<UILogicalRun>> m_runs;
};

// UILogicalLine::m_runs の範囲。
class UIRunBlock : public RefObject
{
public:


public:	// TODO: private
	UILogicalRun* m_run;
};

// 物理行。LogicalLine に対して、さらに折り返しなどを考慮したもの。
class UIPhysicalLine : public RefObject
{
public:

public:	// TODO: private
	List<UIRunBlock> m_runBlocks;
};

// テキストの配置・描画
class UITextLayout : public Object
{
public:

public:	// TODO: private
	List<Ref<UILogicalLine>> m_logicalLines;
	List<Ref<UIPhysicalLine>> m_physicalLines;
	// viewsize
	// scrolloffset
};


//==============================================================================
// UITextArea

UITextArea::UITextArea()
{
}

void UITextArea::init()
{
	UIElement::init();
}

void UITextArea::setText(const StringRef& value)
{
}

Size UITextArea::measureOverride(const Size& constraint)
{
	return UIElement::measureOverride(constraint);
}

void UITextArea::onRender(UIRenderingContext* context)
{
	UIElement::onRender(context);
}

//==============================================================================
// UIEditableTextArea

UIEditableTextArea::UIEditableTextArea()
{
}

void UIEditableTextArea::init()
{
	UITextArea::init();
}

Size UIEditableTextArea::measureOverride(const Size& constraint)
{
	return UITextArea::measureOverride(constraint);
}

void UIEditableTextArea::onRender(UIRenderingContext* context)
{
	UITextArea::onRender(context);
}

//==============================================================================
// UITextField

UITextField::UITextField()
{
}

void UITextField::init()
{
	UIElement::init();
}

void UITextField::setText(const StringRef& value)
{
}

Size UITextField::measureOverride(const Size& constraint)
{
	return UIElement::measureOverride(constraint);
}

void UITextField::onRender(UIRenderingContext* context)
{
	UIElement::onRender(context);
}

} // namespace ln

