/*
	[2016/10/2]
		BindingSlot は、InputGesture は UE4 に合わせて名前を持たせないようにしたため、
		入力ソースとの対応付けを行うために必要。
*/
#include "../Internal.h"
#include <Lumino/Input/InputController.h>
#include "InputManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// InputController
//==============================================================================

//------------------------------------------------------------------------------
InputController* InputController::GetController(int index)
{
	return detail::InputManager::getInstance()->getVirtualPad(index);
}

//------------------------------------------------------------------------------
InputController::InputController(detail::InputManager* manager)
	: m_manager(manager)
	, m_attachedDevices(detail::InputDeviceID_Keyboard | detail::InputDeviceID_Mouse | detail::InputDeviceID_Joystick0)	// TODO
	, m_bindingSlots()
	, m_inputStatus()
	, m_inputStateForAny()
	, m_repeatIntervalStart(20)	// TODO 要調整。時間の方がいいかも？
	, m_repeatIntervalStep(5)
{
	m_inputStateForAny.current = 0;
	m_inputStateForAny.state = 0;
	m_inputStateForAny.ref = 0;
}

//------------------------------------------------------------------------------
InputController::~InputController()
{
}

//------------------------------------------------------------------------------
bool InputController::isPressed(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	return (state->state > 0);
}

//------------------------------------------------------------------------------
bool InputController::isTriggered(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	return (state->state == 1);
}

//------------------------------------------------------------------------------
bool InputController::isOffTriggered(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	return (state->state == -1);
}

//------------------------------------------------------------------------------
bool InputController::isRepeated(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	int s = state->state;
	return ((s == 1) || (s > m_repeatIntervalStart && (s % m_repeatIntervalStep) == 0));
}

//------------------------------------------------------------------------------
float InputController::getAxisValue(const StringRef& bindingName) const
{
	auto* state = m_inputStatus.find(bindingName);
	LN_THROW(state != nullptr, KeyNotFoundException);
	return state->current;
}

//------------------------------------------------------------------------------
void InputController::addBinding(const StringRef& buttonName, InputGesture* gesture)
{
	BindingSlot slot = { buttonName, gesture };
	m_bindingSlots.add(slot);

	// まだ登録したことがない名前であれば InputState を作る。そうでなければ参照カウントを増やす。
	auto* state = m_inputStatus.find(buttonName);
	if (state == nullptr)
	{
		InputState newState;
		newState.current = 0;
		newState.state = 0;
		newState.ref = 1;
		m_inputStatus.add(buttonName, newState);
	}
	else
	{
		state->ref++;
	}
}

//------------------------------------------------------------------------------
void InputController::removeBinding(InputGesture* gesture)
{
	int index = m_bindingSlots.indexOf([gesture](const BindingSlot& slot) { return slot.binding == gesture; });
	if (index < 0) return;

	const String& name = m_bindingSlots[index].name;

	auto* state = m_inputStatus.find(name);
	state->ref--;
	if (state->ref <= 0) {
		m_inputStatus.remove(name);
	}

	m_bindingSlots.removeAt(index);
}

//------------------------------------------------------------------------------
void InputController::clearBindings()
{
	List<BindingSlot> list = m_bindingSlots;
	for (int i = list.getCount() - 1; i >= 0; ++i)	// 後ろから回した方がちょっと削除の効率がいい
	{
		removeBinding(list[i].binding);
	}
}

//------------------------------------------------------------------------------
void InputController::setRepeatInterval(int start, int step)
{
	m_repeatIntervalStart = start;
	m_repeatIntervalStep = step;
}

//------------------------------------------------------------------------------
void InputController::updateFrame()
{
	//SortedArray<String, InputState>::iterator itr = m_inputStatus.begin();
	//SortedArray<String, InputState>::iterator end = m_inputStatus.end();
	//for (; itr != end; ++itr) {
	//	(*itr).second.current = 0;
	//}
	for (auto& state : m_inputStatus) {
		state.second.current = 0;
	}
	m_inputStateForAny.current = 0;

	// m_inputStatus に現在の入力値を展開する
	for (const BindingSlot& slot : m_bindingSlots)
	{
		InputGesture* binding = slot.binding;

		float v = m_manager->getVirtualButtonState(
			binding,
			(m_attachedDevices & detail::InputDeviceID_Keyboard) != 0,
			(m_attachedDevices & detail::InputDeviceID_Mouse) != 0,
			getJoyNumber());
		InputState* state = m_inputStatus.find(slot.name);
		if (state != nullptr)
		{
			v *= binding->getScale();
			if (v >= binding->GetMinValidMThreshold()) {
				state->current = std::max(state->current, v);	// Binding が重複したとか、とりあえず大きい方を使う
			}
		}
	}

	// 現在の入力値から状態を遷移させる
	for (auto& state : m_inputStatus)
	{
		UpdateOneInputState(&state.second);

		if (state.second.current > 0.0f)
		{
			m_inputStateForAny.current = 1.0f;
		}
	}
	UpdateOneInputState(&m_inputStateForAny);
}

//------------------------------------------------------------------------------
int InputController::getJoyNumber() const
{
	if (m_attachedDevices & detail::InputDeviceID_Joystick0) return 0;
	if (m_attachedDevices & detail::InputDeviceID_Joystick1) return 1;
	if (m_attachedDevices & detail::InputDeviceID_Joystick2) return 2;
	if (m_attachedDevices & detail::InputDeviceID_Joystick3) return 3;
	return -1;
}

//------------------------------------------------------------------------------
void InputController::UpdateOneInputState(InputState* state)
{
	if (state->current > 0.0f) {
		state->state++;	// 押されてる間は毎フレームインクリメントする
	}
	else
	{
		if (state->state > 0.0f) {
			state->state = -1;	// 離された瞬間のフレーム
		}
		else {
			state->state = 0;		// 離された瞬間の次のフレーム
		}
	}
}

//------------------------------------------------------------------------------
const InputController::InputState* InputController::LockupState(const StringRef& bindingName) const
{
	if (bindingName.isEmpty())
	{
		return &m_inputStateForAny;
	}
	return m_inputStatus.find(bindingName);
}

LN_NAMESPACE_END
