
#pragma once
#include <LuminoPlatform/PlatformEvent.hpp>
#include <LuminoEngine/Input/Common.hpp>
#include <LuminoEngine/Input/InputBinding.hpp>

namespace ln {

// ジョイパッドの POV の向きを表すフラグ
enum PovDirFlags
{
	PovDirFlags_None = 0x00,
	PovDirFlags_Up = 0x01,
	PovDirFlags_Right = 0x02,
	PovDirFlags_Down = 0x04,
	PovDirFlags_Left = 0x08,
};

// ジョイパッドの状態を表す構造体
struct JoystickDeviceState
{
	static const int MaxButtons = 16;    // ジョイパッドのボタンの最大数
	static const int MaxAxis = 8;        // ジョイパッドの軸の最大数

	float		Axes[MaxAxis];			// -1.0～1.0
	uint32_t	POV;					// POV 状態 (POVDir の組み合わせ  GLFWでは取れない)
	uint8_t		Buttons[MaxButtons];

	void reset()
	{
		memset(Axes, 0, sizeof(Axes));
		POV = 0;
		memset(Buttons, 0, sizeof(Buttons));
	}
};

// 
class InputDriver
	: public RefObject
{
public:
	InputDriver();
	virtual ~InputDriver();
	virtual void dispose();

	bool queryKeyState(Keys key);
	bool queryMouseState(MouseAction action);

	void preUpdateFrame();

	




	//bool GetKeyState(Key key) const { return m_keyStatus[(int)key]; }
	//bool GetMouseState(MouseButton button) const { return m_mouseStatus[(int)button]; }
	const PointI& getMousePoint() const { return m_mousePoint; }
	int getMouseWheel() const { return m_mouseWheel; }

	virtual int getJoystickCount();
	virtual void getJoystickState(int joystickNumber, JoystickDeviceState* state);

	virtual void onEvent(const PlatformEventArgs& e);



	void onKeyDown(Keys key);
	void onKeyUp(Keys key);
	void onMouseButtonDown(MouseButtons button);
	void onMouseButtonUp(MouseButtons button);

	void updatePressedAnyGamepadElement();

	// TODO: コピーして使おう
	KeyGesture* getPressedAnyKey() const;
	MouseGesture* getPressedAnyMouseButton() const;
	GamepadGesture* getPressedAnyGamepadElement() const;

private:
	MouseAction toMouseActionSimple(MouseButtons button);
	bool getPressedAnyGamepadElementHelper(int* outPadNumber, GamepadElement* outElement);

	struct MouseClickTracker
	{
		//double		lastTime;
		ElapsedTimer timer;
		int			clickCount;
	};
	


	std::array<bool, 256>				m_keyStatus;
	std::array<bool, 8>					m_mouseStatus;
	std::array<MouseClickTracker, 8>	m_mouseClickTrackers;
	float								m_mouseButtonClickTimeout;
	PointI								m_mousePoint;
	int									m_mouseWheel;

	Ref<KeyGesture>						m_pressedAnyKey;
	Ref<MouseGesture>					m_pressedAnyMouseButton;
	Ref<GamepadGesture>					m_pressedAnyGamepadElement;
};

} // namespace ln
