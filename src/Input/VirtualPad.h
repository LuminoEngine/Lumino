
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

	bool IsPress(const StringRef& bindingName) const;
	bool IsOnTrigger(const StringRef& bindingName) const;
	bool IsOffTrigger(const StringRef& bindingName) const;
	bool IsRepeat(const StringRef& bindingName) const;
	float GetAxisValue(const StringRef& bindingName) const;

	void AttachBinding(InputBinding* binding);
	void DetachBinding(InputBinding* binding);
	void SetRepeatInterval(int start, int step);

	void UpdateFrame();

private:
	struct InputState
	{
		float	current;
		int		state;
		int		ref;
	};

	detail::InputManager*			m_manager;
	Array<RefPtr<InputBinding>>		m_bindings;
	SortedArray<String, InputState>	m_inputStatus;
	int								m_repeatIntervalStart;
	int								m_repeatIntervalStep;
};

LN_NAMESPACE_END
