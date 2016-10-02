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

	pad->AddBinding(InputButtons::Left, InputBinding::CreateKeyboardBinding(Key::Left));
	pad->AddBinding(InputButtons::Right, InputBinding::CreateKeyboardBinding(Key::Right));
	pad->AddBinding(InputButtons::Up, InputBinding::CreateKeyboardBinding(Key::Up));
	pad->AddBinding(InputButtons::Down, InputBinding::CreateKeyboardBinding(Key::Down));
	pad->AddBinding(InputButtons::Ok, InputBinding::CreateKeyboardBinding(Key::Z));
	pad->AddBinding(InputButtons::Cancel, InputBinding::CreateKeyboardBinding(Key::X));


	//pad->AddBinding(InputBinding::CreateJoystickAxisBinding(InputButtons::Left, 0));
	//pad->AddBinding(InputBinding::CreateJoystickAxisBinding(InputButtons::Right, Key::Right));
	//pad->AddBinding(InputBinding::CreateJoystickAxisBinding(InputButtons::Up, Key::Up));
	//pad->AddBinding(InputBinding::CreateJoystickAxisBinding(InputButtons::Down, Key::Down));

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
float InputManager::GetVirtualButtonState(const detail::DeviceInputSource& input, bool keyboard, bool mouse, int joyNumber)
{
	// キーボード
	if (keyboard && input.type == detail::DeviceInputSourceType::Keyboard)
	{
		if (input.Keyboard.modifierKeys != 0) { LN_NOTIMPLEMENTED(); }
		return m_inputDriver->GetKeyState(input.Keyboard.key) ? 1.0f : 0.0f;
	}
	// マウス
	if (mouse && input.type == detail::DeviceInputSourceType::Mouse)
	{
		return m_inputDriver->GetMouseState(input.Mouse.buttonNumber) ? 1.0f : 0.0f;
	}
	// ジョイスティック - ボタン
	if (joyNumber > -1 && input.type == detail::DeviceInputSourceType::JoystickButton)
	{
		JoystickDeviceState state;
		m_inputDriver->GetJoystickState(joyNumber, &state);
		LN_CHECK_RANGE(input.JoystickButton.buttonNumber, 0, JoystickDeviceState::MaxButtons);
		return state.Buttons[input.JoystickButton.buttonNumber] ? 1.0f : 0.0f;
	}
	// ジョイスティック - 軸
	if (joyNumber > -1 && input.type == detail::DeviceInputSourceType::JoystickAxis)
	{
		JoystickDeviceState state;
		m_inputDriver->GetJoystickState(joyNumber, &state);
		LN_CHECK_RANGE(input.JoystickAxis.axizNumber, 0, JoystickDeviceState::MaxAxis);
		return state.Axes[input.JoystickAxis.axizNumber];
	}
	// ジョイスティック - POV
	if (joyNumber > -1 && input.type == detail::DeviceInputSourceType::JoystickPov)
	{
		JoystickDeviceState state;
		m_inputDriver->GetJoystickState(joyNumber, &state);

		if (input.JoystickPov.povAxis == JoystickPovAxis::X)
		{
			if (state.POV & PovDirFlags::Left) return -1.0;
			if (state.POV & PovDirFlags::Right) return 1.0;
			return 0.0f;
		}
		else if (input.JoystickPov.povAxis == JoystickPovAxis::Y)
		{
			if (state.POV & PovDirFlags::Up) return -1.0;
			if (state.POV & PovDirFlags::Down) return 1.0;
			return 0.0f;
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

