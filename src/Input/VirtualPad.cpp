
#include "../Internal.h"
#include "InputManager.h"
#include "VirtualPad.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// VirtualPad
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VirtualPad::VirtualPad(detail::InputManager* manager)
	: m_manager(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VirtualPad::~VirtualPad()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VirtualPad::AttachBinding(InputBinding* binding)
{
	m_bindings.Add(RefPtr<InputBinding>(binding));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VirtualPad::DetachBinding(InputBinding* binding)
{
	m_bindings.Remove(binding);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VirtualPad::UpdateFrame()
{
	//SortedArray<String, InputState>::iterator itr = m_inputStatus.begin();
	//SortedArray<String, InputState>::iterator end = m_inputStatus.end();
	//for (; itr != end; ++itr) {
	//	(*itr).second.current = 0;
	//}
	for (auto& state : m_inputStatus) {
		state.second.current = 0;
	}

	// m_inputStatus に現在の入力値を展開する
	for (auto& binding : m_bindings)
	{
		float v = m_manager->GetVirtualButtonState(binding->GetDeviceInputSource(), true, true);
		InputState* state = m_inputStatus.Find(binding->GetName());
		if (state != nullptr)
		{
			v = (binding->IsNegativeValue()) ? -v : v;
			if (v >= binding->GetMinValidMThreshold()) {
				state->current = std::max(state->current, v);	// Binding が重複したとか、とりあえず大きい方を使う
			}
		}
	}

	// 現在の入力値から状態を遷移させる
	for (auto& state : m_inputStatus)
	{
		if (state.second.current > 0.0f) {
			state.second.state++;	// 押されてる間は毎フレームインクリメントする
		}
		else
		{
			if (state.second.state > 0.0f) {
				state.second.state = -1;	// 離された瞬間のフレーム
			}
			else {
				state.second.state = 0;		// 離された瞬間の次のフレーム
			}
		}
	}
}

LN_NAMESPACE_END
