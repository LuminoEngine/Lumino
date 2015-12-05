
#pragma once
#include <array>
#include <Lumino/Input/Common.h>

LN_NAMESPACE_BEGIN

class InputDriver
	: public RefObject
{
public:
	InputDriver();
	virtual ~InputDriver();
	
	void UpdateFrame();

	virtual void OnEvent(const Platform::EventArgs& e);

private:
	std::array<bool, (int)Key::TERMINATOR>			m_keyStatus;
	std::array<bool, (int)MouseButton::TERMINATOR>	m_mouseStatus;
	Point	m_mousePoint;
	int		m_mouseWheel;
};

LN_NAMESPACE_END
