/*
	[2015/12/5]
		ポーリングの入力判定はいままでどおりにする。
		1フレームの間にON/OFFが同時に来たときには ON にするようにもできるけど、
		じゃあ逆のOFFにしたいときはどうするの？とか考えないければならないことが割りとありそう。
		（ほとんどの場合はONだけだろうし、ONだけは対応してもいいかも？いずれにしても後で）
		ON/OFFが同時に来て結果がOFFにしかならないのがイヤならイベントをハンドリングする。

	[2015/12/5]
		キーボードとマウスの入力はウィンドウイベントから取る方がいいと思う。
		GUI 操作中にキャラクターが移動するのも良くないし。

	[2015/12/5]
		イベントは上側の Manager に送ってもらう。
		InputManager が直接 MainWindow にリスナーを登録すると、後々優先度をいじりづらくなるかも。
		(まぁ内部クラスなのでどう変更しても良いけど・・・)
*/
#include "Internal.h"
#include "InputDriver.h"
#include "InputManager.h"
#if defined(LN_OS_WIN32)
	#include "Win32InputDriver.h"
#elif defined(LN_OS_MAC)
	#include "CocoaInputDriver.h"
#endif
#include <Lumino/Platform/PlatformSupport.h>
#include <Lumino/Input/Input.h>
#include <Lumino/Input/InputBinding.h>
#include <Lumino/Input/InputController.h>

LN_NAMESPACE_BEGIN
namespace detail
{

//==============================================================================
// InputManager
//==============================================================================

static InputManager* g_inputManager = nullptr;
	
InputManager* InputManager::GetInstance(InputManager* priority)
{
	return (priority != nullptr) ? priority : g_inputManager;
}
	
//------------------------------------------------------------------------------
InputManager::InputManager()
	: m_inputDriver(nullptr)
	, m_defaultVirtualPads{}
{
}

//------------------------------------------------------------------------------
InputManager::~InputManager()
{
}

//------------------------------------------------------------------------------
void InputManager::Initialize(const Settings& settings)
{
#if defined(LN_OS_WIN32)
	RefPtr<Win32InputDriver> driver(LN_NEW Win32InputDriver());
	driver->Initialize(PlatformSupport::GetWindowHandle(settings.mainWindow));
	m_inputDriver = driver;
#elif defined(LN_OS_MAC)
	RefPtr<CocoaInputDriver> driver(LN_NEW CocoaInputDriver());
	m_inputDriver = driver;
#endif
	
	LN_THROW(m_inputDriver != nullptr, NotImplementedException);

	// TODO: 今は1つだけ
	auto pad = RefPtr<InputController>::MakeRef(this);
	m_defaultVirtualPads[0] = pad;
	m_defaultVirtualPads[0]->AddRef();

	pad->AddBinding(InputButtons::Left,		KeyboardBinding::Create(Keys::Left));
	pad->AddBinding(InputButtons::Right,	KeyboardBinding::Create(Keys::Right));
	pad->AddBinding(InputButtons::Up,		KeyboardBinding::Create(Keys::Up));
	pad->AddBinding(InputButtons::Down,		KeyboardBinding::Create(Keys::Down));
	pad->AddBinding(InputButtons::Ok,		KeyboardBinding::Create(Keys::Z));
	pad->AddBinding(InputButtons::Cancel,	KeyboardBinding::Create(Keys::X));

	pad->AddBinding(InputButtons::Left,		GamepadBinding::Create(GamepadElement::PovLeft));
	pad->AddBinding(InputButtons::Right,	GamepadBinding::Create(GamepadElement::PovRight));
	pad->AddBinding(InputButtons::Up,		GamepadBinding::Create(GamepadElement::PovUp));
	pad->AddBinding(InputButtons::Down,		GamepadBinding::Create(GamepadElement::PovDown));

	pad->AddBinding(InputButtons::Left,		GamepadBinding::Create(GamepadElement::Axis1Minus));
	pad->AddBinding(InputButtons::Right,	GamepadBinding::Create(GamepadElement::Axis1Plus));
	pad->AddBinding(InputButtons::Up,		GamepadBinding::Create(GamepadElement::Axis2Minus));
	pad->AddBinding(InputButtons::Down,		GamepadBinding::Create(GamepadElement::Axis2Plus));
	pad->AddBinding(InputButtons::Ok,		GamepadBinding::Create(GamepadElement::Button1));
	pad->AddBinding(InputButtons::Cancel,	GamepadBinding::Create(GamepadElement::Button2));

	if (g_inputManager == nullptr) {
		g_inputManager = this;
	}
}

//------------------------------------------------------------------------------
void InputManager::Finalize()
{
	for (auto* pad : m_defaultVirtualPads) {
		LN_SAFE_RELEASE(pad);
	}

	if (m_inputDriver != nullptr) {
		LN_SAFE_RELEASE(m_inputDriver);
	}

	if (g_inputManager == this) {
		g_inputManager = nullptr;
	}
}

//------------------------------------------------------------------------------
void InputManager::PreUpdateFrame()
{
	m_inputDriver->PreUpdateFrame();
}

//------------------------------------------------------------------------------
void InputManager::UpdateFrame()
{
	for (auto* pad : m_defaultVirtualPads)
	{
		if (pad != nullptr) {
			pad->UpdateFrame();
		}
	}
}

//------------------------------------------------------------------------------
void InputManager::OnEvent(const PlatformEventArgs& e)
{
	if (m_inputDriver != nullptr) {
		m_inputDriver->OnEvent(e);
	}
}

//------------------------------------------------------------------------------
float InputManager::GetVirtualButtonState(InputBinding* binding, bool keyboard, bool mouse, int joyNumber)
{
	// キーボード
	if (keyboard && binding->GetType() == detail::InputBindingType::Keyboard)
	{
		auto* b = static_cast<KeyboardBinding*>(binding);
		if (b->GetModifierKeys() != ModifierKeys::None) { LN_NOTIMPLEMENTED(); }
		return m_inputDriver->QueryKeyState(b->GetKey()) ? 1.0f : 0.0f;
	}
	// マウス
	if (mouse && binding->GetType() == detail::InputBindingType::Mouse)
	{
		auto* b = static_cast<MouseBinding*>(binding);
		if (b->GetModifierKeys() != ModifierKeys::None) { LN_NOTIMPLEMENTED(); }
		return m_inputDriver->QueryMouseState(b->GetMouseAction()) ? 1.0f : 0.0f;
	}

	// ゲームパッド
	if (binding->GetType() == detail::InputBindingType::Gamepad)
	{
		if (joyNumber >= m_inputDriver->GetJoystickCount()) return 0.0f;

		auto b = static_cast<GamepadBinding*>(binding);
		int e = (int)b->GetElement();
		// ボタン
		if ((int)GamepadElement::Button1 <= e && e <= (int)GamepadElement::Button16)
		{
			int number = e - (int)GamepadElement::Button1;
			JoystickDeviceState state;
			m_inputDriver->GetJoystickState(joyNumber, &state);
			LN_CHECK_RANGE(number, 0, JoystickDeviceState::MaxButtons);
			return state.Buttons[number] ? 1.0f : 0.0f;
		}
		// POV
		if ((int)GamepadElement::PovLeft <= e && e <= (int)GamepadElement::PovDown)
		{
			JoystickDeviceState state;
			m_inputDriver->GetJoystickState(joyNumber, &state);
			if (b->GetElement() == GamepadElement::PovLeft && (state.POV & PovDirFlags::Left)) return 1.0;
			if (b->GetElement() == GamepadElement::PovRight && (state.POV & PovDirFlags::Right)) return 1.0;
			if (b->GetElement() == GamepadElement::PovUp && (state.POV & PovDirFlags::Up)) return 1.0;
			if (b->GetElement() == GamepadElement::PovDown && (state.POV & PovDirFlags::Down)) return 1.0;
			return 0.0f;
		}
		// Axis -1.0 .. 1.0
		if ((int)GamepadElement::Axis1 <= e && e <= (int)GamepadElement::Axis8)
		{
			int number = e - (int)GamepadElement::Axis1;
			JoystickDeviceState state;
			m_inputDriver->GetJoystickState(joyNumber, &state);
			LN_CHECK_RANGE(number, 0, JoystickDeviceState::MaxAxis);
			return state.Axes[number];
		}
		// Axis 0.0 .. 1.0
		if ((int)GamepadElement::Axis1Minus <= e && e <= (int)GamepadElement::Axis8Plus)
		{
			int number = (e - (int)GamepadElement::Axis1Minus) / 2;
			int sign = ((e - (int)GamepadElement::Axis1Minus) % 2 == 0) ? -1.0f : 1.0f;

			JoystickDeviceState state;
			m_inputDriver->GetJoystickState(joyNumber, &state);
			LN_CHECK_RANGE(number, 0, JoystickDeviceState::MaxAxis);
			return state.Axes[number] * sign;
		}
	}
	return 0.0f;
}

} // namespace detail
LN_NAMESPACE_END

#if 0
namespace Lumino
{
namespace Input
{
namespace Private
{

//==============================================================================
// InputManager
//==============================================================================
//
//public:
//	static InputManager& GetInstance();
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//InputManager& InputManager::GetInstance()
//{
//	static InputManager instance;
//	return instance;
//}

//------------------------------------------------------------------------------
InputManager* InputManager::Create(void* hWnd)
{
	return LN_NEW InputManager(hWnd);
}

//------------------------------------------------------------------------------
InputManager::InputManager(void* hWnd)
	: m_inputDevice(NULL)
	, m_mouse(NULL)
	, m_keyboard(NULL)
{
#ifdef _WIN32
	m_inputDevice = LN_NEW Win32InputDevice((HWND)hWnd);
#endif

	m_mouse = LN_NEW Mouse(this);
	m_keyboard = LN_NEW Keyboard(this);

	for (int i = 0; i < MaxJoypads; ++i) {
		m_joypadList.Add(LN_NEW Joypad(this));
	}

	RefreshDevices();
}

//------------------------------------------------------------------------------
InputManager::~InputManager()
{
	LN_SAFE_RELEASE(m_mouse);
	LN_SAFE_RELEASE(m_keyboard);

	LN_FOREACH(Joypad* j, m_joypadList) {
		j->Release();
	}
	m_joypadList.Clear();

	LN_SAFE_RELEASE(m_inputDevice);
}

//------------------------------------------------------------------------------
Mouse* InputManager::GetMouse() const
{
	return m_mouse;
}

//------------------------------------------------------------------------------
void InputManager::UpdateFrame()
{
	m_mouse->UpdateFrame();
	m_keyboard->UpdateFrame();
}

//------------------------------------------------------------------------------
void InputManager::RefreshDevices()
{
	// 認識済みジョイパッドの番号を割り振っていく
	int joyCount = m_inputDevice->GetJoypadCount();
	for (int i = 0; i < MaxJoypads; ++i)
	{
		if (i < joyCount) {
			m_joypadList[i]->SetJoypadNumber(i);
		}
		else {
			m_joypadList[i]->SetJoypadNumber(-1);
		}
	}
}

} // namespace Private
} // namespace Input
} // namespace Lumino
#endif

