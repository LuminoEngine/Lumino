
#pragma once
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/RoutedCommand.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// CommandManager
//=============================================================================

CommandManager::TypeContextList	CommandManager::m_typeContextList;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RoutedCommandTypeContext* CommandManager::RegisterCommand(const TypeInfo& type, RoutedCommand* command)
{
	LN_VERIFY_RETURNV(command != NULL, NULL);

	std::shared_ptr<RoutedCommandTypeContext> ptr;
	if (!m_typeContextList.TryGetValue(&type, &ptr))
	{
		// 見つからなかったので新しく作る
		ptr.reset(LN_NEW RoutedCommandTypeContext());
		ptr->Type = &type;
		m_typeContextList.Add(&type, ptr);
	}

	// command が登録済みでなければ登録する
	if (!command->m_registerd) {
		ptr->RoutedCommandList.Add(command);
		command->m_registerd = true;	// 登録済みにする
	}
	return ptr.get();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CommandManager::CanExecute(UIElement* caller, Command* command, const Variant& parameter)
{
	RoutedCommand* routedCommand = dynamic_cast<RoutedCommand*>(command);
	if (routedCommand != NULL)
	{
		// RoutedEvent として、実行できるコマンドを探す。
		// UIElement::Handler_CanExecuteRoutedCommandEventArgs() が呼ばれて、その中でその UIElement が持っているコマンドを探す。
		RefPtr<CanExecuteRoutedCommandEventArgs> args(caller->GetManager()->GetEventArgsPool()->Create<CanExecuteRoutedCommandEventArgs>(parameter));
		caller->RaiseEvent(UIElement::CanExecuteRoutedCommandEvent, caller, args);
		return args->CanExecute;
	}
	else {
		// ViewModel のユーザーカスタマイズコマンド。普通に呼び出す。
		return command->CanExecute(parameter);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CommandManager::Execute(UIElement* caller, Command* command, const Variant& parameter)
{
	RoutedCommand* routedCommand = dynamic_cast<RoutedCommand*>(command);
	if (routedCommand != NULL)
	{
		// RoutedEvent として、実行できるコマンドを探す。
		// UIElement::Handler_ExecuteRoutedCommandEventArgs() が呼ばれて、その中でその UIElement が持っているコマンドを探す。
		RefPtr<ExecuteRoutedCommandEventArgs> args(caller->GetManager()->GetEventArgsPool()->Create<ExecuteRoutedCommandEventArgs>(parameter));
		caller->RaiseEvent(UIElement::ExecuteRoutedCommandEvent, caller, args);
	}
	else {
		// ViewModel のユーザーカスタマイズコマンド。普通に呼び出す。
		command->Execute(parameter);
	}
}


} // namespace GUI
} // namespace Lumino
