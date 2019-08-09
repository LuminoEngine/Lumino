#pragma once
#include <LuminoEngine/UI/UIElement.hpp>

namespace ln {
namespace detail {
class UITextLayout;
class UILogicalLine;

enum class UICursorAlignment
{
    /** カーソルをグリフの左側に配置する */
    Left,

    /** カーソルをグリフの右側に配置する */
    Right,
};

enum class UITextDirection 
{
    LeftToRight,
    RightToLeft,
};

enum class UICursorMoveMethod
{
    /** 上下左右に移動します。 */
    Cardinal,

    /** 指定した位置に移動します。 */
    Position,
};

enum class UICursorMoveGranularity
{
    /** 文字グリフ単位で移動する */
    Character,

    /** 単語単位で移動する */
    Word,
};

enum class UICursorAction
{
    /** カーソルを移動するだけです。 */
    MoveCursor,

    /** カーソルを移動し、通過するテキストを選択します。 */
    SelectText,
};


struct UITextLocation
{
    UITextLocation() {}
    UITextLocation(int line, int ofs) : lineIndex(line), offset(ofs) {}

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
    bool contains(int index) const { return index >= beginIndex && index < endIndex; }
    bool inclusiveContains(int index) const { return index >= beginIndex && index <= endIndex; }
    UITextRange intersect(const UITextRange& other) const
    {
        UITextRange Intersected(std::max(beginIndex, other.beginIndex), std::min(endIndex, other.endIndex));
        if (Intersected.endIndex <= Intersected.beginIndex) {
            return UITextRange(0, 0);
        }
        return Intersected;
    }


	int beginIndex; // (unit: Char)
	int endIndex;   // (unit: Char)
};

struct UICursorInfo
{
    UITextLocation position;

    UICursorAlignment alignment = UICursorAlignment::Left;

    UITextDirection textDirection = UITextDirection::LeftToRight;

    double interactionTime = 0;

    float preferredCursorScreenOffsetInLine = 0;    // 物理行内のオフセット (unit:dp)
};

// フォントスタイルが同じ一続きの文字列。１行の中に複数ある。
class UILogicalRun : public RefObject
{
public:
    UILogicalRun(UILogicalLine* owner, const UITextRange& range);
    Vector2 measure(Font* defaultFont) const;
    Vector2 measure(const UITextRange& range) const;

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
    UILogicalLine(UITextLayout* owner, const String& text);

public:	// TODO: private
    UITextLayout* m_ownerLayout;
	String m_text;
	List<Ref<UILogicalRun>> m_runs;
    //List<UITextLineHighlight> highlights;

};

// UILogicalLine::m_runs の範囲。
class UIPhysicalBlock : public RefObject
{
public:
    UIPhysicalBlock(UILogicalRun* run, const UITextRange& range, const Vector2& offset, const Vector2& size);

    UITextRange rangeInLogicalLine() const { return UITextRange(m_run->m_range.beginIndex + m_range.beginIndex, m_run->m_range.beginIndex + m_range.beginIndex + m_range.length()); }
    StringRef str() const { return m_run->substr(m_range); }
    float getLocalOffsetAt(int charIndex) const;

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
    UITextRange logicalRange;   // ↑の中の文字列範囲 (unit: Char)
};

// テキストの配置・描画
class UITextLayout : public Object
{
public:
	void setBaseTextStyle(Font* font, const Color& textColor);	// PhysicalLine再構築
	void setText(const StringRef& value);	// すべて再構築

    void updateCursorHighlight();
    void removeCursorHighlight();

	Size measure();
	void arrange(const Size& area);
	void render(UIRenderingContext* context);

    bool handleKeyDown(UIKeyEventArgs* e);

    bool isHorizontalFlow() const { return true; }

public:	// TODO: private
    void updatePreferredCursorScreenOffsetInLine();
    Vector2 getLocalOffsetFromLogicalLocation(const UITextLocation& loc) const;
    int getPhysicalLineIndexFromLogicalLocation(const UITextLocation& loc) const;
    void moveCursor(UICursorMoveMethod method, UICursorMoveGranularity granularity, const Vector2& dirOrPos, UICursorAction action);
    UITextLocation translateLocationToCharDirection(const UITextLocation& loc, int dir);
    static int moveToCandidate(const StringRef& text, int begin, int offset);


	Ref<Font> m_baseFont;
	Color m_baseTextColor;
	List<Ref<UILogicalLine>> m_logicalLines;
	List<Ref<UIPhysicalLine>> m_physicalLines;
    UICursorInfo m_cursorInfo;
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
    virtual void onRoutedEvent(UIEventArgs* e) override;
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

