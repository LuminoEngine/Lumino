
#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define INITGUID
#include <dinput.h>
#include <xinput.h>
//#include <LuminoCore/Base/DllLoader.h>
//#include <LuminoCore/Base/Platform.h>
#include "InputDriver.hpp"

namespace ln {

/// ジョイスティック1つ分
class Win32JoystickDriver
    : public RefObject
{
public:

	/// スティックの状態を表す構造体
	struct AnalogState
	{
		int32_t	X1;
		int32_t	Y1;
		int32_t	X2;
		int32_t	Y2;
	};

public:
	Win32JoystickDriver();
	virtual ~Win32JoystickDriver();

public:

	/// 初期化 (xinputNumber は XInput 用コントローラの割り当て番号。XInput 以外のものは -1 を渡すこと)
	void init(IDirectInputDevice8* device, HWND hwnd, int xinputNumber, bool forcefeedback);

	/// 解放
	void dispose();

	/// ジョイパッドの状態を取得する
	void getJoystickDeviceState(JoystickDeviceState* state);

	/// XInput デバイスかを判定する
	bool isXInputDevice() const { return mXInputNo >= 0; }

	/// ジョイパッドの振動を開始する
	void startVibration(int power, int time);

	/// ジョイパッドの振動を停止する
	void stopVibration();

	/// フレーム更新
	void update();

private:

	/// ジョイスティックの軸を列挙する関数
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID thisDevice);

public:

	static const int AXIS_RANGE = 1000;							///< DirectInput に設定する軸の範囲 ( -1000～1000 )
	static const int MAX_BUTTON_NUM = JoystickDeviceState::MaxButtons;	///< ボタンの数
	static const int MAX_AXIS_NUM = JoystickDeviceState::MaxButtons;			///< 軸の数
	

private:
	IDirectInputDevice8*	mDevice;

	IDirectInputEffect*     mDeviceEffect;
	DWORD                   mDIAxes[2];
	LONG                    mDIDirection[2];
	DIPERIODIC              mDIPeriodic;
	DIENVELOPE              mDIEnvelope;
	DIEFFECT                mDIEffect;
	uint32_t				mVibrationStartTime;
	uint32_t				mVibrationTime;

	float                   mPlayRange;
	int                     mXInputNo;
};

class XInputModule
{
public:
	static void init();

	typedef DWORD(WINAPI *MD_XInputGetState)(
		DWORD         dwUserIndex,
		XINPUT_STATE* pState
		);

	typedef DWORD(WINAPI *MD_XInputSetState)(
		DWORD             dwUserIndex,
		XINPUT_VIBRATION* pVibration
		);

	static MD_XInputGetState   XInputGetState;
	static MD_XInputSetState   XInputSetState;

private:
	static HMODULE m_XInputModule;
};

} // namespace ln
