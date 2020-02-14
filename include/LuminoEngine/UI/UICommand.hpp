#pragma once
#include "../Input/InputBinding.hpp"
#include "UIEvents.hpp"

namespace ln {




/**	

   

*/
//  メニューやボタンの実行アクションの「名前」のようなもの。実行本体は UIAction
// WPF の RoutedCommand 相当だが、実行ロジックは持たない。
class UICommand
	: public Object
{
public:
    void addInputGesture(InputGesture* value);

	Ref<EventConnection> connectOnCanExecuteChanged(Ref<UIEventHandler> handler);

    bool testInputEvent(UIEventArgs* e);

protected:
    virtual void onCanExecuteChanged();

LN_CONSTRUCT_ACCESS:
    UICommand();
	virtual ~UICommand() = default;
	void init();

private:
    List<Ref<InputGesture>> m_inputGestures;
    Event<UIEventHandler> m_onCanExecuteChanged;
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

