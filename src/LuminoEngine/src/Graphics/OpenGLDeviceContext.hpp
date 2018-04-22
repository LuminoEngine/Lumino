#pragma once
#include "GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {

class OpenGLDeviceContext
	: public GraphicsDeviceContext
{
public:
	OpenGLDeviceContext();
	virtual ~OpenGLDeviceContext() = default;

	void initialize();
	void dispose();
};

} // namespace detail
} // namespace ln

