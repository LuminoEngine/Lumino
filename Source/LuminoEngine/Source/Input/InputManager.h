
#pragma once
#include <array>
#include <Lumino/Platform/PlatformEventArgs.h>
#include <Lumino/Input/Common.h>

LN_NAMESPACE_BEGIN
class InputDriver;
class InputGesture;

namespace detail
{

class InputManager
	: public RefObject
{
public:	static const int MaxJoysticks = 4;

	struct Settings
	{
		PlatformWindow*	mainWindow;
	};
	
	static InputManager* getInstance(InputManager* priority = nullptr);

public:
	InputManager();
	virtual ~InputManager();

	void initialize(const Settings& settings);
	void dispose();

	InputController* getVirtualPad(int index) { return m_defaultVirtualPads[index]; }

	void preUpdateFrame();
	void updateFrame();

	//InputDevice* GetInputDevice() { return m_inputDevice; }
	//int GetButtonRepertFirstFrameCount() const { return 20; }
	//int GetButtonRepertIntervalFrameCount()const { return 5; }
	//const Matrix& GetMouseTransform() const { return m_mouseTransform; }

	void onEvent(const PlatformEventArgs& e);


	float getVirtualButtonState(InputGesture* binding, bool keyboard, bool mouse, int joyNumber);

	InputGesture* getAnyActiveTriggered();

private:
	void refreshDevices();

private:
	static const int MaxVirtualPads = 1;
	InputDriver*							m_inputDriver;
	std::array<InputController*, MaxVirtualPads>	m_defaultVirtualPads;
	//Matrix				m_mouseTransform;
	//Mouse*				m_mouse;
	//Keyboard*			m_keyboard;
	//ArrayList<Joypad*>	m_joypadList;

	Ref<InputGesture>	m_lasgAnyActiveTriggered;
	int					m_anyActiveTriggeredFrameCount;
};

} // namespace detail
LN_NAMESPACE_END
