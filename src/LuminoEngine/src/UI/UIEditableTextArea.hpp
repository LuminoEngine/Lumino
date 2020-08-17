#pragma once
#include <LuminoEngine/UI/UIElement.hpp>

namespace ln {
class UIActiveTimer;
namespace detail {
class UITextLayout;
class UILogicalLine;
class UIPhysicalLine;
class UILineHighlighter;

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
	bool isEmpty() const { return (endIndex - beginIndex) <= 0; }
    void offset(int amount) { beginIndex += amount; beginIndex = std::max(0, beginIndex);  endIndex += amount; endIndex = std::max(0, endIndex); }
    bool contains(int index) const { return index >= beginIndex && index < endIndex; }
    bool inclusiveContains(int index) const { return index >= beginIndex && index <= endIndex; }
    UITextRange intersect(const UITextRange& other) const
    {
        UITextRange intersected(std::max(beginIndex, other.beginIndex), std::min(endIndex, other.endIndex));
        if (intersected.endIndex <= intersected.beginIndex) {
            return UITextRange(0, 0);
        }
        return intersected;
    }

	bool operator==(const UITextRange& other) const
	{
		return beginIndex == other.beginIndex && endIndex == other.endIndex;
	}

	bool operator!=(const UITextRange& other) const
	{
		return !(*this == other);
	}


	int beginIndex; // (unit: Char)
	int endIndex;   // (unit: Char)
};

struct UICursorInfo
{
	// ドキュメント全体の絶対位置で表す
    UITextLocation position;

    UICursorAlignment alignment = UICursorAlignment::Left;

    UITextDirection textDirection = UITextDirection::LeftToRight;

    double interactionTime = 0;

    float preferredCursorScreenOffsetInLine = 0;    // 物理行内のオフセット (unit:dp)
};

// １行１つのインスタンス。UILogicalLine へ追加していく。
// 選択範囲が複数行にまたがるような場合は、その分だけ作られる。
struct UITextLineHighlight
{
	UITextRange range;	// 行内における範囲を示す。0は行頭。
	UILineHighlighter* lineHighlighter;
};

struct UIScreenRange
{
	float offset;	// (unit:dp)
	float length;	// (unit:dp)
};

struct UIPhysicalLineHighlight
{
	float offset;	// 物理行の行頭からのオフセット (unit:dp)
	float length;	// ハイライト範囲 (unit:dp)
	UILineHighlighter* lineHighlighter;
};

// 実際に行内のハイライト部分を描画するクラス。
// UITextLayout につきひとつのインスタンスが存在する。
// 具体的にどの範囲をハイライトするかは UITextLineHighlight を使って指定する。
class UILineHighlighter : public RefObject
{
public:
	// physicalOffset: 行頭からのオフセット (unit:dp)
	// physicalLength: ハイライト範囲 (unit:dp)
	virtual void onDraw(UIRenderingContext* context, const UIPhysicalLine* line, float physicalOffset, float physicalLength) const = 0;
};

// Caret 線
class UICursorCaretHighlighter : public UILineHighlighter
{
public:
	virtual void onDraw(UIRenderingContext* context, const UIPhysicalLine* line, float physicalOffset, float physicalLength) const override;
};



// フォントスタイルが同じ一続きの文字列。１行の中に複数ある。
class UILogicalRun : public RefObject
{
public:
    UILogicalRun(UILogicalLine* owner, const UITextRange& range);
    Vector2 measure(Font* defaultFont) const;
    Vector2 measure(const UITextRange& range) const;

    int length() const { return m_range.length(); }
	StringRef str() const;
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
	String m_text;	// 改行文字を "含む". テキストエディタ作るわけじゃないけど、NewLineCode 表示したりするのに必要となる
	List<Ref<UILogicalRun>> m_runs;
    List<UITextLineHighlight> highlights;

};

// UILogicalLine::m_runs をさらに分割した範囲。折り返しなどのため。
class UIPhysicalBlock : public RefObject
{
public:
    UIPhysicalBlock(UILogicalRun* run, const UITextRange& range, const Vector2& offset, const Vector2& size, const UITextRange& rangeInPhysicalLine);

    UITextRange rangeInLogicalLine() const { return UITextRange(m_run->m_range.beginIndex + m_range.beginIndex, m_run->m_range.beginIndex + m_range.beginIndex + m_range.length()); }
	int length() const { return m_range.length(); }
	StringRef str() const { return m_run->substr(m_range); }
    float getLocalOffsetAt(int charIndex) const;
	UIScreenRange getLocalScreenRange(const UITextRange& range) const;

public:	// TODO: private
	UILogicalRun* m_run;
    UITextRange m_range; // m_run の中の範囲 (unit:Char)
    Vector2 m_offset; // uint:dp, スクリーン上の絶対位置
    Vector2 m_size; // uint:dp
	UITextRange m_rangeInPhysicalLine;	// このブロックが含まれる物理行内の先頭からの Char 範囲
};

// 物理行。LogicalLine に対して、さらに折り返しなどを考慮したもの。
class UIPhysicalLine : public RefObject
{
public:
	// この物理行内の先頭からの range に対応する物理範囲 (dp 単位) を求める。
	// 位置だけほしいときは range.length を 0 にしておく。
	// return: この UIPhysicalLine の左上を 0 とした範囲。
	UIScreenRange getLocalScreenRange(const UITextRange& range) const;

public:	// TODO: private
	List<Ref<UIPhysicalBlock>> m_runBlocks;

    Vector2 offset; // uint:dp
    Vector2 size;   // unit:dp
    float lineHeight;   // 行高さ (unit:dp)
    int logicalIndex;   // 対応する UILogicalLine のインデックス
    UITextRange logicalRange;   // ↑の中の文字列範囲 (unit: Char. 0 は LogicalLine の行頭)

	// 背面ハイライト
	List<UIPhysicalLineHighlight> m_underlayHighlights;

	// 全面
	List<UIPhysicalLineHighlight> m_overlayHighlights;

};

// テキストの配置・描画
class UITextLayout : public Object
{
public:
	UITextLayout();
	void setBaseTextStyle(Font* font, const Color& textColor);	// PhysicalLine再構築
	void setText(const StringRef& value);	// すべて再構築
	const String& boundText() const { return m_boundText; }
	void clearText();
	// カーソル移動はしない
	void insertAt(const UITextLocation& loc, const StringRef& text);


	Size measure();
	void arrange(const Size& area);
	void layoutHighlights();
	void render(UIRenderingContext* context);

    bool handleKeyDown(UIKeyEventArgs* e);
    bool handleTypeChar(Char ch);

    bool isHorizontalFlow() const { return true; }

	void updateCursorHighlight();
	void removeCursorHighlight();

public:	// TODO: private
	void updateBoundText();
    void updatePreferredCursorScreenOffsetInLine();
    Vector2 getLocalOffsetFromLogicalLocation(const UITextLocation& loc) const;
    int getPhysicalLineIndexFromLogicalLocation(const UITextLocation& loc) const;
    void moveCursor(UICursorMoveMethod method, UICursorMoveGranularity granularity, const Vector2& dirOrPos, UICursorAction action);
    UITextLocation translateLocationToCharDirection(const UITextLocation& loc, int dir);
    
	// サロゲート文字などを考慮し、begin から offset 文字グリフ分だけ前後に移動したいときの Char 数を計算する。
	static int moveToCandidate(const StringRef& text, int begin, int offset);


	Ref<Font> m_baseFont;
	Color m_baseTextColor;
	List<Ref<UILogicalLine>> m_logicalLines;
	List<Ref<UIPhysicalLine>> m_physicalLines;
    UICursorInfo m_cursorInfo;
	//List<UITextLineHighlight> m_activeCursorHighlights;
	Ref<UICursorCaretHighlighter> m_cursorCaretHighlighter;
	// viewsize
	// scrolloffset
	bool m_dirtyPhysicalLines = true;
	bool m_dirtyHighlights = true;

	// 編集されたテキスト。getText() や、onTextChanged イベントに流すため、
	// 編集が発生するたびに m_logicalLines を結合して保持しておく。
	String m_boundText;
	String m_lineTerminator;
};



} // namespace detail


// 編集機能は持たないが、選択可能なテキスト領域。
// - UIElement を継承している。これは Timer イベントやキーボードイベント、D&D など、の処理を共通化するためのもの。
//   - UITextField だけではなく、NumericUpDown など様々な場所で使われる。
//	 - UITextArea 自体はフォーカスを持たない。WPF 同様、UITextField は Focusable=True だが、UITextArea(TextBlockView) は UITextArea=False.
//     親の UITextField などからイベントを送ってもらう。
// - スクロールする機能は持つが、スクロールバーは持たない。(スクロールバーを付けるのはTextBoxの役目)
// - UI4の SEditableText 相当。
class UITextArea
	: public UIElement
{
public:
	void setText(const StringRef& value);
	const String& text() const;

LN_CONSTRUCT_ACCESS:
	UITextArea();
	void init();

protected:
    virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
	virtual void onRender(UIRenderingContext* context) override;

private:
	void handleCursorTimerTickEvent(ln::UITimerEventArgs* e);

	Ref<detail::UITextLayout> m_textLayout;
	Ref<UIActiveTimer> m_cursorTimer;
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
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
	virtual void onRender(UIRenderingContext* context) override;

private:
};

} // namespace ln

