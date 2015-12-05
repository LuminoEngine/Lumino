
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief
*/
class InputGesture	// TODO: Input ‚ÉˆÚ“®‚·‚é‚ÆŽv‚¤
	: public RefObject
{
public:
	InputGesture();
	virtual ~InputGesture();
};

/**
	@brief
*/
class KeyGesture
	: public InputGesture
{
public:
	KeyGesture(Key key);
	KeyGesture(Key key, ModifierKeys modifierKeys);
	virtual ~KeyGesture();
};

/**
	@brief
*/
class UIInputBinding
	: public RefObject
{
public:
	UIInputBinding(Command* command, InputGesture* gesture);
	virtual ~UIInputBinding();
	
public:
	Command*		m_command;
	InputGesture*	m_gesture;
};

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
