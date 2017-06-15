
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIRoutedCommand
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static RefPtr<UIRoutedCommand> create();

LN_CONSTRUCT_ACCESS:
	UIRoutedCommand();
	virtual ~UIRoutedCommand();
	void initialize();

private:
};

class UIApplicationCommands
{
public:
	static UIRoutedCommand* Paste;
};

LN_NAMESPACE_END
