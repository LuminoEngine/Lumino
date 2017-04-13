
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN
class InputGesture;

/**
	@brief		
*/
class UIRoutedCommand
	: public Object
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static RefPtr<UIRoutedCommand> Create();

LN_CONSTRUCT_ACCESS:
	UIRoutedCommand();
	virtual ~UIRoutedCommand();
	void Initialize();

private:
};

/**
	@brief		
*/
class UICommandBinding
	: public Object
{
	LN_UI_TYPEINFO_DECLARE();
public:
	//static RefPtr<InputBinding> Create();

	UIRoutedCommand* GetCommand() const;
	InputGesture* GetGesture() const;

LN_CONSTRUCT_ACCESS:
	UICommandBinding();
	virtual ~UICommandBinding();
	void Initialize(UIRoutedCommand* command, InputGesture* gesture);

private:
	RefPtr<UIRoutedCommand>	m_command;
	RefPtr<InputGesture>	m_gesture;
};

class UIApplicationCommands
{
public:
	static UIRoutedCommand* Paste;
};

LN_NAMESPACE_END
