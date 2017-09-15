
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

	bool isPressed(const StringRef& bindingName) const;
	bool isTriggered(const StringRef& bindingName) const;
	bool isOffTriggered(const StringRef& bindingName) const;
	bool isRepeated(const StringRef& bindingName) const;
	float getAxisValue(const StringRef& bindingName) const;

	void addBinding(const StringRef& buttonName, InputGesture* gesture);
	void removeBinding(InputGesture* gesture);
	void clearBindings(const StringRef& buttonName);
	void clearAllBindings();
	void setRepeatInterval(int start, int step);

	void updateFrame();

	// TODO:
	int getJoyNumber() const;

private:
	struct BindingSlot
	{
		String					name;
		Ref<InputGesture>	binding;
	};

	struct InputState
	{
		float	current;
		int		state;
		int		ref;
	};

	void UpdateOneInputState(InputState* state);
	const InputState* LockupState(const StringRef& bindingName) const;

	using StatusMap = SortedArray<String, InputState>;

	detail::InputManager*			m_manager;
	uint32_t						m_attachedDevices;		// TODO: 本当ならちゃんとインターフェイス組むべきかも
	List<BindingSlot>				m_bindingSlots;
	StatusMap						m_inputStatus;
	InputState						m_inputStateForAny;
	int								m_repeatIntervalStart;
	int								m_repeatIntervalStep;
};

LN_NAMESPACE_END
