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
	
InputManager* InputManager::getInstance(InputManager* priority)
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
void InputManager::initialize(const Settings& settings)
{
#if defined(LN_OS_WIN32)
	RefPtr<Win32InputDriver> driver(LN_NEW Win32InputDriver());
	driver->initialize(PlatformSupport::getWindowHandle(settings.mainWindow));
	m_inputDriver = driver;
#elif defined(LN_OS_MAC)
	RefPtr<CocoaInputDriver> driver(LN_NEW CocoaInputDriver());
	m_inputDriver = driver;
#endif
	
	LN_THROW(m_inputDriver != nullptr, NotImplementedException);

	// TODO: 今は1つだけ
	auto pad = RefPtr<InputController>::makeRef(this);
	m_defaultVirtualPads[0] = pad;
	m_defaultVirtualPads[0]->addRef();

	pad->addBinding(InputButtons::Left,		KeyboardBinding::create(Keys::Left));
	pad->addBinding(InputButtons::Right,	KeyboardBinding::create(Keys::Right));
	pad->addBinding(InputButtons::Up,		KeyboardBinding::create(Keys::Up));
	pad->addBinding(InputButtons::Down,		KeyboardBinding::create(Keys::Down));
	pad->addBinding(InputButtons::OK,		KeyboardBinding::create(Keys::Z));
	pad->addBinding(InputButtons::Cancel,	KeyboardBinding::create(Keys::X));

	pad->addBinding(InputButtons::Left,		GamepadBinding::create(GamepadElement::PovLeft));
	pad->addBinding(InputButtons::Right,	GamepadBinding::create(GamepadElement::PovRight));
	pad->addBinding(InputButtons::Up,		GamepadBinding::create(GamepadElement::PovUp));
	pad->addBinding(InputButtons::Down,		GamepadBinding::create(GamepadElement::PovDown));

	pad->addBinding(InputButtons::Left,		GamepadBinding::create(GamepadElement::Axis1Minus));
	pad->addBinding(InputButtons::Right,	GamepadBinding::create(GamepadElement::Axis1Plus));
	pad->addBinding(InputButtons::Up,		GamepadBinding::create(GamepadElement::Axis2Minus));
	pad->addBinding(InputButtons::Down,		GamepadBinding::create(GamepadElement::Axis2Plus));
	pad->addBinding(InputButtons::OK,		GamepadBinding::create(GamepadElement::Button1));
	pad->addBinding(InputButtons::Cancel,	GamepadBinding::create(GamepadElement::Button2));

	if (g_inputManager == nullptr) {
		g_inputManager = this;
	}
}

//------------------------------------------------------------------------------
void InputManager::dispose()
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
void InputManager::preUpdateFrame()
{
	m_inputDriver->preUpdateFrame();
}

//------------------------------------------------------------------------------
void InputManager::updateFrame()
{
	for (auto* pad : m_defaultVirtualPads)
	{
		if (pad != nullptr) {
			pad->updateFrame();
		}
	}
}

//------------------------------------------------------------------------------
void InputManager::onEvent(const PlatformEventArgs& e)
{
	if (m_inputDriver != nullptr) {
		m_inputDriver->onEvent(e);
	}
}

//------------------------------------------------------------------------------
float InputManager::getVirtualButtonState(InputBinding* binding, bool keyboard, bool mouse, int joyNumber)
{
	// キーボード
	if (keyboard && binding->getType() == detail::InputBindingType::Keyboard)
	{
		auto* b = static_cast<KeyboardBinding*>(binding);
		if (b->getModifierKeys() != ModifierKeys::None) { LN_NOTIMPLEMENTED(); }
		return m_inputDriver->queryKeyState(b->getKey()) ? 1.0f : 0.0f;
	}
	// マウス
	if (mouse && binding->getType() == detail::InputBindingType::Mouse)
	{
		auto* b = static_cast<MouseBinding*>(binding);
		if (b->getModifierKeys() != ModifierKeys::None) { LN_NOTIMPLEMENTED(); }
		return m_inputDriver->queryMouseState(b->getMouseAction()) ? 1.0f : 0.0f;
	}

	// ゲームパッド
	if (binding->getType() == detail::InputBindingType::Gamepad)
	{
		if (joyNumber >= m_inputDriver->getJoystickCount()) return 0.0f;

		auto b = static_cast<GamepadBinding*>(binding);
		int e = (int)b->getElement();
		// ボタン
		if ((int)GamepadElement::Button1 <= e && e <= (int)GamepadElement::Button16)
		{
			int number = e - (int)GamepadElement::Button1;
			JoystickDeviceState state;
			m_inputDriver->getJoystickState(joyNumber, &state);
			if (LN_CHECK_RANGE(number, 0, JoystickDeviceState::MaxButtons)) return 0.0f;
			return state.Buttons[number] ? 1.0f : 0.0f;
		}
		// POV
		if ((int)GamepadElement::PovLeft <= e && e <= (int)GamepadElement::PovDown)
		{
			JoystickDeviceState state;
			m_inputDriver->getJoystickState(joyNumber, &state);
			if (b->getElement() == GamepadElement::PovLeft && (state.POV & PovDirFlags::Left)) return 1.0;
			if (b->getElement() == GamepadElement::PovRight && (state.POV & PovDirFlags::Right)) return 1.0;
			if (b->getElement() == GamepadElement::PovUp && (state.POV & PovDirFlags::Up)) return 1.0;
			if (b->getElement() == GamepadElement::PovDown && (state.POV & PovDirFlags::Down)) return 1.0;
			return 0.0f;
		}
		// Axis -1.0 .. 1.0
		if ((int)GamepadElement::Axis1 <= e && e <= (int)GamepadElement::Axis8)
		{
			int number = e - (int)GamepadElement::Axis1;
			JoystickDeviceState state;
			m_inputDriver->getJoystickState(joyNumber, &state);
			if (LN_CHECK_RANGE(number, 0, JoystickDeviceState::MaxAxis)) return 0.0f;
			return state.Axes[number];
		}
		// Axis 0.0 .. 1.0
		if ((int)GamepadElement::Axis1Minus <= e && e <= (int)GamepadElement::Axis8Plus)
		{
			int number = (e - (int)GamepadElement::Axis1Minus) / 2;
			int sign = ((e - (int)GamepadElement::Axis1Minus) % 2 == 0) ? -1.0f : 1.0f;

			JoystickDeviceState state;
			m_inputDriver->getJoystickState(joyNumber, &state);
			if (LN_CHECK_RANGE(number, 0, JoystickDeviceState::MaxAxis)) return 0.0f;
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
//	static InputManager& getInstance();
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//InputManager& InputManager::getInstance()
//{
//	static InputManager instance;
//	return instance;
//}

//------------------------------------------------------------------------------
InputManager* InputManager::create(void* hWnd)
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

	refreshDevices();
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
void InputManager::refreshDevices()
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

