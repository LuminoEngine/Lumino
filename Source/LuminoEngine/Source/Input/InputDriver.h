
#pragma once
#include <array>
#include <Lumino/Platform/PlatformEventArgs.h>
#include <Lumino/Input/Common.h>
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN

/// ジョイパッドの POV の向きを表すフラグ
LN_ENUM_FLAGS(PovDirFlags)
{
	None = 0x00,
	Up = 0x01,
	Right = 0x02,
	Down = 0x04,
	Left = 0x08,
};
LN_ENUM_FLAGS_DECLARE(PovDirFlags);

/// ジョイパッドの状態を表す構造体
struct JoystickDeviceState
{
	static const int MaxButtons = 16;    ///< ジョイパッドのボタンの最大数
	static const int MaxAxis = 8;        ///< ジョイパッドの軸の最大数

	float		Axes[MaxAxis];
	uint32_t	POV;						        ///< POV 状態 (POVDir の組み合わせ  GLFWでは取れない)
	uint8_t		Buttons[MaxButtons];
};

// 
class InputDriver
	: public RefObject
{
public:
	InputDriver();
	virtual ~InputDriver();

	bool QueryKeyState(Keys key);
	bool QueryMouseState(MouseAction action);

	void PreUpdateFrame();






	//bool GetKeyState(Key key) const { return m_keyStatus[(int)key]; }
	//bool GetMouseState(MouseButton button) const { return m_mouseStatus[(int)button]; }
	const PointI& GetMousePoint() const { return m_mousePoint; }
	int GetMouseWheel() const { return m_mouseWheel; }

	virtual int GetJoystickCount() = 0;
	virtual void GetJoystickState(int joystickNumber, JoystickDeviceState* state) = 0;

	virtual void OnEvent(const PlatformEventArgs& e);



	void OnKeyDown(Keys key);
	void OnKeyUp(Keys key);
	void OnMouseButtonDown(MouseButtons button);
	void OnMouseButtonUp(MouseButtons button);



private:
	struct MouseClickTracker
	{
		double		lastTime;
		int			clickCount;
	};
	


	std::array<bool, 256>			m_keyStatus;
	std::array<bool, 8>	m_mouseStatus;
	std::array<MouseClickTracker, 8>	m_mouseClickTrackers;
	float				m_mouseButtonClickTimeout;
	PointI	m_mousePoint;
	int		m_mouseWheel;
};

LN_NAMESPACE_END
