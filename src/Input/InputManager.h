
#pragma once
#include <Lumino/Input/Common.h>

LN_NAMESPACE_BEGIN
class InputDriver;

class InputManager
	: public RefObject
{
public:
	static const int MaxJoypads = 4;

	struct Settings
	{
	};

public:
	InputManager();
	virtual ~InputManager();

	void Initialize(const Settings& settings);
	void Finalize();

	/// 入力情報を 1 フレーム分更新
	void UpdateFrame();

	//InputDevice* GetInputDevice() { return m_inputDevice; }
	//int GetButtonRepertFirstFrameCount() const { return 20; }
	//int GetButtonRepertIntervalFrameCount()const { return 5; }
	//const Matrix& GetMouseTransform() const { return m_mouseTransform; }

	void OnEvent(const Platform::EventArgs& e);

private:
	void RefreshDevices();

private:
	InputDriver*		m_InputDriver;
	//Matrix				m_mouseTransform;
	//Mouse*				m_mouse;
	//Keyboard*			m_keyboard;
	//ArrayList<Joypad*>	m_joypadList;
};

LN_NAMESPACE_END
