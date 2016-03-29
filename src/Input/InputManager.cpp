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
#ifdef LN_OS_WIN32
	#include "Win32InputDriver.h"
#endif
#include <Lumino/Input/Input.h>
#include <Lumino/Input/InputBinding.h>
#include <Lumino/Input/InputController.h>

LN_NAMESPACE_BEGIN
namespace detail
{

InputManager* GetInputManager(InputManager* priority)
{
	if (priority != NULL) return priority;
	return InputManager::Instance;
}

//=============================================================================
// InputManager
//=============================================================================

InputManager* InputManager::Instance = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager::InputManager()
	: m_inputDriver(nullptr)
	, m_defaultVirtualPads{}
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager::~InputManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::Initialize(const Settings& settings)
{
#ifdef LN_OS_WIN32
	RefPtr<Win32InputDriver> driver(LN_NEW Win32InputDriver());
	driver->Initialize(PlatformSupport::GetWindowHandle(settings.mainWindow));
	m_inputDriver = driver;
#endif

	auto pad = RefPtr<InputController>::Construct(this);
	m_defaultVirtualPads[0] = pad;
	m_defaultVirtualPads[0]->AddRef();

	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Left, Key::Left));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Right, Key::Right));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Up, Key::Up));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Down, Key::Down));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Ok, Key::Z));
	pad->AddBinding(RefPtr<InputBinding>::Construct(InputButtons::Cancel, Key::X));

	if (Instance == nullptr) {
		Instance = this;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::Finalize()
{
	for (auto* pad : m_defaultVirtualPads) {
		LN_SAFE_RELEASE(pad);
	}

	if (m_inputDriver != nullptr) {
		LN_SAFE_RELEASE(m_inputDriver);
	}

	if (Instance == this) {
		Instance = nullptr;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::UpdateFrame()
{
	for (auto* pad : m_defaultVirtualPads)
	{
		if (pad != nullptr) {
			pad->UpdateFrame();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::OnEvent(const PlatformEventArgs& e)
{
	if (m_inputDriver != nullptr) {
		m_inputDriver->OnEvent(e);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float InputManager::GetVirtualButtonState(const detail::DeviceInputSource& input, bool keyboard, bool mouse)
{
	// キーボード
	if (input.id & detail::DeviceInputSource::KeyboardFlag)
	{
		uint32_t k = input.id & detail::DeviceInputSource::ValumeMask;
		return m_inputDriver->GetKeyState((Key)k) ? 1.0f : 0.0f;
	}
	// マウス
	if (input.id & detail::DeviceInputSource::MouseFlag)
	{
		uint32_t k = input.id & detail::DeviceInputSource::ValumeMask;
		return m_inputDriver->GetMouseState((MouseButton::enum_type)k) ? 1.0f : 0.0f;
	}
	// ジョイスティック - ボタン
	if (input.id & detail::DeviceInputSource::JoystickButtonFlag)
	{
		uint32_t number = (input.id & detail::DeviceInputSource::JoystickNumberMask) >> 12;
		LN_THROW(0, NotImplementedException);
	}
	// ジョイスティック - 軸
	if (input.id & detail::DeviceInputSource::JoystickAxisFlag)
	{
		LN_THROW(0, NotImplementedException);
	}
	// ジョイスティック - POV
	if (input.id & detail::DeviceInputSource::JoystickPovFlag)
	{
		LN_THROW(0, NotImplementedException);
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

//=============================================================================
// InputManager
//=============================================================================
//
//public:
//	static InputManager& GetInstance();
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//InputManager& InputManager::GetInstance()
//{
//	static InputManager instance;
//	return instance;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager* InputManager::Create(void* hWnd)
{
	return LN_NEW InputManager(hWnd);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Mouse* InputManager::GetMouse() const
{
	return m_mouse;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::UpdateFrame()
{
	m_mouse->UpdateFrame();
	m_keyboard->UpdateFrame();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
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

