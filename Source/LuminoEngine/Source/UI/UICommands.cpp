
#include "Internal.h"
#include <Lumino/UI/UICommands.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIRoutedCommand
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIRoutedCommand, Object)

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
// UIApplicationCommands
//==============================================================================
UIRoutedCommand* UIApplicationCommands::Paste = nullptr;

LN_NAMESPACE_END
