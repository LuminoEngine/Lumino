
#pragma once
#include <Lumino/Platform/PlatformEvent.hpp>
#include <Lumino/Graphics/GeometryStructs.hpp>

namespace ln {


class Mouse
{
public:
	static Point position();

private:
};

namespace detail {
class InputManager;

class MouseInputDevice
	: public RefObject
{
public:
	MouseInputDevice();

	Point position();

private:
	detail::InputManager* m_manager;
};

} // namespace detail
} // namespace ln
