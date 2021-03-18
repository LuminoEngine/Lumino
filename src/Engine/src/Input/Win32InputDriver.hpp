
#pragma once
#include "InputDriver.hpp"
#include "Win32JoystickDriver.hpp"

namespace ln {
class Win32JoystickDriver;

class Win32InputDriver
	: public InputDriver
{
public:
	Win32InputDriver();
	virtual ~Win32InputDriver();

	void init(HWND hWnd);
	virtual void dispose() override;

	virtual int getJoystickCount() override;
	virtual void getJoystickState(int joysticNumber, JoystickDeviceState* state) override;

	void startVibration(int joysticNumber, int power, int time);
	void stopVibration(int joysticNumber);

private:
	void refreshDevice();
	void releaseDevice();

	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* instance, VOID* thisInputDevice);
	void CreateJoysticksDevice(const DIDEVICEINSTANCE* instance);
	static BOOL isXInputDevice(const GUID* pGuidProductFromDirectInput);

private:
	HWND						m_hWnd;
	IDirectInput8*				m_directInput;
	List<Ref<Win32JoystickDriver>>	m_joystickList;
	int							m_XInputDeviceCount;
	bool						m_comInited;
};

} // namespace ln
