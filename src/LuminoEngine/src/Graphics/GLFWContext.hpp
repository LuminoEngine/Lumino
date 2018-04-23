#pragma once

#include "OpenGLDeviceContext.hpp"

namespace ln {
namespace detail {

class GLFWSwapChain
	: public GLSwapChain
{
public:
	virtual ~GLFWSwapChain() = default;
};

class GLFWContext
	: public GLContext
{
public:
	GLFWContext() = default;
	virtual ~GLFWContext() = default;
};

} // namespace detail
} // namespace ln

