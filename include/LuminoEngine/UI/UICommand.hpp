#pragma once
#include "UIEvents.hpp"

namespace ln {

/**	

   

*/
//  メニューやボタンの実行アクションの「名前」のようなもの。実行本体は UIAction
class UICommand
	: public Object
{
public:
    EventConnection connectOnCanExecuteChanged(UIEventHandler handler);

protected:
    virtual void onCanExecuteChanged();

LN_CONSTRUCT_ACCESS:
    UICommand();
	virtual ~UICommand() = default;
	void init();

private:
    Event<UIEventHandler> m_onCanExecuteChanged;
};

class UIAction
    : public Object
{
public:
    UICommand* command() const { return m_command; }
    EventConnection connectOnCanExecute(UICommandEventHandler handler);
    EventConnection connectOnExecute(UICommandEventHandler handler);

    bool canExecute();
    void execute();

protected:
    virtual void onCanExecute(bool* canExecute);
    virtual void onExecute();

LN_CONSTRUCT_ACCESS:
    UIAction();
    virtual ~UIAction() = default;
    void init(UICommand* command, UICommandEventHandler onExecute);

private:
    Ref<UICommand> m_command;
    Event<UICommandEventHandler> m_onCanExecuteEvent;
    Event<UICommandEventHandler> m_onExecuteEvent;
};

namespace detail {
struct UICommandInternal
{
    static bool handleCommandRoutedEvent(UIEventArgs* e, const Ref<List<Ref<UIAction>>>& actions);
};
} // namespace detail
} // namespace ln

