
#pragma once
#include "Win32JoystickDriver.h"
#include "InputDriver.h"

LN_NAMESPACE_BEGIN

class Win32InputDriver
	: public InputDriver
{
public:
	Win32InputDriver();
	virtual ~Win32InputDriver();

	void initialize(HWND hWnd);

	virtual int GetJoystickCount() override;
	virtual void GetJoystickState(int joysticNumber, JoystickDeviceState* state) override;

	void StartVibration(int joysticNumber, int power, int time);
	void StopVibration(int joysticNumber);

private:
	void Finalize();
	void RefreshDevice();
	void ReleaseDevice();

	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* instance, VOID* thisInputDevice);
	void CreateJoysticksDevice(const DIDEVICEINSTANCE* instance);
	static BOOL IsXInputDevice(const GUID* pGuidProductFromDirectInput);

private:
	HWND						m_hWnd;
	IDirectInput8*				m_directInput;
	List<Win32JoystickDriver*>	m_joystickList;
	int							m_XInputDeviceCount;
	bool						m_comInited;
};

LN_NAMESPACE_END
