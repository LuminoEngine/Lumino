#pragma once

namespace ln {
class UIElement;
class UIStyle;

class UIContext
	: public Object
{
public:

public: // TODO: internal
    void setLayoutRootElement(UIElement* element);	// 通常は MainWindow。もし１つのウィンドウ内に複数の Context を含める場合は
    void addElement(UIElement* element);
    bool updateMouseHover(const Point& mousePos);
    UIElement* mouseHoverElement() const { return m_mouseHoverElement; }

	// 各更新の起点。UIContext には複数の UIFrameWindow を追加することもできるので、UIFrameWindow をツリー更新のルートとするのはちょっと違う。
	// GameEngine としてなら、メインループから。IDE としてなら、メッセージループから呼び出してもらうのが自然だろう。
	// ひとまず、UIContext の作成元となっている Manager から呼び出してもらうことにする。
	void updateStyleTree();
	void updateLayoutTree();


LN_CONSTRUCT_ACCESS:
    UIContext();
    virtual ~UIContext() = default;
	void init();

private:
    Ref<UIElement> m_layoutRootElement;
    UIElement* m_mouseHoverElement;
	Ref<UIStyle> m_defaultStyle;
};

} // namespace ln

