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
#include "../Internal.h"
#include "InputDriver.h"
#include "InputManager.h"
//#ifdef _WIN32
//	#include "Win32/Win32InputDevice.h"
//#endif
//#include "Mouse.h"
//#include "Keyboard.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// InputManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputManager::InputManager()
	: m_InputDriver(nullptr)
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
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::Finalize()
{
	LN_SAFE_RELEASE(m_InputDriver);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::UpdateFrame()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputManager::OnEvent(const Platform::EventArgs& e)
{
}


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

