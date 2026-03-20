#pragma once
#include "../Input/InputBinding.hpp"
#include "UIEvents.hpp"

namespace ln {




/**	

   

*/
// メニューやボタンの実行アクションの「名前」のようなもの。
// エディタ用だと、"New", "Save", "Open" など。 実行本体は UIAction。
// ゲーム内UI用だと、"left", "right", "submit", "page down" など。(InputBinding 相当)
// WPF の RoutedCommand 相当だが、実行ロジックは持たない。
// 動作としては、Application::onRoutedEvent() で入力をチェックし、一致する UICommand があれば focusdElement へ raise する。
class UICommand
	: public Object
{
public:
    void addInputGesture(InputGesture* value);

	Ref<EventConnection> connectOnCanExecuteChanged(Ref<UIGeneralEventHandler> handler);

    bool testInputEvent(UIEventArgs* e);

protected:
    virtual void onCanExecuteChanged();

LN_CONSTRUCT_ACCESS:
    UICommand();
    Result<> init();
    Result<> init(const String& name);

private:
    String m_name;
    List<Ref<InputGesture>> m_inputGestures;
    Event<UIGeneralEventHandler> m_onCanExecuteChanged;
};

class UICommonInputCommands
{
public:
    /** 左方向操作 */
    static const UICommand* left();

    /** 右方向操作 */
    static const UICommand* right();

    /** 上方向操作 */
    static const UICommand* up();

    /** 下方向操作 */
    static const UICommand* down();

    /** 決定操作 */
    static const UICommand* submit();

    /** キャンセル操作 */
    static const UICommand* cancel();

    /** メニュー操作 (項目のヘルプ表示などを想定) */
    static const UICommand* menu();

    /** 補助操作 (複数選択やお気に入りマークなどを想定) */
    static const UICommand* shift();

    /** 前ページの表示操作 */
    static const UICommand* pageUp();

    /** 次ページの表示操作 */
    static const UICommand* pageDown();

};


// Note: WPF だと CommandBinding 相当。
class UIAction
    : public Object
{
public:
    UICommand* command() const { return m_command; }
	Ref<EventConnection> connectOnCanExecute(Ref<UICommandEventHandler> handler);
	Ref<EventConnection> connectOnExecute(Ref<UICommandEventHandler> handler);

    bool canExecute();
    void execute();

protected:
    virtual void onCanExecute(bool* canExecute);
    virtual void onExecute();

LN_CONSTRUCT_ACCESS:
    UIAction();
    virtual ~UIAction() = default;
    void init(UICommand* command, Ref<UICommandEventHandler> onExecute);

private:
    Ref<UICommand> m_command;
    Event<UICommandEventHandler> m_onCanExecuteEvent;
    Event<UICommandEventHandler> m_onExecuteEvent;
};

namespace detail {
struct UICommandInternal
{
    static bool handleCommandRoutedEvent(UIEventArgs* e, const List<Ref<UIAction>>& actions);
};
} // namespace detail
} // namespace ln

