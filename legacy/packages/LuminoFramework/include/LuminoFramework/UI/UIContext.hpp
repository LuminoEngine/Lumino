#pragma once
#include "UILayoutElement.hpp"	// TODO: for StyleData

namespace ln {
class UIElement;
class UIStyle;
class UIStyleContext;
class UIFrameWindow;

#if 0
// [2019/10/6] アプリ内でどのような文脈でUIシステムを使うか、その単位。
// 例えば、ゲーム全体で共通のメニュー (プレイヤーのステータスなど) と、ゲーム内オブジェクトの UI (シーンの中に PC のようなオブジェクトがあり、のぞき込むとレトロなUIだしたり) の区別。
// 内部的によく使うのは、Editor の UI と、Game プレビュー内の UI.
// 
// 考え中だけど、特徴的な動作としては、Bubble な RoutedEvent は、UIContext の外側には出ない。
// 1つの UIContext を複数の UIシーン (VisualTree) が共有するのは多分やめたほうがいい。
class UIContext
	: public Object
{
public:

public: // TODO: internal
    void setLayoutRootElement(UIElement* element);	// 通常は MainWindow。もし１つのウィンドウ内に複数の Context を含める場合は
    void addElement(UIElement* element);
	//const Ref<UIStyle>& defaultStyle() const { return m_defaultStyle; }
    const Ref<UIStyleContext>& styleContext() const { return m_styleContext; }
    const Ref<detail::UIStyleInstance>& finalDefaultStyle() const { return m_finalDefaultStyle; }

	// 各更新の起点。UIContext には複数の UIFrameWindow を追加することもできるので、UIFrameWindow をツリー更新のルートとするのはちょっと違う。
	// GameEngine としてなら、メインループから。IDE としてなら、メッセージループから呼び出してもらうのが自然だろう。
	// ひとまず、UIContext の作成元となっている Manager から呼び出してもらうことにする。
	void updateStyleTree();
	//void updateLayoutTree();


LN_CONSTRUCT_ACCESS:
    UIContext();
    virtual ~UIContext() = default;
	void init();

private:
    void setupDefaultStyle();

    Ref<UIElement> m_layoutRootElement;
	//Ref<UIStyle> m_defaultStyle;
    Ref<UIStyleContext> m_styleContext;
	Ref<detail::UIStyleInstance> m_finalDefaultStyle;
};
#endif

} // namespace ln

