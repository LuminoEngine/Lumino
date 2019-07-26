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
    EventConnection connectOnCanExecute(UICommandEventHandler handler);
    EventConnection connectOnExecute(UICommandEventHandler handler);

protected:
    virtual void onCanExecute(bool* canExecute);
    virtual void onExecute();

LN_CONSTRUCT_ACCESS:
    UIAction();
    virtual ~UIAction() = default;
    void init(UICommand* command, UICommandEventHandler onExecute);

private:
    Ref<Command> m_command;
    Event<UICommandEventHandler> m_onCanExecuteEvent;
    Event<UICommandEventHandler> m_onExecuteEvent;
};

} // namespace ln

