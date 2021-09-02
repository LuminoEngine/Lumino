
#if _WIN32
#include "Internal.hpp"
#include <algorithm>
#include <LuminoCore/Base/Platform.hpp>
#include "Win32JoystickDriver.hpp"

namespace ln {

GUID GUID_SINE = { 0x13541C23, 0x8E33, 0x11D0, 0x9A, 0xD0, 0x00, 0xA0, 0xC9, 0xA0, 0x6E, 0x35 };

// Joystick GUID
GUID GUIDDIJOYSTICK[8] =
{
	// Data1    Data2   Data3   Data4
	0xa36d02e0,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e1,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e2,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f4,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f5,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e3,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02e4,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
	0xa36d02f2,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00",
} ;

// Joystick device object data
DIOBJECTDATAFORMAT C_ODFDIJOYSTICK[44] =
{
	// pguid            dwOfs   dwType     dwFlags
	&GUIDDIJOYSTICK[0],	0x00,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[1],	0x04,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[2],	0x08,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[3],	0x0c,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[4],	0x10,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[5],	0x14,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[6],	0x18,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[6],	0x1c,	0x80ffff03,	0x100,
	&GUIDDIJOYSTICK[7],	0x20,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x24,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x28,	0x80ffff10,	0x000,
	&GUIDDIJOYSTICK[7],	0x2c,	0x80ffff10,	0x000,
	NULL,				0x30,	0x80ffff0c,	0x000,
	NULL,				0x31,	0x80ffff0c,	0x000,
	NULL,				0x32,	0x80ffff0c,	0x000,
	NULL,				0x33,	0x80ffff0c,	0x000,
	NULL,				0x34,	0x80ffff0c,	0x000,
	NULL,				0x35,	0x80ffff0c,	0x000,
	NULL,				0x36,	0x80ffff0c,	0x000,
	NULL,				0x37,	0x80ffff0c,	0x000,
	NULL,				0x38,	0x80ffff0c,	0x000,
	NULL,				0x39,	0x80ffff0c,	0x000,
	NULL,				0x3a,	0x80ffff0c,	0x000,
	NULL,				0x3b,	0x80ffff0c,	0x000,
	NULL,				0x3c,	0x80ffff0c,	0x000,
	NULL,				0x3d,	0x80ffff0c,	0x000,
	NULL,				0x3e,	0x80ffff0c,	0x000,
	NULL,				0x3f,	0x80ffff0c,	0x000,
	NULL,				0x40,	0x80ffff0c,	0x000,
	NULL,				0x41,	0x80ffff0c,	0x000,
	NULL,				0x42,	0x80ffff0c,	0x000,
	NULL,				0x43,	0x80ffff0c,	0x000,
	NULL,				0x44,	0x80ffff0c,	0x000,
	NULL,				0x45,	0x80ffff0c,	0x000,
	NULL,				0x46,	0x80ffff0c,	0x000,
	NULL,				0x47,	0x80ffff0c,	0x000,
	NULL,				0x48,	0x80ffff0c,	0x000,
	NULL,				0x49,	0x80ffff0c,	0x000,
	NULL,				0x4a,	0x80ffff0c,	0x000,
	NULL,				0x4b,	0x80ffff0c,	0x000,
	NULL,				0x4c,	0x80ffff0c,	0x000,
	NULL,				0x4d,	0x80ffff0c,	0x000,
	NULL,				0x4e,	0x80ffff0c,	0x000,
	NULL,				0x4f,	0x80ffff0c,	0x000,
};

//==============================================================================
// Win32JoystickDriver
//==============================================================================

//------------------------------------------------------------------------------
Win32JoystickDriver::Win32JoystickDriver()
	: mDevice(NULL)
	, mDeviceEffect(NULL)
	, mVibrationStartTime(0)
	, mVibrationTime(0)
{
}

//------------------------------------------------------------------------------
Win32JoystickDriver::~Win32JoystickDriver()
{
	dispose();
}

//------------------------------------------------------------------------------
void Win32JoystickDriver::init( IDirectInputDevice8* device, HWND hwnd, int xinputNumber, bool forcefeedback )
{
	HRESULT hr;
	XInputModule::init();
	dispose();

	mDevice = device;
	mXInputNo = xinputNumber;

	if (!isXInputDevice())
	{
		// ジョイスティックとしてデータフォーマットを設定
		// (データ取得に DIJOYSTATE2 構造体を使う)
		hr = mDevice->SetDataFormat(&c_dfDIJoystick2);
		if (LN_ENSURE(SUCCEEDED(hr))) {
			return;
		}

		// フォアグラウンド・排他モード
		//LN_COMCALL(mDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE));
		//LN_COMCALL(mDevice->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE));

		// コールバック関数を使って各軸のモードを設定
		hr = mDevice->EnumObjects(EnumAxesCallback, this, DIDFT_AXIS);
		if (LN_ENSURE(SUCCEEDED(hr))) {
			return;
		}

		if (forcefeedback)
		{
			// エフェクト周期設定
			ZeroMemory(&mDIPeriodic, sizeof(mDIPeriodic));
			mDIPeriodic.dwMagnitude = DI_FFNOMINALMAX; // エフェクトの強さ(0～10,000)
			mDIPeriodic.lOffset = 0;
			mDIPeriodic.dwPhase = 0;
			mDIPeriodic.dwPeriod = (DWORD)(1.5 * DI_SECONDS); // エフェクトの間隔

			// エンベロープ
			ZeroMemory(&mDIEnvelope, sizeof(mDIEnvelope));
			mDIEnvelope.dwSize = sizeof(DIENVELOPE);
			mDIEnvelope.dwAttackLevel = 0;
			mDIEnvelope.dwAttackTime = (DWORD)(0.5 * DI_SECONDS);
			mDIEnvelope.dwFadeLevel = 0;
			mDIEnvelope.dwFadeTime = (DWORD)(1.0 * DI_SECONDS);

			// エフェクト軸
			mDIAxes[0] = DIJOFS_X;
			mDIAxes[1] = DIJOFS_Y;

			// エフェクト方向
			mDIDirection[0] = 0;
			mDIDirection[1] = 0;

			// 振動エフェクト設定
			ZeroMemory(&mDIEffect, sizeof(mDIEffect));
			mDIEffect.dwSize = sizeof(mDIEffect);
			mDIEffect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;//DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
			mDIEffect.dwDuration = INFINITE; // エフェクト継続時間
			mDIEffect.dwSamplePeriod = 0;
			mDIEffect.dwGain = DI_FFNOMINALMAX;
			mDIEffect.dwTriggerButton = DIEB_NOTRIGGER;
			mDIEffect.dwTriggerRepeatInterval = 0;
			mDIEffect.cAxes = 2;
			mDIEffect.rgdwAxes = mDIAxes;
			mDIEffect.rglDirection = mDIDirection;
			mDIEffect.lpEnvelope = &mDIEnvelope; // エンベロープ構造体
			mDIEffect.cbTypeSpecificParams = sizeof(mDIPeriodic); // エフェクト周期構造体のサイズ
			mDIEffect.lpvTypeSpecificParams = &mDIPeriodic; // エフェクト周期構造体

			// エフェクト生成
			hr = mDevice->CreateEffect(GUID_SINE, &mDIEffect, &mDeviceEffect, NULL);
			if (LN_ENSURE(SUCCEEDED(hr))) {
				return;
			}
		}

		mDevice->Acquire();
	}
}

//------------------------------------------------------------------------------
void Win32JoystickDriver::dispose()
{
    if ( mDeviceEffect )
    {
        mDeviceEffect->Unload();
		LN_COM_SAFE_RELEASE( mDeviceEffect );
    }

    // 入力を停止してジョイスティックデバイスを解放
	if ( mDevice )
	{
		mDevice->Unacquire();
		LN_COM_SAFE_RELEASE(mDevice);
	}
}

//------------------------------------------------------------------------------
void Win32JoystickDriver::getJoystickDeviceState(JoystickDeviceState* joyState)
{
	// XInput
	if (isXInputDevice())
	{
		XINPUT_STATE state;
		if (XInputModule::XInputGetState(mXInputNo, &state) == ERROR_SUCCESS)
		{
			// ボタン
			joyState->Buttons[0] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
			joyState->Buttons[1] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
			joyState->Buttons[2] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
			joyState->Buttons[3] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
			joyState->Buttons[4] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
			joyState->Buttons[5] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
			joyState->Buttons[6] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
			joyState->Buttons[7] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
			joyState->Buttons[8] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0;
			joyState->Buttons[9] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0;

			// 軸
			float inv_s16 = 1.0f / 32767.f;
			float inv_u8 = 1.0f / 255.f;
			joyState->Axes[0] = inv_s16 * static_cast<float>(state.Gamepad.sThumbLX);
			joyState->Axes[1] = inv_s16 * static_cast<float>(state.Gamepad.sThumbLY);
			joyState->Axes[2] = inv_s16 * static_cast<float>(state.Gamepad.sThumbRX);
			joyState->Axes[3] = inv_s16 * static_cast<float>(state.Gamepad.sThumbRY);
			joyState->Axes[4] = inv_u8  * static_cast<float>(state.Gamepad.bLeftTrigger);
			joyState->Axes[5] = inv_u8  * static_cast<float>(state.Gamepad.bRightTrigger);
			joyState->Axes[6] = 0.0f;
			joyState->Axes[7] = 0.0f;

			// 軸の範囲は -32768 ～ 32767。32767.f で割るだけだと -方向が 1.0f よりも多くなるので、その辺の修正
			for (int i = 0; i < 4; ++i)
			{
				joyState->Axes[i] = std::max(joyState->Axes[i], -1.0f);
			}

			// POV
			joyState->POV =
				(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? PovDirFlags_Left : 0 |
				(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? PovDirFlags_Right : 0 |
				(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? PovDirFlags_Up : 0 |
				(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? PovDirFlags_Down : 0;

			/*
			for ( int i = 0; i < 10; ++i )
			{
			printf( "%d ", mButtonState[ i ] );
			}
			printf( "b:%x  pov:%x\n", state.Gamepad.wButtons, mPOVState );

			for ( int i = 0; i < 6; ++i )
			{
			printf( "%f ", mAxisState[ i ] );
			}
			printf( "\n" );
			*/
		}
	}
	// DirectInput
	else
	{
		DIJOYSTATE2 state;
		HRESULT hr = mDevice->GetDeviceState(sizeof(DIJOYSTATE2), &state);
		if (SUCCEEDED(hr))
		{
			// ボタン
			for (int i = 0; i < JoystickDeviceState::MaxButtons; ++i)
			{
				joyState->Buttons[i] = (state.rgbButtons[i] & 0x80) != 0x00;
			}

			// 軸
			static const float r_range = 1.0f / AXIS_RANGE;
			joyState->Axes[0] = r_range * state.lX;
			joyState->Axes[1] = r_range * state.lY;
			joyState->Axes[2] = r_range * state.lZ;
			joyState->Axes[3] = r_range * state.lRz;
			joyState->Axes[4] = r_range * state.lRx;
			joyState->Axes[5] = r_range * state.lRy;
			joyState->Axes[6] = 0.0f;
			joyState->Axes[7] = 0.0f;

			// POV
			uint32_t pov = state.rgdwPOV[0];
			int dx, dy;
			if (LOWORD(pov) != 0xffff)
			{
				if (pov > 0 && pov < 18000) {
					dx = 1; 	        // 右
				}
				else if (pov > 18000) {
					dx = -1;
				}		        // 左
				else {
					dx = 0;
				}

				if (pov > 9000 && pov < 27000) {
					dy = 1;
				}	    // 下
				else if (pov < 9000 || pov > 27000) {
					dy = -1;
				}	// 上
				else {
					dy = 0;
				}
			}
			else
			{
				dx = dy = 0;
			}
			joyState->POV =
				((dx == 1) ? PovDirFlags_Right : 0) |
				((dx == -1) ? PovDirFlags_Left : 0) |
				((dy == 1) ? PovDirFlags_Down : 0) |
				((dy == -1) ? PovDirFlags_Up : 0);

			/*
			printf( "%f %f %f %f %f %f\n",
			mAxisState[ 0 ],
			mAxisState[ 1 ],
			mAxisState[ 2 ],
			mAxisState[ 3 ],
			mAxisState[ 4 ],
			mAxisState[ 5 ] );
			*/
		}
		else
		{
			mDevice->Acquire();
			memset(joyState, 0, sizeof(JoystickDeviceState));
		}
	}

	// 軸の遊び潰し
	//for (int i = 0; i < MAX_AXIS_NUM; ++i)
	//{
	//	if (abs(mAxisState[i]) < mPlayRange)
	//	{
	//		mAxisState[i] = 0.0f;
	//	}
	//}
}

//------------------------------------------------------------------------------
void Win32JoystickDriver::startVibration( int power, int time )
{
	if (isXInputDevice())
	{
		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = power * 65535 / 1000;
		vibration.wRightMotorSpeed = power * 65535 / 1000;
		XInputModule::XInputSetState(mXInputNo, &vibration);
	}
	else
	{
		if (mDeviceEffect)
		{
			mDIPeriodic.dwMagnitude = 10 * std::max(0, std::min(power, 1000));

			if (time > 0)
			{
				mDIPeriodic.dwPeriod = std::max(0, time) * 10000; // 普通は 1000 だけど、なんか途中で振動が一瞬弱くなる
				//mDIEffect.dwDuration = mDIPeriodic.dwPeriod = time_ * 1000;
			}

			mDeviceEffect->SetParameters(&mDIEffect, DIEP_TYPESPECIFICPARAMS | DIEP_START);
			mDeviceEffect->Start(1, 0);
		}
	}

	if (time > 0)
	{
		mVibrationTime = time;
		mVibrationStartTime = ::GetTickCount();
	}
	else
	{
		mVibrationTime = 0;
		mVibrationStartTime = 0;
	}
}

//------------------------------------------------------------------------------
void Win32JoystickDriver::stopVibration()
{
	if (isXInputDevice())
	{
		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;
		XInputModule::XInputSetState(mXInputNo, &vibration);
	}
	else
	{
		if (mDeviceEffect)
		{
			mDeviceEffect->Stop();
			mVibrationStartTime = 0;
			mVibrationTime = 0;
		}
	}
}

//------------------------------------------------------------------------------
void Win32JoystickDriver::update()
{
	// 振動終了の監視
	if (mVibrationStartTime > 0)
	{
		if (::GetTickCount() >= mVibrationStartTime + mVibrationTime)
		{
			stopVibration();
		}
	}
}

//------------------------------------------------------------------------------
BOOL CALLBACK Win32JoystickDriver::EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID thisDevice )
{
	Win32JoystickDriver* self = static_cast<Win32JoystickDriver*>( thisDevice );

	// 軸の値の範囲を設定
	DIPROPRANGE diprg;
	ZeroMemory( &diprg, sizeof( diprg ) );
	diprg.diph.dwSize		= sizeof( diprg );
	diprg.diph.dwHeaderSize	= sizeof( diprg.diph ); 
	diprg.diph.dwObj		= lpddoi->dwType;
	diprg.diph.dwHow		= DIPH_BYID;
	diprg.lMin				= -AXIS_RANGE;
	diprg.lMax				= AXIS_RANGE;
	HRESULT hr = self->mDevice->SetProperty( DIPROP_RANGE, &diprg.diph );
	if ( FAILED( hr ) ) { 
		return DIENUM_STOP; 
	}

	return DIENUM_CONTINUE;
}

//==============================================================================
// XInputModule

XInputModule::MD_XInputGetState	XInputModule::XInputGetState;
XInputModule::MD_XInputSetState	XInputModule::XInputSetState;
HMODULE XInputModule::m_XInputModule;

void XInputModule::init()
{
	if (!XInputGetState)
	{
		const WCHAR* names[] =
		{
			XINPUT_DLL,
			L"xinput1_3.dll",
			L"xinput1_2.dll",
			L"xinput1_1.dll",
		};
		for (int i = 0; i < LN_ARRAY_SIZE_OF(names); i++)
		{
			m_XInputModule = ::LoadLibraryW(XINPUT_DLL);
			if (m_XInputModule) {
				break;
			}
		}
		if (LN_ENSURE_IO(m_XInputModule)) {	// TODO: FileNotFoundException はよく使うのであったほうがいいかも
			return;
		}

		XInputGetState = reinterpret_cast<MD_XInputGetState>(::GetProcAddress(m_XInputModule, "XInputGetState"));
		XInputSetState = reinterpret_cast<MD_XInputSetState>(::GetProcAddress(m_XInputModule, "XInputSetState"));
	}
}

} // namespace ln

#endif
