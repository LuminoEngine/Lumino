#pragma once
#include "UIContainerElement.hpp"

namespace ln {

/**
 * 要素のフォーカス遷移を管理すためのコンテナ要素。
 *
 * Dialog のようにひとつの Active な要素にのみ、UI イベントを通知するようにしたいときに使う。
 * Dialog と異なるのは、
 * - 関係する要素を常に表示したままにすることができること。
 *   - あくまで「フォーカスの移動」を管理する。Dialog だと遷移にともない winodw の Close が発生するが、こちらはその必要はない。
 * - 履歴を一気にさかのぼれること。
 *  - 例：[メニュー] > [アイテム] > [つかう] と選んだら全てのウィンドウを閉じてゲーム画面に戻る
 * - 履歴を積まないで画面遷移もできること。
 *  - 例：PageUp/PageDown で別メニューへ移動
 * 
 *
 * 基本動作
 * ----------
 * 例えば RPG の装備画面を考えてみる。
 * - 左側にはキャラクターの装備しているウィンドウ
 * - 右側には持ち物一覧ウィンドウ
 * この2つは最初から表示されているので、Navigator の子要素として最初から追加しておく。
 *
 * 最初は gotoFocus() で装備ウィンドウにフォーカスを当てておく。
 * 装備ウィンドウはキーやマウスで操作できるが、持ち物ウィンドウにはイベントが回らなくなるため一切操作はできなくなる。
 *
 * 装備ウィンドウで決定ボタンなどを押したら、pushFocus() で持ち物ウィンドウへのフォーカスをスタックに積む。
 * これで持ち物ウィンドウが操作可能になり、装備ウィンドウは操作できなくなる。
 *
 * 持ち物ウィンドウで決定やキャンセルを押したら、popFocus() で装備ウィンドウにフォーカスを戻すことができる。
 *
 * その他の動作
 * ----------
 * 初期子要素以外の UIElement を goto, push に指定することもできる。
 * その場合その時点で子要素として追加され、フォーカスと操作は管理下に置かれる。
 *
 *
 */
class UIFocusNavigator
    //: public UIContainerElement
    : public Object
{
public:
	void gotoFocus(UIElement* element);
	void pushFocus(UIElement* element);
	void popFocus();
	void clearFocusHistory();

	UIElement* focusedElement() const;

LN_CONSTRUCT_ACCESS:
	UIFocusNavigator();
	bool init();

protected:
	//virtual UIElement* lookupMouseHoverElement(const Point& frameClientPosition) override;

private:
    List<UIElement*> m_history;
};

} // namespace ln

