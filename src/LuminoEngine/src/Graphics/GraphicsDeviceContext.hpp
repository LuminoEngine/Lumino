#pragma once

namespace ln {
namespace detail {

class GraphicsDeviceContext
	: public RefObject
{
public:
	GraphicsDeviceContext();
	virtual ~GraphicsDeviceContext() = default;

	void initialize();
	virtual void dispose();
};

} // namespace detail
} // namespace ln

