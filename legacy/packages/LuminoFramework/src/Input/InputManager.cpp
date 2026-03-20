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
#include "Internal.hpp"
#include "InputDriver.hpp"
#include "InputManager.hpp"
#if defined(LN_OS_WIN32)
	#include "Win32InputDriver.hpp"
#endif
#include "GLFWInputDriver.hpp"
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>
#include <LuminoFramework/Input/Input.hpp>
#include <LuminoFramework/Input/InputBinding.hpp>
#include <LuminoFramework/Input/InputController.hpp>
#include <LuminoFramework/Input/Mouse.hpp>

namespace ln {
namespace detail
{

//==============================================================================
// InputManager
//==============================================================================
	
//------------------------------------------------------------------------------
InputManager::InputManager()
	: m_inputDriver(nullptr)
	, m_defaultVirtualPads{}
	, m_lasgAnyActiveTriggered(nullptr)
	, m_anyActiveTriggeredFrameCount(0)
	, m_repeatIntervalStartTime(0.7f)
	, m_repeatIntervalStepTime(0.2f)
{
}

//------------------------------------------------------------------------------
InputManager::~InputManager()
{
}

//------------------------------------------------------------------------------
void InputManager::init(const Settings& settings)
{
    LN_LOG_DEBUG("InputManager Initialization started.");

#if defined(LN_OS_WIN32)
	auto driver = makeRef<Win32InputDriver>();
	driver->init((HWND)PlatformSupport::getWin32WindowHandle(settings.mainWindow));
	m_inputDriver = driver;
#elif defined(LN_EMSCRIPTEN) || defined(LN_OS_MAC)
	auto driver = makeRef<GLFWInputDriver>();
	driver->init();
	m_inputDriver = driver;
#else
	auto driver = makeRef<InputDriver>();
	m_inputDriver = driver;
#endif

	// TODO: 今は1つだけ
	auto pad = makeRef<InputController>(this);
	m_defaultVirtualPads[0] = pad;

	if (settings.inputConfig == InputBindingSet::Keyboard) {
		pad->addBinding(InputButtons::Left, KeyGesture::create(Keys::Left));
		pad->addBinding(InputButtons::Right, KeyGesture::create(Keys::Right));
		pad->addBinding(InputButtons::Up, KeyGesture::create(Keys::Up));
		pad->addBinding(InputButtons::Down, KeyGesture::create(Keys::Down));
		pad->addBinding(InputButtons::Submit, KeyGesture::create(Keys::Z));
		pad->addBinding(InputButtons::Submit, KeyGesture::create(Keys::Enter));
		pad->addBinding(InputButtons::Cancel, KeyGesture::create(Keys::X));
		pad->addBinding(InputButtons::Cancel, KeyGesture::create(Keys::Escape));
		pad->addBinding(InputButtons::Menu, KeyGesture::create(Keys::X));
		pad->addBinding(InputButtons::Menu, KeyGesture::create(Keys::Escape));
		pad->addBinding(InputButtons::Shift, KeyGesture::create(Keys::LShift));
		pad->addBinding(InputButtons::Shift, KeyGesture::create(Keys::RShift));
		pad->addBinding(InputButtons::PageUp, KeyGesture::create(Keys::Q));
		pad->addBinding(InputButtons::PageDown, KeyGesture::create(Keys::W));
	}
	else if (settings.inputConfig == InputBindingSet::WASD) {
		pad->addBinding(InputButtons::Left, KeyGesture::create(Keys::A));
		pad->addBinding(InputButtons::Right, KeyGesture::create(Keys::D));
		pad->addBinding(InputButtons::Up, KeyGesture::create(Keys::W));
		pad->addBinding(InputButtons::Down, KeyGesture::create(Keys::S));
		pad->addBinding(InputButtons::Submit, KeyGesture::create(Keys::Space));
		pad->addBinding(InputButtons::Cancel, KeyGesture::create(Keys::Escape));
		pad->addBinding(InputButtons::Menu, KeyGesture::create(Keys::Tab));
		pad->addBinding(InputButtons::Shift, KeyGesture::create(Keys::LShift));
		pad->addBinding(InputButtons::Shift, KeyGesture::create(Keys::RShift));
		pad->addBinding(InputButtons::PageUp, KeyGesture::create(Keys::Q));
		pad->addBinding(InputButtons::PageDown, KeyGesture::create(Keys::E));
	}

	pad->addBinding(InputButtons::Left,		GamepadGesture::create(GamepadElement::PovLeft));
	pad->addBinding(InputButtons::Right,	GamepadGesture::create(GamepadElement::PovRight));
	pad->addBinding(InputButtons::Up,		GamepadGesture::create(GamepadElement::PovUp));
	pad->addBinding(InputButtons::Down,		GamepadGesture::create(GamepadElement::PovDown));

	pad->addBinding(InputButtons::Left,		GamepadGesture::create(GamepadElement::Axis1Minus));
	pad->addBinding(InputButtons::Right,	GamepadGesture::create(GamepadElement::Axis1Plus));
	pad->addBinding(InputButtons::Up,		GamepadGesture::create(GamepadElement::Axis2Minus));
	pad->addBinding(InputButtons::Down,		GamepadGesture::create(GamepadElement::Axis2Plus));
	pad->addBinding(InputButtons::Submit,	GamepadGesture::create(GamepadElement::Button1));
	pad->addBinding(InputButtons::Cancel,	GamepadGesture::create(GamepadElement::Button2));
	pad->addBinding(InputButtons::Menu,		GamepadGesture::create(GamepadElement::Button3));
	pad->addBinding(InputButtons::Shift,	GamepadGesture::create(GamepadElement::Button4));
	pad->addBinding(InputButtons::PageUp,	GamepadGesture::create(GamepadElement::Button5));
	pad->addBinding(InputButtons::PageDown,	GamepadGesture::create(GamepadElement::Button6));

	m_mouseInputDevice = makeRef<MouseInputDevice>();

    LN_LOG_DEBUG("InputManager Initialization ended.");
}

//------------------------------------------------------------------------------
void InputManager::dispose()
{
	for (int i = 0; i < m_defaultVirtualPads.size(); i++) {
		m_defaultVirtualPads[i] = nullptr;
	}

	if (m_inputDriver) {
		m_inputDriver->dispose();
		m_inputDriver = nullptr;
	}
}

//------------------------------------------------------------------------------
void InputManager::preUpdateFrame()
{
	m_inputDriver->preUpdateFrame();
}

//------------------------------------------------------------------------------
void InputManager::updateFrame(float elapsedTime)
{
	for (auto& pad : m_defaultVirtualPads)
	{
		if (pad)
		{
			pad->updateFrame(elapsedTime);
		}
	}

	m_inputDriver->updatePressedAnyGamepadElement();


	if (m_inputDriver->getPressedAnyKey() != nullptr ||
		m_inputDriver->getPressedAnyMouseButton() != nullptr ||
		m_inputDriver->getPressedAnyGamepadElement() != nullptr)
	{
		m_anyActiveTriggeredFrameCount++;
	}
	else
	{
		m_anyActiveTriggeredFrameCount = 0;
	}

	m_mouseInputDevice->updateFrame(elapsedTime);
}

//------------------------------------------------------------------------------
void InputManager::onEvent(const PlatformEventArgs& e)
{
	if (m_inputDriver != nullptr) {
		m_inputDriver->onEvent(e);
	}
}

//------------------------------------------------------------------------------
float InputManager::getVirtualButtonState(InputGesture* binding, bool keyboard, bool mouse, int joyNumber)
{
	// キーボード
	if (keyboard && binding->getType() == detail::InputBindingType::Keyboard)
	{
		auto* b = static_cast<KeyGesture*>(binding);
		if (b->getModifierKeys() != ModifierKeys::None) { LN_NOTIMPLEMENTED(); }
		return m_inputDriver->queryKeyState(b->getKey()) ? 1.0f : 0.0f;
	}
	// マウス
	if (mouse && binding->getType() == detail::InputBindingType::Mouse)
	{
		auto* b = static_cast<MouseGesture*>(binding);
		if (b->getModifierKeys() != ModifierKeys::None) { LN_NOTIMPLEMENTED(); }
		return m_inputDriver->queryMouseState(b->getMouseAction()) ? 1.0f : 0.0f;
	}

	// ゲームパッド
	if (binding->getType() == detail::InputBindingType::Gamepad)
	{
		if (joyNumber >= m_inputDriver->getJoystickCount()) return 0.0f;

		auto b = static_cast<GamepadGesture*>(binding);
		int e = (int)b->getElement();
		// ボタン
		if ((int)GamepadElement::Button1 <= e && e <= (int)GamepadElement::Button16)
		{
			int number = e - (int)GamepadElement::Button1;
			JoystickDeviceState state;
			m_inputDriver->getJoystickState(joyNumber, &state);
			if (LN_REQUIRE_RANGE(number, 0, JoystickDeviceState::MaxButtons)) return 0.0f;
			return state.Buttons[number] ? 1.0f : 0.0f;
		}
		// POV
		if ((int)GamepadElement::PovLeft <= e && e <= (int)GamepadElement::PovDown)
		{
			JoystickDeviceState state;
			m_inputDriver->getJoystickState(joyNumber, &state);
			if (b->getElement() == GamepadElement::PovLeft && (state.POV & PovDirFlags_Left)) return 1.0;
			if (b->getElement() == GamepadElement::PovRight && (state.POV & PovDirFlags_Right)) return 1.0;
			if (b->getElement() == GamepadElement::PovUp && (state.POV & PovDirFlags_Up)) return 1.0;
			if (b->getElement() == GamepadElement::PovDown && (state.POV & PovDirFlags_Down)) return 1.0;
			return 0.0f;
		}
		// Axis -1.0 .. 1.0
		if ((int)GamepadElement::Axis1 <= e && e <= (int)GamepadElement::Axis8)
		{
			int number = e - (int)GamepadElement::Axis1;
			JoystickDeviceState state;
			m_inputDriver->getJoystickState(joyNumber, &state);
			if (LN_REQUIRE_RANGE(number, 0, JoystickDeviceState::MaxAxis)) return 0.0f;
			return state.Axes[number];
		}
		// Axis 0.0 .. 1.0
		if ((int)GamepadElement::Axis1Minus <= e && e <= (int)GamepadElement::Axis8Plus)
		{
			int number = (e - (int)GamepadElement::Axis1Minus) / 2;
			int sign = ((e - (int)GamepadElement::Axis1Minus) % 2 == 0) ? -1.0f : 1.0f;

			JoystickDeviceState state;
			m_inputDriver->getJoystickState(joyNumber, &state);
			if (LN_REQUIRE_RANGE(number, 0, JoystickDeviceState::MaxAxis)) return 0.0f;
			return state.Axes[number] * sign;
		}
	}
	return 0.0f;
}

InputGesture* InputManager::getAnyActiveTriggered()
{
	bool pressed = false;

	// keyboard
	{
		auto* g = m_inputDriver->getPressedAnyKey();
		if (g != nullptr)
		{
			m_lasgAnyActiveTriggered = makeObject_deprecated<KeyGesture>(g->getKey(), g->getModifierKeys());	// TODO: makeObject_deprecated
			pressed = true;
		}
	}
	// mouse
	{
		auto* g = m_inputDriver->getPressedAnyMouseButton();
		if (g != nullptr)
		{
			m_lasgAnyActiveTriggered = makeObject_deprecated<MouseGesture>(g->getMouseAction(), g->getModifierKeys());	// TODO: makeObject_deprecated
			pressed = true;
		}
	}
	// gamepad
	{
		auto* g = m_inputDriver->getPressedAnyGamepadElement();
		if (g != nullptr)
		{
			m_lasgAnyActiveTriggered = makeObject_deprecated<GamepadGesture>(g->getElement());	// TODO: makeObject_deprecated
			pressed = true;
		}
	}

	if (!pressed)
	{
		m_lasgAnyActiveTriggered = nullptr;
	}

	return (m_anyActiveTriggeredFrameCount == 1) ? m_lasgAnyActiveTriggered : nullptr;
}

} // namespace detail
} // namespace ln

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

