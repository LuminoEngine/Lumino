#pragma once
#include <LuminoEngine/UI/UIElement.hpp>

namespace ln {
namespace detail {
class UILogicalLine;

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
	static void splitLineRanges(const String& str, List<UITextRange>* outLines);

    UITextRange() : beginIndex(0), endIndex(0) {}
    UITextRange(int b, int e) : beginIndex(b), endIndex(e) {}

    int length() const { return endIndex - beginIndex; }

	int beginIndex; // (unit: Char)
	int endIndex;   // (unit: Char)
};

// フォントスタイルが同じ一続きの文字列。１行の中に複数ある。
class UILogicalRun : public RefObject
{
public:
    UILogicalRun(UILogicalLine* owner, const UITextRange& range);
    Vector2 measure(Font* defaultFont) const;

    int length() const { return m_range.length(); }
    StringRef substr(const UITextRange& range) const;

public:	// TODO: private
    UILogicalLine* m_ownerLine;
    UITextRange m_range;    // m_ownerLine->m_text のどの範囲を示すか
    Ref<Font> m_font;       // null の場合は UITextLayout のベースフォントを使う
};

// 論理行。入力文字列を改行で切ったもの。
class UILogicalLine : public RefObject
{
public:
    UILogicalLine(const String& text);

public:	// TODO: private
	String m_text;
	List<Ref<UILogicalRun>> m_runs;
    //List<UITextLineHighlight> highlights;

};

// UILogicalLine::m_runs の範囲。
class UIPhysicalBlock : public RefObject
{
public:
    UIPhysicalBlock(UILogicalRun* run, const UITextRange& range, const Vector2& offset, const Vector2& size);

    StringRef str() const { return m_run->substr(m_range); }

public:	// TODO: private
	UILogicalRun* m_run;
    UITextRange m_range; // m_run の中の範囲 (unit:Char)
    Vector2 m_offset; // uint:dp
    Vector2 m_size; // uint:dp
};

// 物理行。LogicalLine に対して、さらに折り返しなどを考慮したもの。
class UIPhysicalLine : public RefObject
{
public:

public:	// TODO: private
	List<Ref<UIPhysicalBlock>> m_runBlocks;

    //List<UILineViewHighlight> UnderlayHighlights;
    //List<UILineViewHighlight> OverlayHighlights;
    Vector2 offset; // uint:dp
    Vector2 size;   // unit:dp
    float lineHeight;   // 行高さ (unit:dp)
    int logicalIndex;   // 対応する UILogicalLine のインデックス
    UITextRange range;   // ↑の中の文字列範囲 (unit: Char)
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

    void handleKeyDown(UIKeyEventArgs* e);

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

