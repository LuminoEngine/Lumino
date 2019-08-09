#pragma once
#include <LuminoEngine/UI/UIElement.hpp>

namespace ln {
namespace detail {

struct UITextLocation
{
	int lineIndex = 0;  // 0 start
	int offset = 0;     // offset from line head (unit: Char. 2 point for surrogates)
};

struct UITextSelection
{
	UITextLocation locationA;
	UITextLocation locationB;
	// A, B の前後関係は問わない

	const UITextLocation& getBegin() const
	{
		if (locationA.lineIndex == locationB.lineIndex) {
			if (locationA.offset < locationB.offset) {
				return locationA;
			}
			return locationB;
		}
		else if (locationA.lineIndex < locationB.lineIndex) {
			return locationA;
		}

		return locationB;
	}

	const UITextLocation& getEnd() const
	{
		if (locationA.lineIndex == locationB.lineIndex) {
			if (locationA.offset > locationB.offset) {
				return locationA;
			}
			return locationB;
		}
		else if (locationA.lineIndex > locationB.lineIndex) {
			return locationA;
		}
		return locationB;
	}
};

struct UITextRange
{
	static void splitLineRanges(const String& str, List<UITextRange>* outRanges);

	int beginIndex = 0; // (unit: Char)
	int endIndex = 0;   // (unit: Char)
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
	void setBaseTextStyle(Font* font, const Color& textColor);	// PhysicalLine再構築
	void setText(const StringRef& value);	// すべて再構築


	Size measure();
	void arrange(const Size& area);
	void render(UIRenderingContext* context);

public:	// TODO: private
	Ref<Font> m_baseFont;
	Color m_baseTextColor;
	List<Ref<UILogicalLine>> m_logicalLines;
	List<Ref<UIPhysicalLine>> m_physicalLines;
	// viewsize
	// scrolloffset
	bool m_dirtyPhysicalLines = true;
};









} // namespace detail


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
	Ref<detail::UITextLayout> m_textLayout;
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

} // namespace ln

