
#if _WIN32
#include "Internal.hpp"
#include <wbemidl.h>
#include <oleauto.h>
#include <LuminoCore/Base/Platform.hpp>
#include "Win32InputDriver.hpp"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace ln {

//==============================================================================
// Win32InputDriver
//==============================================================================

//------------------------------------------------------------------------------
Win32InputDriver::Win32InputDriver()
	: m_hWnd(nullptr)
	, m_directInput(nullptr)
	, m_XInputDeviceCount(0)
	, m_comInited(false)
{
}

//------------------------------------------------------------------------------
Win32InputDriver::~Win32InputDriver()
{
}



//------------------------------------------------------------------------------
void Win32InputDriver::init(HWND hWnd)
{
	LN_LOG_DEBUG("Win32InputDriver");

	HRESULT hr;
	m_hWnd = hWnd;

	hr = CoCreateInstance(CLSID_DirectInput8, NULL, CLSCTX_ALL, IID_IDirectInput8, (void**)&m_directInput);
	if (LN_ENSURE(SUCCEEDED(hr))) {
		return;
	}
	hr = m_directInput->Initialize(::GetModuleHandle(NULL), DIRECTINPUT_VERSION);
	if (LN_ENSURE(SUCCEEDED(hr))) {
		return;
	}

	refreshDevice();
}

//------------------------------------------------------------------------------
void Win32InputDriver::dispose()
{
	releaseDevice();
	LN_COM_SAFE_RELEASE(m_directInput);
}

//------------------------------------------------------------------------------
int Win32InputDriver::getJoystickCount()
{
	return m_joystickList.size();
}

//------------------------------------------------------------------------------
void Win32InputDriver::getJoystickState(int joystickNumber, JoystickDeviceState* state)
{
	m_joystickList[joystickNumber]->getJoystickDeviceState(state);
}

//------------------------------------------------------------------------------
void Win32InputDriver::startVibration(int joystickNumber, int power, int time)
{
	m_joystickList[joystickNumber]->startVibration(power, time);
}

//------------------------------------------------------------------------------
void Win32InputDriver::stopVibration(int joystickNumber)
{
	m_joystickList[joystickNumber]->stopVibration();
}

//------------------------------------------------------------------------------
void Win32InputDriver::refreshDevice()
{
	releaseDevice();

	// 接続済みのゲームコントローラーデバイスを列挙する
	m_XInputDeviceCount = 0;
	// DI8DEVCLASS_GAMECTRL : XBox 認識するっぽい
	// DI8DEVTYPE_JOYSTICK : 他の認識するっぽい
	HRESULT hr = m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, this, DIEDFL_ATTACHEDONLY);
	if (LN_ENSURE(SUCCEEDED(hr))) {
		return;
	}
}

//------------------------------------------------------------------------------
void Win32InputDriver::releaseDevice()
{
	m_joystickList.clear();
	m_XInputDeviceCount = 0;
}

//------------------------------------------------------------------------------
BOOL CALLBACK Win32InputDriver::EnumJoysticksCallback(const DIDEVICEINSTANCE* instance, VOID* thisInputDevice)
{
	Win32InputDriver* self = static_cast<Win32InputDriver*>(thisInputDevice);
	self->CreateJoysticksDevice(instance);
	return DIENUM_CONTINUE;
}

//------------------------------------------------------------------------------
void Win32InputDriver::CreateJoysticksDevice(const DIDEVICEINSTANCE* instance)
{
	if (isXInputDevice(&instance->guidProduct))
	{
		// デバイスの情報を取得するため、一度デバイスを作る
		IDirectInputDevice8* device = NULL;
		HRESULT hr = m_directInput->CreateDevice(instance->guidInstance, &device, NULL);
		if SUCCEEDED(hr)
		{
			DIDEVICEINSTANCE di = { 0 };
			device->GetDeviceInfo(&di);
			//printf( "XInput Device  No:%d Name:%s\n", mXInputNo, di.tszInstanceName );
		}
		LN_COM_SAFE_RELEASE(device);

		auto joystick = makeRef<Win32JoystickDriver>();
		joystick->init(NULL, m_hWnd, m_XInputDeviceCount, true);
		m_joystickList.add(joystick);
		++m_XInputDeviceCount;
	}
	else
	{
		IDirectInputDevice8* device = NULL;
		HRESULT hr = m_directInput->CreateDevice(instance->guidInstance, &device, NULL);
		if (SUCCEEDED(hr))
		{
			// デバイスの情報取得
			DIDEVCAPS caps;
			memset(&caps, 0, sizeof(caps));
			caps.dwSize = sizeof(caps);
			if (SUCCEEDED(device->GetCapabilities(&caps)))
			{
				//DXST_ERRORLOGFMT_ADDA(( "デバイスの登録名:%s" , State.tszInstanceName )) ;
				//DXST_ERRORLOGFMT_ADDA(( "デバイスの製品登録名:%s" , State.tszProductName )) ;
				auto joystick = makeRef<Win32JoystickDriver>();
				joystick->init(device, m_hWnd, -1, (caps.dwFlags & DIDC_FORCEFEEDBACK) != 0);
				m_joystickList.add(joystick);
			}
		}
	}
	/*
	// デバイスのタイプを取得する ( 下位バイトがタイプ )
	lnU32 type = instance_->dwDevType & 0xff;
	// ジョイスティックの場合
	// (DI8DEVTYPE_JOYSTICK だけだとヒットしないコントローラもあるので DI8DEVTYPE_GAMEPADもチェック)
	if ( type == DI8DEVTYPE_JOYSTICK || type == DI8DEVTYPE_GAMEPAD )
	{
	// ジョイスティックのデバイスオブジェクトを作成する
	IDirectInputDevice8* device = NULL;
	HRESULT hr = mDirectInput->CreateDevice( instance_->guidInstance, &device, NULL );
	if ( SUCCEEDED( hr ) )
	{
	// ジョイスティックの管理クラスを作成する
	mJoypads[ mJoypadNum ] = NEW JoypadDevice();
	mJoypads[ mJoypadNum ]->init( device, mWindowHandle );
	++mJoypadNum;
	}
	}
	*/
}

//-------------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it's an XInput device
// Unfortunately this information can not be found by just using DirectInput 
//-------------------------------------------------------------------------------
// http://msdn.microsoft.com/ja-jp/library/bb173051(v=vs.85).aspx
#pragma warning(disable:4996)

BOOL Win32InputDriver::isXInputDevice(const GUID* pGuidProductFromDirectInput)
{
	IWbemLocator*           pIWbemLocator = NULL;
	IEnumWbemClassObject*   pEnumDevices = NULL;
	IWbemClassObject*       pDevices[20] = { 0 };
	IWbemServices*          pIWbemServices = NULL;
	BSTR                    bstrNamespace = NULL;
	BSTR                    bstrDeviceID = NULL;
	BSTR                    bstrClassName = NULL;
	DWORD                   uReturned = 0;
	bool                    bIsXinputDevice = false;
	UINT                    iDevice = 0;
	VARIANT                 var;
	HRESULT                 hr;

	// CoInit if needed
	hr = CoInitialize(NULL);
	bool bCleanupCOM = SUCCEEDED(hr);

	// Create WMI
	hr = CoCreateInstance(
		__uuidof(WbemLocator),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWbemLocator),
		(LPVOID*)&pIWbemLocator);
	if (FAILED(hr) || pIWbemLocator == NULL)
		goto LCleanup;

	bstrNamespace = SysAllocString(L"\\\\.\\root\\cimv2"); if (bstrNamespace == NULL) goto LCleanup;
	bstrClassName = SysAllocString(L"Win32_PNPEntity");   if (bstrClassName == NULL) goto LCleanup;
	bstrDeviceID = SysAllocString(L"DeviceID");          if (bstrDeviceID == NULL)  goto LCleanup;

	// Connect to WMI 
	hr = pIWbemLocator->ConnectServer(bstrNamespace, NULL, NULL, 0L,
		0L, NULL, NULL, &pIWbemServices);
	if (FAILED(hr) || pIWbemServices == NULL)
		goto LCleanup;

	// Switch security level to IMPERSONATE. 
	CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

	hr = pIWbemServices->CreateInstanceEnum(bstrClassName, 0, NULL, &pEnumDevices);
	if (FAILED(hr) || pEnumDevices == NULL)
		goto LCleanup;

	// Loop over all devices
	for (;;)
	{
		// Get 20 at a time
		hr = pEnumDevices->Next(10000, 20, pDevices, &uReturned);
		if (FAILED(hr))
			goto LCleanup;
		if (uReturned == 0)
			break;

		for (iDevice = 0; iDevice<uReturned; iDevice++)
		{
			// For each device, get its device ID
			hr = pDevices[iDevice]->Get(bstrDeviceID, 0L, &var, NULL, NULL);
			if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != NULL)
			{
				// Check if the device ID contains "IG_".  If it does, then it's an XInput device
				// This information can not be found from DirectInput 
				if (wcsstr(var.bstrVal, L"IG_"))
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
					if (strVid && swscanf(strVid, L"VID_%4X", &dwVid) != 1)
						dwVid = 0;
					WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
					if (strPid && swscanf(strPid, L"PID_%4X", &dwPid) != 1)
						dwPid = 0;

					// Compare the VID/PID to the DInput device
					DWORD dwVidPid = MAKELONG(dwVid, dwPid);
					if (dwVidPid == pGuidProductFromDirectInput->Data1)
					{
						bIsXinputDevice = true;
						goto LCleanup;
					}
				}
			}
			LN_COM_SAFE_RELEASE(pDevices[iDevice]);
		}
	}

LCleanup:
	if (bstrNamespace)
		SysFreeString(bstrNamespace);
	if (bstrDeviceID)
		SysFreeString(bstrDeviceID);
	if (bstrClassName)
		SysFreeString(bstrClassName);
	for (iDevice = 0; iDevice<20; iDevice++)
		LN_COM_SAFE_RELEASE(pDevices[iDevice]);
	LN_COM_SAFE_RELEASE(pEnumDevices);
	LN_COM_SAFE_RELEASE(pIWbemLocator);
	LN_COM_SAFE_RELEASE(pIWbemServices);

	if (bCleanupCOM)
		CoUninitialize();

	return bIsXinputDevice;
}

} // namespace ln

#endif
