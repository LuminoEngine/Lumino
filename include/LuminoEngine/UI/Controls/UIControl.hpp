#pragma once
#include "../../Base/Collection.hpp"
#include "../UIElement.hpp"

namespace ln {
class UIActiveTimer;
class UIFrameLayout2;

namespace detail {
class UIAligned3x3GridLayoutArea;
}

/** UIInlinePlacement */
LN_ENUM()
enum class UIInlinePlacement
{
    /** TopLeft */
    TopLeft,

    /** Top */
    Top,

    /** TopRight */
    TopRight,

    /** Left */
    Left,

    /** Center */
    Center,

    /** Right */
    Right,

    /** BottomLeft */
    BottomLeft,

    /** Bottom */
    Bottom,

    /** BottomRight */
    BottomRight,
};


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


    アイコンの標準プロパティ化について
    ----------
    やっぱりやっておきたい。
    必要なのは Content を持つ各種コントロール。
    HeaderdItemsControl
    HeaderdContentContorl なんかもあてはまる。
    子要素の配置領域を UIContentsArea みたいにクラス化しておいて、このクラスは領域の上下左右にアイコンなどビジュアル要素を任意に足せるようにしてみる。
    で、各種 Control はこれをヘルパーとして使う。
    こんな方針でいいかな。
    →アイコンだけでなく、コンテンツ周辺のコントロール追加全体的に使用したい。例えば、テキストボックスの右側にボタン追加したり。

    ### 任意要素の追加を提供するべきか？
    直近ではボタンを追加したい。
    その方法だけど…
    ・google 検索ページのinputには、音声入力ボタンと検索ボタン、2つが横に並んでいる。
    ・NumUpDown は上下に並ぶ２つのボタンで実装される。
    ・パスワードinputとかは、pushbutton ではなくToggleButton を使うこともある。
    ・ボタンクリックでコンテキストメニューを出すようなカスタムボタンを配置したいこともある。プロパティグリッドのドロップダウンとか。
    ・VisualStudio のソリューションエクスプローラ検索欄では、ドロップダウンのテキストボックスの中に検索ボタンがある
    ・ECサイトとか、検索バーの左側にカテゴリのドロップダウン付けたり
    ContainerItem だと、
    ・シーンのアウトラインのツリービューで、アイテムの右側に[表示/非表示]と[編集ロック]ボタンを置いたり。

    正しくやる方法 (ContentControl なら、addChild で Layout と Icon と TextBlock 追加したり、LeafControl なら measureOverride) は
    それはそれでいいんだけど、ただアイコン追加するだけのためにそれをやるのはめんどくさすぎる。

    ただ、setIcon() みたいな関数は公開しないほうがいいと思う。
    Window はタイトルバーへのアイコン設定関数を公開するが、それと混乱する。

    UE4はaddChild で Layout と Icon と TextBlockで作るらしい。
    https://www.reddit.com/r/unrealengine/comments/4f1z8v/how_can_i_make_a_button_with_both_text_and_a_icon/

    検索ボックスの実装。
    UnrealEngine\Engine\Source\Runtime\Slate\Private\Widgets\Input\SSearchBox.cpp
    SEditableTextBox が持っている TSharedPtr<SHorizontalBox> Box; に slot を差し込む形でボタンを配置している。

    …やっぱりvisualは自分でlayoutつかってレイアウトして、ContentPresenter とか使って子要素配置してもらうのが正しいのか・・・。

    JavaFX では、TextField を枠無しにして、ボタンと並べるみたい。
    https://lawrencepremkumar.wordpress.com/2012/09/15/custom-textfield-in-javafx-2-2/
*/
/**
 * 視覚情報の表示やユーザー入力の処理を行う要素の基本クラスです。
 *
 * UIControl は直接の子要素を、UIFrameLayout2 のレイアウト仕様に従ってレイアウトします。
 *
 * Visual States
 * ----------
 *
 * | Name           | Group           | Description    |
 * |----------------|-----------------|----------------|
 * | Normal         | CommonGroup     | Default state. |
 * | MouseOver      | CommonGroup     | Mouse pointer is positioned over the control. |
 * | Disabled       | CommonGroup     | Control is disabled. |
 * | Focused        | FocusGroup      | Control has focus. |
 * | Unfocused      | FocusGroup      | Control does not have focus. |
 * | Visible        | VisibilityGroup | Control is visible. |
 * | Hidden         | VisibilityGroup | Control is hidden. |
 * | Collapsed      | VisibilityGroup | Control is collapsed. |
 *
 */
LN_CLASS()
class UIControl
	: public UIElement
{
	LN_OBJECT;
public:
    UIControl();

    /** init */
    LN_METHOD()
    bool init();
    virtual void onDispose(bool explicitDisposing) override;




    /** コンテンツの横方向の配置方法を設定します。 */
    void setHorizontalContentAlignment(UIHAlignment value);

    /** コンテンツの横方向の配置方法を取得します。 */
    UIHAlignment horizontalContentAlignment() const;

    /** コンテンツの縦方向の配置方法を設定します。 */
    void setVerticalContentAlignment(UIVAlignment value);

    /** コンテンツの縦方向の配置方法を取得します。 */
    UIVAlignment verticalContentAlignment() const;


	void addElement(UIElement* element);
    void removeElement(UIElement* element);

	void removeAllChildren();

    // 要素の外側に張り付くものに利用する。
    // アイコンボタンを作るとき、外側ではなく文字側に張り付くようなものは、addContent で、Icon と TextBlock を並べた BoxLayout を追加する。
    /** addInlineVisual */
    LN_METHOD()
    void addInlineVisual(UIElement* element, UIInlinePlacement layout);

	const Ref<Collection<Ref<UIElement>>>& logicalChildren() const { return m_logicalChildren; }

	// TODO: UIElement に持たせたい気持ち。TextArea でも使いたい
	void registerActiveTimer(UIActiveTimer* timer);
	void unregisterActiveTimer(UIActiveTimer* timer);

    void addAction(UIAction* action);


    /** Activated イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
    Ref<EventConnection> connectOnActivated(Ref<UIEventHandler> handler);

    /** Unchecked イベントの通知を受け取るコールバックを登録します。*/
    //LN_METHOD(Event)
    Ref<EventConnection> connectOnDeactivated(Ref<UIEventHandler> handler);

    // UIComboBoxItem は UIComboBox(UIControl) の子としてレイアウトしてほしくないので、その回避に用意したもの。
    bool m_autoLayoutLogicalChildren = true;


protected:
    virtual void onActivated();
    virtual void onDeactivated();
    virtual void onLogicalChildRemoved(UIElement* removedElement);

	///** この要素内の子ビジュアル要素の数を取得します。 */
	//virtual int getVisualChildrenCount() const;

	///** 子ビジュアル要素を取得します。奥にある要素が先、手前にある要素が後になります。(Zオーダーやアクティブ状態の考慮は実装側で行うこと) */
	//virtual UIElement* getVisualChild(int index) const;

    virtual void onAddChild(UIElement* child) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

    bool m_enabledDirectChildrenContentAlignment;

    //List<Ref<UIElement>> m_logicalChildren;
	Ref<Collection<Ref<UIElement>>> m_logicalChildren;

    Ref<detail::UIAligned3x3GridLayoutArea> m_aligned3x3GridLayoutArea;
    List<Ref<UIElement>> m_inlineElements;

    void removeAllLogicalChildren();
    void addLogicalChild(UIElement* element);

private:
    void activateInternal();
    void deactivateInternal();
    

	List<Ref<UIActiveTimer>> m_activeTimers;
    Ref<List<Ref<UIAction>>> m_actions;

    Event<UIEventHandler> m_onActivated;
    Event<UIEventHandler> m_onDeactivated;

    friend class detail::UIManager;
    friend class UIFrameLayout2;
};


namespace detail {

class UIAligned3x3GridLayoutArea
    : public Object
{
public:
    Size measure(UILayoutContext* layoutContext, const List<Ref<UIElement>>& inlineElements, const Size& constraint, const Size& contentDesiredSize);
    void arrange(UILayoutContext* layoutContext, const List<Ref<UIElement>>& inlineElements, const Rect& finalArea, Rect* outActualContentRect);

LN_CONSTRUCT_ACCESS:
    UIAligned3x3GridLayoutArea();
    void init();

private:
    void getGridInfoHelper(UIElement* element, int* row, int* column, int* rowSpan, int* columnSpan) const;

    struct LineInfo
    {
        // このセルの右辺または下辺の座標 = 次のセルの左辺または上辺の座標。
        // このセルの右または下のラインの座標と考える。
        //float desiredLastOffset = 0.0f;

        float desiredSize = 0.0f;
        float actualOffset = 0.0f;
        float actualSize = 0.0f;
    };

    std::array<LineInfo, 3> m_rows;
    std::array<LineInfo, 3> m_columns;
};

} // namespace detail


} // namespace ln

