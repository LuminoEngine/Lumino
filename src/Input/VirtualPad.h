
#pragma once
#include <Lumino/Input/Common.h>
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN
class InputDriver;

class VirtualPad
	: public RefObject
{
public:
	VirtualPad(detail::InputManager* manager);
	virtual ~VirtualPad();

	void AttachBinding(InputBinding* binding);
	void DetachBinding(InputBinding* binding);

	void UpdateFrame();

private:
	struct InputState
	{
		float	current;
		int		state;
	};

	detail::InputManager*		m_manager;
	Array<RefPtr<InputBinding>>	m_bindings;
	SortedArray<String, InputState>	m_inputStatus;
};

LN_NAMESPACE_END
