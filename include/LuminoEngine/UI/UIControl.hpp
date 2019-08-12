#pragma once
#include "UIElement.hpp"

namespace ln {
class UILayoutPanel;

// TODO: TextBox の TextArea みたいに、UIElement の派生ではなくて、内部的なパーツとして利用する ScrollArea みたいなのを作ってみたらどうだろう。
// → 2019/8/12 作業中。いったん ScrollViewer を継承から外した
/*
    Note:

ScrollView の位置づけ
    ・シンプルなコントロールにスクロールバー分のメモリは使いたくないが・・・

Control
    ・Contet だろうが Items だろうが、フォーカスを持ったりタブストップしたり、共通はある。

直近の問題
    ・ListView に addd したい。addChild と addItem どっち使えばいいの？
    ・ListView は removeAllChildren じゃなくて removeAllItems の方がいいよね。
    ・ListView からは removeAllChildren は見えないようにしたい。逆もしかり。

LeafControl, ContentsControl, CollectionControl を分けようぜ、という方針。
Leaf がベースでいいかな。これは普通の UIControl。

*/
class UIControl
	: public UIElement
{
public:
    UIControl();
	void init();
    virtual void onDispose(bool explicitDisposing) override;



    /** コンテンツの横方向の配置方法を設定します。 */
    void setHorizontalContentAlignment(HAlignment value);

    /** コンテンツの横方向の配置方法を取得します。 */
    HAlignment horizontalContentAlignment() const;

    /** コンテンツの縦方向の配置方法を設定します。 */
    void setVerticalContentAlignment(VAlignment value);

    /** コンテンツの縦方向の配置方法を取得します。 */
    VAlignment verticalContentAlignment() const;


	void addElement(UIElement* element);
    void removeElement(UIElement* element);

	void removeAllChildren();


	///**
	//	子要素をレイアウトするための UILayoutPanel を設定します。

	//	デフォルトは nullptr です。
	//*/
	//void setLayoutPanel(UILayoutPanel* panel);

	//UILayoutPanel* layoutPanel() const;

	const List<Ref<UIElement>>& logicalChildren() const { return m_logicalChildren; }

protected:
	///** この要素内の子ビジュアル要素の数を取得します。 */
	//virtual int getVisualChildrenCount() const;

	///** 子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。(Zオーダーやアクティブ状態の考慮は実装側で行うこと) */
	//virtual UIElement* getVisualChild(int index) const;

	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

    virtual void onLayoutPanelChanged(UILayoutPanel* newPanel);

    bool m_enabledDirectChildrenContentAlignment;

    List<Ref<UIElement>> m_logicalChildren;
private:

    //Ref<UILayoutPanel> m_layout;
	//Size m_layoutDesiredSize;	// Layout is state-less
};



} // namespace ln

