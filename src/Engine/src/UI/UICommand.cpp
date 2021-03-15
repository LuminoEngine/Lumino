
#include "Internal.hpp"
#include <LuminoEngine/UI/UICommand.hpp>
#include "UIManager.hpp"

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

bool UICommand::init()
{
    return Object::init();
}

bool UICommand::init(const String& name)
{
    if (!init()) return false;
    m_name = name;
    return true;
}

void UICommand::addInputGesture(InputGesture* value)
{
    if (value->getType() != detail::InputBindingType::Keyboard) {
        LN_NOTIMPLEMENTED();
    }

    m_inputGestures.add(value);
}

Ref<EventConnection> UICommand::connectOnCanExecuteChanged(Ref<UIGeneralEventHandler> handler)
{
    return m_onCanExecuteChanged.connect(handler);
}

void UICommand::onCanExecuteChanged()
{
    auto args = UIEventArgs::create(nullptr, UIEvents::CanExecuteChangedEvent);
    m_onCanExecuteChanged.raise(args);
}

bool UICommand::testInputEvent(UIEventArgs* e)
{
    //if (e->type() == UIEvents::MouseDownEvent) {
    //    auto* me = static_cast<UIMouseEventArgs*>(e);
    //    for (auto& gesture : m_inputGestures) {
    //        if (gesture->getType() == detail::InputBindingType::Mouse) {
    //            auto* mg = static_cast<MouseGesture*>(gesture.get());

    //        }
    //    }
    //}
    //if (e->type() == UIEvents::KeyDownEvent) {
    //    auto* ke = static_cast<UIKeyEventArgs*>(e);
    //    for (auto& gesture : m_inputGestures) {
    //        if (gesture->getType() == detail::InputBindingType::Keyboard) {
    //            auto* mg = static_cast<KeyGesture*>(gesture.get());
    //            if (ke->getKey() == mg->getKey() && ke->getModifierKeys() == mg->getModifierKeys()) {
    //                return true;
    //            }
    //        }
    //    }
    //}

    for (const auto& gesture : m_inputGestures) {
        if (UIEventArgs::testInputGesture(e, gesture)) {
            return true;
        }
    }

    return false;
}

//==============================================================================
// UICommonInputCommands

const UICommand* UICommonInputCommands::left()
{
    return detail::EngineDomain::uiManager()->commonInputCommands().left;
}

const UICommand* UICommonInputCommands::right()
{
    return detail::EngineDomain::uiManager()->commonInputCommands().right;
}

const UICommand* UICommonInputCommands::up()
{
    return detail::EngineDomain::uiManager()->commonInputCommands().up;
}

const UICommand* UICommonInputCommands::down()
{
    return  detail::EngineDomain::uiManager()->commonInputCommands().down;
}

const UICommand* UICommonInputCommands::submit()
{
    return detail::EngineDomain::uiManager()->commonInputCommands().submit;
}

const UICommand* UICommonInputCommands::cancel()
{
    return detail::EngineDomain::uiManager()->commonInputCommands().cancel;
}

const UICommand* UICommonInputCommands::menu()
{
    return  detail::EngineDomain::uiManager()->commonInputCommands().menu;
}

const UICommand* UICommonInputCommands::shift()
{
    return detail::EngineDomain::uiManager()->commonInputCommands().shift;
}

const UICommand* UICommonInputCommands::pageUp()
{
    return detail::EngineDomain::uiManager()->commonInputCommands().pageUp;
}

const UICommand* UICommonInputCommands::pageDown()
{
    return detail::EngineDomain::uiManager()->commonInputCommands().pageDown;
}


//==============================================================================
// UIAction

UIAction::UIAction()
{
}

void UIAction::init(UICommand* command, Ref<UICommandEventHandler> onExecute)
{
    Object::init();
    m_command = command;
    if (onExecute) {
		connectOnExecute(onExecute);
    }
}

Ref<EventConnection> UIAction::connectOnCanExecute(Ref<UICommandEventHandler> handler)
{
    return m_onCanExecuteEvent.connect(handler);
}

Ref<EventConnection> UIAction::connectOnExecute(Ref<UICommandEventHandler> handler)
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

bool detail::UICommandInternal::handleCommandRoutedEvent(UIEventArgs* e, const List<Ref<UIAction>>& actions)
{
    if (e->type() == UIEvents::CanExecuteCommandEvent ||
        e->type() == UIEvents::ExecuteCommandEvent) {
        auto* ce = static_cast<UICommandEventArgs*>(e);
        auto action = actions.findIf([&](auto& x) { return x->command() == ce->command(); });
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

    return false;
}

} // namespace ln

