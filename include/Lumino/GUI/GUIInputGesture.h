
#pragma once
#include "Common.h"

namespace Lumino
{
namespace GUI
{

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
class InputBinding
	: public RefObject
{
public:
	InputBinding(Command* command, InputGesture* gesture);
	virtual ~InputBinding();
	
public:
	Command*		m_command;
	InputGesture*	m_gesture;
};

} // namespace GUI
} // namespace Lumino
