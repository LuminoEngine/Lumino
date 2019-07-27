
#include "Internal.hpp"
#include <LuminoEngine/UI/UICommand.hpp>

namespace ln {

//==============================================================================
// UICommand
// https://docs.microsoft.com/en-us/dotnet/api/system.windows.input.routedUIAction?view=netframework-4.8
/*
    s_oepnCommand = UIAction::create(u"Open", tr("開く"));
    ...
    auto menuItem = UIMenuItem::create();
    menuItem->setText(tr("開く (_O)"));
    menuItem->setCommand(s_openCommand);
    ...
    auto button = UIButton::create();
    button->setCommand(s_openCommand);
    ...
    何かModel側で操作発生して、メニューをグレーアウト更新とかしたい場合は s_oepnCommand->notifyCanExecuteChanged(); する。
    こうすると、s_oepnCommand を購読しているコントロールにイベントが流れて、グレー状態が更新される。（CanExecute が再評価される）
*/

UICommand::UICommand()
{
}

void UICommand::init()
{
    Object::init();
}

EventConnection UICommand::connectOnCanExecuteChanged(UIEventHandler handler)
{
    return m_onCanExecuteChanged.connect(handler);
}

void UICommand::onCanExecuteChanged()
{
    auto args = UIEventArgs::create(nullptr, UIEvents::CanExecuteChangedEvent);
    m_onCanExecuteChanged.raise(args);
}


//==============================================================================
// UIAction

UIAction::UIAction()
{
}

void UIAction::init(UICommand* command, UICommandEventHandler onExecute)
{
    Object::init();
    m_command = command;
    if (onExecute) {
        connectOnExecute(onExecute);
    }
}

EventConnection UIAction::connectOnCanExecute(UICommandEventHandler handler)
{
    return m_onCanExecuteEvent.connect(handler);
}

EventConnection UIAction::connectOnExecute(UICommandEventHandler handler)
{
    return m_onExecuteEvent.connect(handler);
}

bool UIAction::canExecute()
{
    bool r = false;
    onCanExecute(&r);
    return r;
}

void UIAction::execute()
{
    onExecute();
}

void UIAction::onCanExecute(bool* canExecute)
{
    assert(canExecute);
    auto args = UICommandEventArgs::create(nullptr, UIEvents::CanExecuteCommandEvent, m_command);
    m_onCanExecuteEvent.raise(args);
    *canExecute = args->canExecute();
}

void UIAction::onExecute()
{
    auto args = UICommandEventArgs::create(nullptr, UIEvents::ExecuteCommandEvent, m_command);
    m_onExecuteEvent.raise(args);
}


//==============================================================================
// UICommandInternal

bool detail::UICommandInternal::handleCommandRoutedEvent(UIEventArgs* e, const Ref<List<Ref<UIAction>>>& actions)
{
    if (actions)
    {
        if (e->type() == UIEvents::CanExecuteCommandEvent ||
            e->type() == UIEvents::ExecuteCommandEvent) {
            auto* ce = static_cast<UICommandEventArgs*>(e);
            auto action = actions->findIf([&](auto& x) { return x->command() == ce->command(); });
            if (action) {
                if ((*action)->canExecute()) {
                    ce->setCanExecute(true);
                    if (e->type() == UIEvents::ExecuteCommandEvent) {
                        (*action)->execute();
                    }
                }
                e->handled = true;
                return true;
            }
        }
    }
    return false;
}

} // namespace ln

