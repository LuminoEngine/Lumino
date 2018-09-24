/*
	[2016/10/2]
		BindingSlot は、InputGesture は UE4 に合わせて名前を持たせないようにしたため、
		入力ソースとの対応付けを行うために必要。
*/
#include "../Internal.hpp"
#include <Lumino/Input/InputController.hpp>
#include "InputManager.hpp"

namespace ln {

//==============================================================================
// InputController
//==============================================================================

//------------------------------------------------------------------------------
InputController* InputController::GetController(int index)
{
	return detail::EngineDomain::inputManager()->getVirtualPad(index);
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
	if (LN_REQUIRE_KEY(state != nullptr)) return false;
	return (state->state > 0);
}

//------------------------------------------------------------------------------
bool InputController::isTriggered(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	if (LN_REQUIRE_KEY(state != nullptr)) return false;
	return (state->state == 1);
}

//------------------------------------------------------------------------------
bool InputController::isOffTriggered(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	if (LN_REQUIRE_KEY(state != nullptr)) return false;
	return (state->state == -1);
}

//------------------------------------------------------------------------------
bool InputController::isRepeated(const StringRef& bindingName) const
{
	auto* state = LockupState(bindingName);
	if (LN_REQUIRE_KEY(state != nullptr)) return false;
	int s = state->state;
	return ((s == 1) || (s > m_repeatIntervalStart && (s % m_repeatIntervalStep) == 0));
}

//------------------------------------------------------------------------------
float InputController::getAxisValue(const StringRef& bindingName) const
{
	auto state = m_inputStatus.findIf([&](auto& item) { return item.first == bindingName; });
	if (LN_REQUIRE_KEY(state.hasValue())) return 0.0f;
	return state->second.current;
}

//------------------------------------------------------------------------------
void InputController::addBinding(const StringRef& buttonName, InputGesture* gesture)
{
	BindingSlot slot = { buttonName, gesture };
	m_bindingSlots.add(slot);

	// まだ登録したことがない名前であれば InputState を作る。そうでなければ参照カウントを増やす。
	auto state = m_inputStatus.findIf([&](auto& item) { return item.first == buttonName; });
	if (state.hasValue())
	{
		state->second.ref++;
	}
	else
	{
		InputState newState;
		newState.current = 0;
		newState.state = 0;
		newState.ref = 1;
		m_inputStatus.add(std::pair<String, InputState>(buttonName, newState));
	}
}

//------------------------------------------------------------------------------
void InputController::removeBinding(InputGesture* gesture)
{
	int index = m_bindingSlots.indexOfIf([gesture](const BindingSlot& slot) { return slot.binding == gesture; });
	if (index < 0) return;

	const String& name = m_bindingSlots[index].name;

	auto state = m_inputStatus.findIf([&](auto& item) { return item.first == name; });
	state->second.ref--;
	if (state->second.ref <= 0) {
		m_inputStatus.removeIf([&](auto& item) { return item.first == name; });
	}

	m_bindingSlots.removeAt(index);
}

//------------------------------------------------------------------------------
void InputController::clearBindings(const StringRef& buttonName)
{
	List<BindingSlot> list = m_bindingSlots;
	for (int i = list.size() - 1; i >= 0; --i)	// 後ろから回した方がちょっと削除の効率がいい
	{
		if (list[i].name == buttonName)
		{
			removeBinding(list[i].binding);
		}
	}
}

//------------------------------------------------------------------------------
void InputController::clearAllBindings()
{
	List<BindingSlot> list = m_bindingSlots;
	for (int i = list.size() - 1; i >= 0; --i)	// 後ろから回した方がちょっと削除の効率がいい
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

	// m_inputStatus の current に現在の入力値を展開する
	for (const BindingSlot& slot : m_bindingSlots)
	{
		InputGesture* binding = slot.binding;

		float v = m_manager->getVirtualButtonState(
			binding,
			(m_attachedDevices & detail::InputDeviceID_Keyboard) != 0,
			(m_attachedDevices & detail::InputDeviceID_Mouse) != 0,
			getJoyNumber());
		auto state = m_inputStatus.findIf([&](auto& item) { return item.first == slot.name; });
		if (state.hasValue())
		{
			v *= binding->getScale();
			if (v >= binding->GetMinValidMThreshold()) {
				state->second.current = std::max(state->second.current, v);	// Binding が重複したとか、とりあえず大きい方を使う
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
	auto state = m_inputStatus.findIf([&](auto& item) { return item.first == bindingName; });
	if (state) {
		return &state->second;
	}
	else {
		return nullptr;
	}
}

} // namespace ln
