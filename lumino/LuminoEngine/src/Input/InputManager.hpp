
#pragma once
#include <array>
#include <LuminoEngine/Platform/PlatformEvent.hpp>
#include <LuminoEngine/Input/Common.hpp>

namespace ln {
class InputDriver;
class InputGesture;

namespace detail {
class MouseInputDevice;

class InputManager
	: public RefObject
{
	public:	static const int MaxJoysticks = 4;

	struct Settings
	{
		PlatformWindow*	mainWindow;
		InputBindingSet inputConfig;
	};
	

public:

	InputManager();
	virtual ~InputManager();

	void init(const Settings& settings);
	void dispose();

	const Ref<InputDriver>& inputDriver() const { return m_inputDriver; }
	const Ref<InputController>& getVirtualPad(int index) const { return m_defaultVirtualPads[index]; }
	const Ref<MouseInputDevice>& mouseInputDevice() const { return m_mouseInputDevice; }

	// フレーム開始時に状態をリセットする。特に、Platform モジュールがイベントを発行する前に実行しておく必要がある
	void preUpdateFrame();

	void updateFrame(float elapsedTime);

	//InputDevice* GetInputDevice() { return m_inputDevice; }
	//int GetButtonRepertFirstFrameCount() const { return 20; }
	//int GetButtonRepertIntervalFrameCount()const { return 5; }
	//const Matrix& GetMouseTransform() const { return m_mouseTransform; }

	void onEvent(const PlatformEventArgs& e);


	float getVirtualButtonState(InputGesture* binding, bool keyboard, bool mouse, int joyNumber);

	InputGesture* getAnyActiveTriggered();

	float repeatIntervalStartTime() const { return m_repeatIntervalStartTime; }
	float repeatIntervalStepTime() const { return m_repeatIntervalStepTime; }

private:
	void refreshDevices();

private:
	static const int MaxVirtualPads = 1;
	Ref<InputDriver>							m_inputDriver;
	std::array<Ref<InputController>, MaxVirtualPads>	m_defaultVirtualPads;
	Ref<MouseInputDevice> m_mouseInputDevice;
	//Matrix				m_mouseTransform;
	//Mouse*				m_mouse;
	//Keyboard*			m_keyboard;
	//ArrayList<Joypad*>	m_joypadList;

	Ref<InputGesture>	m_lasgAnyActiveTriggered;
	int					m_anyActiveTriggeredFrameCount;

	float m_repeatIntervalStartTime;
	float m_repeatIntervalStepTime;
};

} // namespace detail
} // namespace ln
