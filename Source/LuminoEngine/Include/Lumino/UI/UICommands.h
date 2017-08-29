
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIRoutedCommand
	: public Object
{
	LN_OBJECT;
public:
	static Ref<UIRoutedCommand> create();

LN_CONSTRUCT_ACCESS:
	UIRoutedCommand();
	virtual ~UIRoutedCommand();
	bool initialize();

private:
};

class UIApplicationCommands
{
public:
	static UIRoutedCommand* Paste;
};

LN_NAMESPACE_END
