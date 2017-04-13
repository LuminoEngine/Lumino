
#include "Internal.h"
#include <Lumino/UI/UICommands.h>
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIRoutedCommand
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIRoutedCommand, Object)

//------------------------------------------------------------------------------
UIRoutedCommand::UIRoutedCommand()
{
}

//------------------------------------------------------------------------------
UIRoutedCommand::~UIRoutedCommand()
{
}

//------------------------------------------------------------------------------
void UIRoutedCommand::Initialize()
{
}

//==============================================================================
// UICommandBinding
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UICommandBinding, Object)

//------------------------------------------------------------------------------
UICommandBinding::UICommandBinding()
{
}

//------------------------------------------------------------------------------
UICommandBinding::~UICommandBinding()
{
}

//------------------------------------------------------------------------------
void UICommandBinding::Initialize(UIRoutedCommand* command, InputGesture* gesture)
{
	m_command = command;
	m_gesture = gesture;
}

//------------------------------------------------------------------------------
UIRoutedCommand* UICommandBinding::GetCommand() const
{
	return m_command;
}

//------------------------------------------------------------------------------
InputGesture* UICommandBinding::GetGesture() const
{
	return m_gesture;
}

//==============================================================================
// UIApplicationCommands
//==============================================================================
UIRoutedCommand* UIApplicationCommands::Paste = nullptr;

LN_NAMESPACE_END
