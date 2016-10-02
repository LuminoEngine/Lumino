
#pragma once
#include <Lumino/Input/Common.h>
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN

class InputController
	: public RefObject
{
public:
	static InputController* GetController(int index);

public:
	InputController(detail::InputManager* manager);
	virtual ~InputController();

	bool IsPressed(const StringRef& bindingName) const;
	bool IsTriggered(const StringRef& bindingName) const;
	bool IsOffTriggered(const StringRef& bindingName) const;
	bool IsRepeated(const StringRef& bindingName) const;
	float GetAxisValue(const StringRef& bindingName) const;

	void AddBinding(const StringRef& buttonName, InputBinding* binding);
	void RemoveBinding(InputBinding* binding);
	void ClearBindings();
	void SetRepeatInterval(int start, int step);

	void UpdateFrame();

	// TODO:
	int GetJoyNumber() const;

private:
	struct BindingSlot
	{
		String					name;
		RefPtr<InputBinding>	binding;
	};

	struct InputState
	{
		float	current;
		int		state;
		int		ref;
	};

	void UpdateOneInputState(InputState* state);
	const InputState* LockupState(const StringRef& bindingName) const;

	detail::InputManager*			m_manager;
	uint32_t						m_attachedDevices;		// TODO: 本当ならちゃんとインターフェイス組むべきかも
	Array<BindingSlot>				m_bindingSlots;
	SortedArray<String, InputState>	m_inputStatus;
	InputState						m_inputStateForAny;
	int								m_repeatIntervalStart;
	int								m_repeatIntervalStep;
};

LN_NAMESPACE_END
